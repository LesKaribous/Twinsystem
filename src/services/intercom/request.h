
#pragma once
#include "intercom.h"

class Request;

using messageCallback_ptr = void (*)(const String&);
using requestCallback_ptr = void (*)(Request&);

class Intercom;

class Request {
public:
    enum class Status{
        IDLE,
        SENT,
        TIMEOUT,
        OK,
        CLOSED,
        TO_ANSWER,
        ERROR
    };
    
    Request(int id, const String& content); //When you receive a request
    Request(const String& payload,  long timeout = 1000, requestCallback_ptr callback = nullptr, callback_ptr timeout_callback = nullptr); //When you send a request

    void setTimeoutCallback(callback_ptr func);
    void setCallback(requestCallback_ptr func);

    void send();
    void reply(const String& answer);
    void close();
    void setStatus(Status status);
    
    void onTimeout();
    void onResponse(const String& response);
    
    bool isTimedOut() const;

    int ID() const;
    Status getStatus() const;
    String getPayload() const;

    const String& getContent() const;
    const String& getResponse() const;
    
    unsigned long getTimeout() const;
    unsigned long getResponseTime() const;
    unsigned long getLastSent() const;
    
private:
    int _uid;
    String _prefix;
    String _crc;
    String _content;
    String _response;
    unsigned long _lastSent;
    unsigned long _responseTime;
    unsigned long _timeout;
    static int _uidCounter;

    Status _status;
    requestCallback_ptr _callback;
    callback_ptr _timeoutCallback;
};