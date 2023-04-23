#include "com/Intercom.h"
#include "debug/Console.h"

#define LIDAR_SERIAL Serial1

namespace TwinSystem{
    Intercom::Intercom() : _stream(LIDAR_SERIAL) {}

    void Intercom::Initialize(){
        LIDAR_SERIAL.begin(115200);
        SendMessage("ping");
    }

    void Intercom::SendMessage(const char* message) {
        _stream.print(message);
        _stream.write('\n');
        Console::trace("Intercom") << ">" << message << Console::endl;
    }

    void Intercom::SendMessage(const String& message) {
        _stream.print(message);
        _stream.write('\n');
        Console::trace("Intercom") << ">" << message.c_str() << Console::endl;
    }

    void Intercom::SendRequest(Request& req){
        SendMessage(req.GetPayload());
        req.SetStatus(Request::Status::SENT);
        _pendingRequest[req.ID()] = req;
    }

    void Intercom::SendRequest(const String& payload, Request::Callback callback, long timeout){
        Request req(payload, callback, timeout);
        req.Send(*this);
        _pendingRequest[req.ID()] = req;
    }

    void Intercom::Update() {
        
        if (_stream.available()) {
            _lastStream = millis();
            _processIncomingData();         
        }

        _processPendingRequests();

        if(millis() - _lastPing > 500 && (!_connected || (_connected && millis() - _lastStream > 2000))){
            SendMessage("ping");
            _lastPing = millis();
        }
        if(_connected && millis() - _lastStream > 4000){
            OnConnectionLost();
        }
    }

    void Intercom::OnPingReceived() {
        SendMessage("pong");
    }

    void Intercom::OnConnectionLost() {
        Console::warn("Intercom") << "Connection lost." << Console::endl;
        _connected = false;
    }

    void Intercom::OnConnectionSuccess(){
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

    void Intercom::_processIncomingData() {
        while (_stream.available()) {
            String incomingMessage = _stream.readStringUntil('\n');
            incomingMessage.trim(); // Remove any leading/trailing whitespace or newline characters
            Console::trace("Intercom") << "<" << incomingMessage.c_str() << Console::endl;
           
           if (incomingMessage.startsWith("ping")) {
                OnPingReceived();
            } else if (incomingMessage.startsWith("pong")) {
                if(!IsConnected())OnConnectionSuccess();
            }else if (!_pendingRequest.empty()) {
                int separatorIndex = incomingMessage.indexOf(':');
                if (separatorIndex > 0) {
                    uint32_t responseId = incomingMessage.substring(0, separatorIndex).toInt();
                    String responseData = incomingMessage.substring(separatorIndex + 1);

                    auto requestIt = _pendingRequest.find(responseId);
                    if (requestIt != _pendingRequest.end()) {
                        Request& request = requestIt->second;
                        request.SetStatus(Request::Status::OK);
                        request.OnResponse(responseData);
                        _pendingRequest.erase(requestIt);
                    }
                }
            }
        }
    }

    void Intercom::_processPendingRequests() {
        for (auto it = _pendingRequest.begin(); it != _pendingRequest.end();) {
            Request& request = it->second;
            Request::Status status = request.GetStatus();

            //Console::info("Intercom") << "Request[" << String(request.ID()) << "]:";
        
            if (status == Request::Status::IDLE) {
                //Console::println("IDLE");
                request.Send(*this);
                ++it;
            } else if (status == Request::Status::SENT) {
                //Console::println("SENT");
                if (request.IsTimedOut()) {
                    //Console::trace("Intercom") << "Request[" << String(request.ID()) << "]:" << "TIMEDOUT" << Console::endl;
                    request.OnTimeout();
                    it = _pendingRequest.erase(it); // Remove the request from the map
                } else {
                    ++it;
                }
            } else if (status == Request::Status::OK) {
                //Console::println("OK");
                it = _pendingRequest.erase(it); // Remove the request from the map
            }  else if (status == Request::Status::TIMEOUT) {
                //Console::println("TIMEDOUT");
                it = _pendingRequest.erase(it); // Remove the request from the map
            } else {
                ++it;
            }
        }
    }

    uint32_t Request::_uidCounter = 0;

    Request::Request()
        :   _payload("bad payload"), 
            _callback(nullptr), 
            _timeout(0),
            _lastSent(0),
            _status(Status::ERROR),
            _uid(0)
            {}

    Request::Request(const String& payload, Callback callback, long timeout)
        :   _payload(payload), 
            _callback(callback), 
            _timeout(timeout),
            _lastSent(0),
            _status(Status::IDLE),
            _uid(0)
            {_uid = _uidCounter++;}


    void Request::Send(Intercom& channel){
        _status = Status::SENT;
        _lastSent = millis();
        channel.SendMessage(GetPayload());
    }

    void Request::OnResponse(const String& response){
        _status = Status::OK;
        if(_callback)(_callback)(response);
    }

    void Request::OnTimeout(){
        _status = Status::TIMEOUT;
    }

    void Request::SetStatus(Status status){
        _status = status;
    }
       
    u_int32_t Request::ID() const{
        return _uid;
    }

    Request::Status Request::GetStatus() const{
        return _status;
    }

    Request::Callback Request::GetCallback() const{
        return _callback;
    }

    const String& Request::GetMessage() const{
        return _payload;
    }

    String Request::GetPayload() const{
        return String(String(_uid) + ":" + _payload);
    }

    bool Request::IsTimedOut() const{
        return millis() - _lastSent >= _timeout;
    }

    unsigned long Request::GetTimeout() const{
        return _timeout;
    }

    unsigned long Request::GetLastSent() const{
        return _lastSent;
    }

}