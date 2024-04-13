#include "request.h"
#include "comUtilities.h"

int Request::_uidCounter = 0;

Request::Request(const String& content, long timeout, requestCallback_ptr func_call, callback_ptr timeout_call)
    :   _uid(0),
        _content(content), 
        _response(""), 
        _lastSent(0),
        _timeout(timeout),
        _status(Status::IDLE), 
        _callback(func_call),
        _timeoutCallback(timeout_call)
        {
            _uid = _uidCounter++;
            _prefix = "";
            _prefix += String(_uid) + ":";

            String payload = _prefix + _content;

            _crc = String((int) CRC8.smbus((uint8_t*)payload.c_str(), payload.length()));
        }

Request::Request(int id, const String& content)
    :   _uid(id),
        _content(content),
        _response(""), 
        _lastSent(0),
        _timeout(0),
        _status(Status::IDLE), 
        _callback(nullptr),
        _timeoutCallback(nullptr)
        {           
            _prefix = "";
            _prefix +=  "r";
            _prefix += String(_uid) + ":";

            String payload = _prefix + _content;
            
            _crc = String((int) CRC8.smbus((uint8_t*)payload.c_str(), payload.length()));
        }

void Request::setTimeoutCallback(callback_ptr func){
    _timeoutCallback = func;
}

void Request::setCallback(requestCallback_ptr func){
    _callback = func;
}

void Request::send(){
    _status = Status::SENT;
    _lastSent = millis();
    Intercom::instance().sendMessage(getPayload());
}

void Request::reply(const String& answer){
    _status = Status::CLOSED;
    _lastSent = millis();
    _content = answer;
    String payload = _prefix + _content;
    _crc = String((int) CRC8.smbus((uint8_t*)payload.c_str(), payload.length()));
    Intercom::instance().sendMessage(getPayload());
}


void Request::close(){
    _status = Status::CLOSED;
}

void Request::onResponse(const String& response){
    _status = Status::OK;
    _responseTime = millis() - _lastSent;
    _response = response;
    if(_callback) _callback(*this);
}

void Request::onTimeout(){
    _status = Status::TIMEOUT;
    if(_timeoutCallback) _timeoutCallback();
}

void Request::setStatus(Status status){
    _status = status;
}
    
int Request::ID() const{
    return _uid;
}

Request::Status Request::getStatus() const{
    return _status;
}

const String& Request::getContent() const{
    return _content;
}

const String& Request::getResponse() const{

    return _response;
}

//uuidcontentcrc
String Request::getPayload() const{
    return _prefix + _content + "|" + _crc;
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

