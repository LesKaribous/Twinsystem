#include "com/Intercom.h"
#include "debug/Console.h"

#define LIDAR_SERIAL Serial1

Intercom::Intercom() : _stream(LIDAR_SERIAL) {}

void Intercom::initialize(){
    LIDAR_SERIAL.begin(115200);
    sendMessage("ping");
}

void Intercom::sendMessage(const char* message) {
    _stream.print(message);
    _stream.write('\n');
    Console::trace("Intercom") << ">" << message << Console::endl;
}

void Intercom::sendMessage(const String& message) {
    _stream.print(message);
    _stream.write('\n');
    Console::trace("Intercom") << ">" << message.c_str() << Console::endl;
}

void Intercom::sendRequest(Request& req){
    sendMessage(req.getPayload());
    req.setStatus(Request::Status::SENT);
    _pendingRequest[req.ID()] = req;
}

void Intercom::sendRequest(const String& payload, long timeout){
    Request req(payload, timeout);
    req.send(*this);
    _pendingRequest[req.ID()] = req;
}

void Intercom::update() {
    
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

void Intercom::onConnectionLost() {
    Console::warn("Intercom") << "Connection lost." << Console::endl;
    _connected = false;
}

void Intercom::onConnectionSuccess(){
    Console::info("Intercom") << "Connection successful." << Console::endl;
    _connected = true;
}

/*
bool Intercom::IsSomethingAtAngle(int angle){
    String s = "isSomethingAtAngle(";
    s += angle;
    s += ");";
    SendCommand(s.c_str());

    long ctime = millis();
    while (millis() - ctime > 1000){
        Intercom::Update();
    }

    if(stopReceived){
        stopReceived = false;
        return true;
    }
    return false;
    
}*/

int Intercom::available(){
    return _readyRequest.size() > 0;
}

Request& Intercom::getReadyRequest(){
    return _pendingRequest[_readyRequest[0]];
}

void Intercom::_processIncomingData() {
    while (_stream.available()) {
        String incomingMessage = _stream.readStringUntil('\n');
        incomingMessage.trim(); // Remove any leading/trailing whitespace or newline characters
        Console::trace("Intercom") << "<" << incomingMessage.c_str() << Console::endl;
        
        if (incomingMessage.startsWith("ping")) {
            onPingReceived();
        } else if (incomingMessage.startsWith("pong")) {
            if(!isConnected())onConnectionSuccess();
        }else if (!_pendingRequest.empty()) {
            int separatorIndex = incomingMessage.indexOf(':');
            if (separatorIndex > 0) {
                uint32_t responseId = incomingMessage.substring(0, separatorIndex).toInt();
                String responseData = incomingMessage.substring(separatorIndex + 1);

                auto requestIt = _pendingRequest.find(responseId);
                if (requestIt != _pendingRequest.end()) {
                    Request& request = requestIt->second;
                    request.onResponse(responseData);
                    _readyRequest.push_back(requestIt->first);
                }
            }
        }
    }
}

void Intercom::_processPendingRequests() {
    for (auto it = _pendingRequest.begin(); it != _pendingRequest.end();) {
        Request& request = it->second;
        Request::Status status = request.getStatus();

        //Console::info("Intercom") << "Request[" << String(request.ID()) << "]:";
    
        if (status == Request::Status::IDLE) {
            //Console::println("IDLE");
            request.send(*this);
            ++it;
        } else if (status == Request::Status::SENT) {
            //Console::println("SENT");
            if (request.isTimedOut()) {
                //Console::trace("Intercom") << "Request[" << String(request.ID()) << "]:" << "TIMEDOUT" << Console::endl;
                request.onTimeout();
                it = _pendingRequest.erase(it); // Remove the request from the map
            } else {
                ++it;
            }
        } else if (status == Request::Status::OK) {
            //Console::println("OK");
            
        } else if (status == Request::Status::CLOSED) {
            it = _pendingRequest.erase(it); // Remove the request from the map
            
        }  else if (status == Request::Status::TIMEOUT) {
            Console::error("Intercom") << ": request " << request.getPayload() << "timedout." << Console::endl;
        } else {
            ++it;
        }
    }
}

uint32_t Request::_uidCounter = 0;

Request::Request()
    :   _uid(0),
        _payload("bad payload"),
        _lastSent(0),
        _timeout(0),
        _status(Status::ERROR)
        {}

Request::Request(const String& payload, long timeout)
    :   _uid(0),
        _payload(payload), 
        _lastSent(0),
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
    _response = response;
}

void Request::onTimeout(){
    _status = Status::TIMEOUT;
}

void Request::setStatus(Status status){
    _status = status;
}
    
u_int32_t Request::ID() const{
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

unsigned long Request::getLastSent() const{
    return _lastSent;
}

