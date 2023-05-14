#pragma once
#include "core/lib.h"

class Intercom;

class Request {
public:
    enum class Status{
        IDLE,
        SENT,
        TIMEOUT,
        OK,
        ERROR
    };

    typedef void (*Callback)(String response);
    
    Request();
    Request(const String& payload, Callback callback = nullptr, long timeout = 5000);

    void send(Intercom& channel);
    void setStatus(Status status);
    
    void onTimeout();
    void onResponse(const String& response);
    
    bool isTimedOut() const;

    uint32_t ID() const;
    Status getStatus() const;
    String getPayload() const;
    Callback getCallback() const;
    const String& getMessage() const;
    unsigned long getTimeout() const;
    unsigned long getLastSent() const;
    
private:
    Status _status = Status::IDLE;

    uint32_t _uid;
    static uint32_t _uidCounter;

    String _payload;

    unsigned long _timeout = 10 * 1000;
    unsigned long _lastSent = 0;
    
    Callback _callback;
};

class Intercom {
public:
    Intercom();

    void initialize();

    void sendMessage(const char* message);
    void sendMessage(const String& message);

    void sendRequest(Request& req);
    void sendRequest(const String& payload, Request::Callback callback = nullptr, long timeout = 5000);

    void update();

    void onPingReceived(); 
    void onConnectionSuccess();
    void onConnectionLost();

    inline bool isConnected(){return _connected;}

private:
    Stream& _stream;

    std::map<uint32_t,Request> _pendingRequest;

    bool stopReceived = false;

    unsigned long _lastStream = 0;
    unsigned long _lastPing = 0;
    bool _connected = false;

    void _sendRequest(Request& req);
    void _processIncomingData();
    void _processPendingRequests();
};

