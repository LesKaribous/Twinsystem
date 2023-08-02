#include "intercom.h"
#include "os.h"

#define LIDAR_SERIAL Serial1

Intercom::Intercom() : Service(INTERCOM),  _stream(LIDAR_SERIAL) {}

void Intercom::enable(){
    Service::enable();
    LIDAR_SERIAL.begin(115200);
    sendMessage("ping");
}

void Intercom::disable(){
    Service::disable();
}

void Intercom::sendMessage(const char* message) {
    _stream.print(message);
    _stream.write('\n');
    os.console.trace("Intercom") << ">" << message << os.console.endl;
}

void Intercom::sendMessage(const String& message) {
    _stream.print(message);
    _stream.write('\n');
    os.console.trace("Intercom") << ">" << message.c_str() << os.console.endl;
}


uint32_t Intercom::sendRequest(const String& payload, long timeout){
    Request req(payload, timeout);
    req.send(*this);
    _requests.insert({req.ID(), req});

    return req.ID();
}

void Intercom::update() {
    if(!m_enabled) return;

    if (_stream.available()) {
        _lastStream = millis();
        _processIncomingData();         
    }

    _processPendingRequests();

    if(millis() - _lastPing > 500 && (!_connected || (_connected && millis() - _lastStream > 2000))){
        sendMessage("ping");
        _lastPing = millis();
    }
    if(_connected && millis() - _lastStream > 4000){
        onConnectionLost();
    }
}

void Intercom::onPingReceived() {
    sendMessage("pong");
}

bool Intercom::closeRequest(const uint32_t& uid) {
    if(_requests.count(uid)){
        _requests.find(uid)->second.close();
        return true;
    }else{
        os.console.warn("Intercom") << __FILE__ << " at line " << __LINE__ << " request " << int(uid) << " does not exist" << os.console.endl;
        false;
    };
}

String Intercom::getRequestResponse(const uint32_t& uid) {
    if(_requests.count(uid) > 0){
        return _requests.find(uid)->second.getResponse();
    }else{
        os.console.warn("Intercom") << __FILE__ << " at line " << __LINE__ << " request " << int(uid)   << " does not exist" << os.console.endl;
        return "ERROR";
    };
}

void Intercom::onConnectionLost() {
   os.console.warn("Intercom") << "Connection lost." << os.console.endl;
    _connected = false;
}

void Intercom::onConnectionSuccess(){
    os.console.info("Intercom") << "Connection successful." << os.console.endl;
    _connected = true;
}



void Intercom::_processIncomingData() {
    while (_stream.available()) {
        String incomingMessage = _stream.readStringUntil('\n');
        incomingMessage.trim(); // Remove any leading/trailing whitespace or newline characters
        os.console.trace("Intercom") << "<" << incomingMessage.c_str() << os.console.endl;
        
        if (incomingMessage.startsWith("ping")) {
            onPingReceived();
        } else if (incomingMessage.startsWith("pong")) {
            if(!isConnected())onConnectionSuccess();
        }else if (!_requests.empty()) {
            int separatorIndex = incomingMessage.indexOf(':');
            if (separatorIndex > 0) {
                uint32_t responseId = incomingMessage.substring(0, separatorIndex).toInt();
                String responseData = incomingMessage.substring(separatorIndex + 1);

                auto requestIt = _requests.find(responseId);
                if (requestIt != _requests.end()) {
                    Request& request = requestIt->second;
                    request.onResponse(responseData);
                }
            }
        }
    }
}

void Intercom::_processPendingRequests() {
    for (auto it = _requests.begin(); it != _requests.end();) {

        
        Request& request = it->second;
        Request::Status status = request.getStatus();
    
        if(status != Request::Status::CLOSED && status != Request::Status::IDLE && millis() - request.getLastSent() > 1000){
            os.console.trace("Intercom") << ": request " << request.getPayload() << "too old, cleared." << os.console.endl;
            request.close();
            ++it;
            continue;
        }

        if (status == Request::Status::IDLE) {
            request.send(*this);
            ++it;
        } else if (status == Request::Status::SENT) {
            if (request.isTimedOut()) {
                request.onTimeout();
                
            } else {
                ++it;
            }
        } else if (status == Request::Status::OK) {
            ++it;
        } else if (status == Request::Status::CLOSED) {
            os.console.trace("Intercom") << int(_requests.size()) << "currently in the buffer" << os.console.endl;
            it = _requests.erase(it); // Remove the request from the map

        }  else if (status == Request::Status::TIMEOUT) {
            request.close();
            os.console.error("Intercom") << ": request " << request.getPayload() << "timedout." << os.console.endl;
        } else if (status == Request::Status::ERROR) {
            request.close();
            os.console.error("Intercom") << ": request " << request.getPayload() << "unknown error." << os.console.endl;
        } else {
            ++it;
        }
    }
}

uint32_t Request::_uidCounter = 0;

Request::Request(const String& payload, long timeout)
    :   _uid(0),
        _payload(payload), 
        _lastSent(0),
        _responseTime(0),
        _timeout(timeout),
        _status(Status::IDLE)
        {_uid = _uidCounter++;}


void Request::send(Intercom& channel){
    _status = Status::SENT;
    _lastSent = millis();
    channel.sendMessage(getPayload());
}

void Request::close(){
    _status = Status::CLOSED;
}

void Request::onResponse(const String& response){
    _status = Status::OK;
    _responseTime = millis();
    _response = response;
}

void Request::onTimeout(){
    _status = Status::TIMEOUT;
}

void Request::setStatus(Status status){
    _status = status;
}
    
uint32_t Request::ID() const{
    return _uid;
}

Request::Status Request::getStatus() const{
    return _status;
}

const String& Request::getMessage() const{
    return _payload;
}

const String& Request::getResponse() const{

    return _response;
}

String Request::getPayload() const{
    return String(String(_uid) + ":" + _payload);
}

bool Request::isTimedOut() const{
    return millis() - _lastSent >= _timeout;
}

unsigned long Request::getTimeout() const{
    return _timeout;
}

unsigned long Request::getResponseTime() const{
    return _responseTime;
}

unsigned long Request::getLastSent() const{
    return _lastSent;
}

