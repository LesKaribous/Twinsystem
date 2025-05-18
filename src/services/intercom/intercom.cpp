#include "intercom.h"
#include "os/console.h"
#include "settings.h"
#include "comUtilities.h"

SINGLETON_INSTANTIATE(Intercom, intercom)

Intercom::Intercom() : Service(ID_INTERCOM),  _stream(INTERCOM_SERIAL) {}

void Intercom::onAttach() {
    Console::info() << "Intercom activated" << Console::endl;
}

void Intercom::onUpdate() {
    if(!enabled()) return;

    if (_stream.available()) {
        _lastStream = millis();
        _processIncomingData();         
    }
    _processPendingRequests();

    if(millis() - _lastPing > 500 && (!_connected || (_connected && millis() - _lastStream > 1000))){
        sendMessage("ping");
        _lastPing = millis();
    }
    if(_connected && millis() - _lastStream > 2000){
        connectionLost();
    }
}

void Intercom::enable(){
    Service::enable();
    INTERCOM_SERIAL.begin(INTERCOM_BAUDRATE);
    sendMessage("ping");
}

void Intercom::disable(){
    INTERCOM_SERIAL.end();
    Service::disable();
}

void Intercom::sendMessage(const char* message) {
    long trial = millis();
    while(!_stream.availableForWrite() && millis() - trial < 100);
    if(millis() - trial > 100){
        THROW("Msg cannot be sent");
        return;
    }
    _stream.print(message);
    _stream.write('\n');
    Console::trace("Intercom") << ">" << message << Console::endl;
}

void Intercom::sendMessage(const String& message) {
    long trial = millis();
    while(!_stream.availableForWrite() && millis() - trial < 100);
    if(millis() - trial > 100){
        THROW("Msg cannot be sent");
        return;
    }
    _stream.print(message);
    _stream.write('\n');
    Console::trace("Intercom") << ">" << message.c_str() << Console::endl;
}

int Intercom::sendRequest(const String& payload, long timeout,  requestCallback_ptr cbfunc,  callback_ptr func){
    Request req(payload, timeout, cbfunc, func);
    req.send();
    _sentRequests.insert({req.ID(), req});
    return req.ID();
}

void Intercom::pingReceived() {
    sendMessage("pong");
}

bool Intercom::closeRequest(int uid) {
    if(_sentRequests.count(uid)){
        _sentRequests.find(uid)->second.close();
        return true;
    }else{
        Console::warn("Intercom") << __FILE__ << " at line " << __LINE__ << " request " << int(uid) << " does not exist" << Console::endl;
        return false;
    };
}

void Intercom::setConnectLostCallback(callback_ptr callback){
    onConnectionLost = callback;
}

void Intercom::setRequestCallback(requestCallback_ptr callback){
     onRequestCallback = callback;
}

void Intercom::setConnectionSuccessCallback(callback_ptr callback){
    onConnectionSuccess = callback;
}


String Intercom::getRequestResponse(int uid) {
    if(_sentRequests.count(uid) > 0){
        return _sentRequests.find(uid)->second.getResponse();
    }else{
        Console::warn("Intercom") << __FILE__ << " at line " << __LINE__ << " request " << int(uid)   << " does not exist" << Console::endl;
        return "ERROR";
    };
}

void Intercom::connectionLost() {
    Console::warn("Intercom") << "Connection lost." << Console::endl;
    _connected = false;
    if(onConnectionLost!=nullptr){
        onConnectionLost();
    }
}

void Intercom::connectionSuccess(){
    Console::info("Intercom") << "Connection successful." << Console::endl;
    _connected = true;
    if(onConnectionSuccess!=nullptr){
        onConnectionSuccess();
    }
}

