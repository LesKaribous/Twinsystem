#pragma once
#include "services/service.h"
#include <map>

class Intercom;
class Request;

using requestCallback_ptr = void (*)(const Request&);

class Request {
public:
    enum class Status{
        IDLE,
        SENT,
        TIMEOUT,
        OK,
        CLOSED,
        ERROR
    };
    
    Request(const String& payload,  long timeout = 5000, requestCallback_ptr callback = nullptr, callback_ptr timeout_callback = nullptr);

    void setTimeoutCallback(callback_ptr func);
    void setCallback(requestCallback_ptr func);

    void send(Intercom& channel);
    void close();
    void setStatus(Status status);
    
    void onTimeout();
    void onResponse(const String& response);
    
    bool isTimedOut() const;

    uint32_t ID() const;
    Status getStatus() const;
    String getPayload() const;
    const String& getMessage() const;
    const String& getResponse() const;
    unsigned long getTimeout() const;
    unsigned long getResponseTime() const;
    unsigned long getLastSent() const;
    
private:
    uint32_t _uid;
    String _payload;
    String _response;
    unsigned long _lastSent;
    unsigned long _responseTime;
    unsigned long _timeout;

    Status _status;
    requestCallback_ptr _callback;
    callback_ptr _timeoutCallback;
    static uint32_t _uidCounter;
};

class Intercom : public Service{
public:
    Intercom();

    void onAttach() override;
    void onUpdate() override;

    void enable() override;
    void disable() override;

    void sendMessage(const char* message);
    void sendMessage(const String& message);

    uint32_t sendRequest(const String& payload, long timeout = 200, requestCallback_ptr cbfunc = nullptr, callback_ptr func = nullptr);
    bool closeRequest(const uint32_t&);
    String getRequestResponse(const uint32_t&);

    void setConnectLostCallback(callback_ptr callback);
    void setConnectionSuccessCallback(callback_ptr callback);

    inline bool isConnected(){return _connected;}

private:

    void pingReceived(); 
    void connectionLost(); 
    void connectionSuccess(); 

    Stream& _stream;
    std::map<uint32_t,Request> _requests;

    callback_ptr onConnectionLost = nullptr;
    callback_ptr onConnectionSuccess = nullptr;

    unsigned long _lastStream = 0;
    unsigned long _lastPing = 0;
    bool _connected = false;

    void _sendRequest(Request& req);
    void _processIncomingData();
    void _processPendingRequests();

    SERVICE(Intercom)
};
EXTERN_DECLARATION(Intercom, intercom)