#pragma once
#include "system/core/lib.h"

class Intercom;

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
    
    Request(const String& payload, long timeout = 5000);

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
    unsigned long _timeout;
    unsigned long _responseTime;
    Status _status;

    static uint32_t _uidCounter;
};

class Intercom {
public:
    Intercom();

    void initialize();

    void sendMessage(const char* message);
    void sendMessage(const String& message);

    uint32_t sendRequest(const String& payload, long timeout = 200);

    void update();
    bool closeRequest(const uint32_t&);
    String getRequestResponse(const uint32_t&);

    void onPingReceived(); 
    void onConnectionSuccess();
    void onConnectionLost();

    inline bool isConnected(){return _connected;}

private:
    Stream& _stream;
    std::map<uint32_t,Request> _requests;

    unsigned long _lastStream = 0;
    unsigned long _lastPing = 0;
    bool _connected = false;

    void _sendRequest(Request& req);
    void _processIncomingData();
    void _processPendingRequests();
};