void Intercom::_processIncomingData() {
    while (_stream.available()) {
       
        String incomingMessage = _stream.readStringUntil('\n'); //We read all bytes hoping that no \n pop before the end
        incomingMessage.trim(); // Remove any leading/trailing whitespace or newline characters

        if (incomingMessage.startsWith("ping")) {
            pingReceived();
        } else if (incomingMessage.startsWith("pong")) {
            if(!isConnected())connectionSuccess();

        }else if (incomingMessage.startsWith("r") && !_sentRequests.empty()){ //reply incomming
            int id_separatorIndex = incomingMessage.indexOf(':');
            int crc_separatorIndex = incomingMessage.indexOf('|');

            if (id_separatorIndex != 0 && crc_separatorIndex != 0) {
                int responseId = incomingMessage.substring(1, id_separatorIndex).toInt(); //get uuid and ignore the 'r'
                String responseData = incomingMessage.substring(id_separatorIndex + 1, crc_separatorIndex); //without crc
                int crc = incomingMessage.substring(crc_separatorIndex + 1).toInt(); //without crc
                
                if(!checkCRC(incomingMessage.substring(0, crc_separatorIndex), crc)){
                    Console::trace("Intercom") << "Bad crc for message " << incomingMessage << Console::endl;
                    continue;
                }

                auto requestIt = _sentRequests.find(responseId);
                if (requestIt != _sentRequests.end()) {
                    Request& request = requestIt->second;
                    request.onResponse(responseData);
                    Console::trace("Intercom") << "<" << incomingMessage << Console::endl;
                }else{
                    Console::trace("Intercom")<< "not found" << Console::endl;
                }
            }

            
        }else{ //request is coming
            int id_separatorIndex = incomingMessage.indexOf(':');
            int crc_separatorIndex = incomingMessage.indexOf('|');
            if (id_separatorIndex != 0 && crc_separatorIndex != 0) {
                int responseId = incomingMessage.substring(0, id_separatorIndex).toInt(); //get uuid and ignore the 'r'
                String responseData = incomingMessage.substring(id_separatorIndex + 1, crc_separatorIndex); //without crc
                int crc = incomingMessage.substring(crc_separatorIndex + 1).toInt(); //without crc
                
                if(!checkCRC(incomingMessage.substring(0, crc_separatorIndex), crc)){
                    Console::error("Intercom") << "Bad crc for message " << incomingMessage << Console::endl;
                    continue;
                }

                Request request(responseId, responseData);
                if(onRequestCallback != nullptr) onRequestCallback(request);
            }
        }
    }
    INTERCOM_SERIAL.clear();
}

void Intercom::_processPendingRequests() {
    for (auto it = _sentRequests.begin(); it != _sentRequests.end();) {

        Request& request = it->second;
        Request::Status status = request.getStatus();
    
        if(status != Request::Status::CLOSED && status != Request::Status::IDLE && millis() - request.getLastSent() > 300){
            Console::trace("Intercom") << ": request " << request.getContent() << "too old, cleared." << Console::endl;
            request.close();
            ++it;
            continue;
        }

        if (status == Request::Status::IDLE) {
            request.send();
            ++it;
        } else if (status == Request::Status::SENT) {
            if (request.isTimedOut()) {
                request.onTimeout();
            } else {
                if(millis() - request.getLastSent() > 10)  request.send();
                ++it;
            }
        } else if (status == Request::Status::OK) {
            ++it;
            request.close();
        } else if (status == Request::Status::CLOSED) {
            Console::trace("Intercom") << int(_sentRequests.size()) << "currently in the buffer" << Console::endl;
            it = _sentRequests.erase(it); // Remove the request from the map

        }  else if (status == Request::Status::TIMEOUT) {
            request.close();
            //CONSOLE_SERIAL.print(request.getPayload());
            Console::trace("Intercom") << "request " << request.getPayload() << " timedout." << Console::endl;
        } else if (status == Request::Status::ERROR) {
            request.close();
            Console::error("Intercom") << "request " << request.getContent() << " unknown error." << Console::endl;
        } else {
            ++it;
        }
    }
}
