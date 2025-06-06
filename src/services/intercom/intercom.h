#pragma once
#include "services/service.h"
#include "request.h"
#include <map>

class Request;

using messageCallback_ptr = void (*)(const String&);
using requestCallback_ptr = void (*)(Request&);

class Intercom : public Service{
public:
    Intercom();

    void attach() override;
    void run() override;

    void enable() override;
    void disable() override;

    void sendMessage(const char* message);
    void sendMessage(const String& message);

    int sendRequest(const String& payload, long timeout = 300, requestCallback_ptr cbfunc = nullptr, callback_ptr func = nullptr);
    bool closeRequest(int);
    String getRequestResponse(int);

    void setConnectLostCallback(callback_ptr callback);
    void setRequestCallback(requestCallback_ptr callback);
    void setConnectionSuccessCallback(callback_ptr callback);

    bool isConnected();

private:

    void pingReceived(); 
    void connectionLost(); 
    void connectionSuccess(); 

    bool debug = false;

    Stream& _stream;
    std::map<int,Request> _sentRequests;
    //std::map<uint32_t,Request> _receivedRequests;

    requestCallback_ptr onRequestCallback = nullptr;
    callback_ptr onConnectionLost = nullptr;
    callback_ptr onConnectionSuccess = nullptr;

    unsigned long _lastStream = 0;
    unsigned long _lastPing = 0;
    bool _connected = false;

    void _sendRequest(Request& req);
    void _processIncomingData();
    void _processPendingRequests();

    SINGLETON(Intercom)
};
SINGLETON_EXTERN(Intercom, intercom)