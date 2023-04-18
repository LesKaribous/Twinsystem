#pragma once
#include "event/Event.h"

#include <Arduino.h>
#include <map>

namespace TwinSystem{

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

        void Send(Intercom& channel);
        void SetStatus(Status status);
        
        void OnTimeout();
        void OnResponse(const String& response);
        
        bool IsTimedOut() const;

        uint32_t ID() const;
        Status GetStatus() const;
        String GetPayload() const;
        Callback GetCallback() const;
        const String& GetMessage() const;
        unsigned long GetTimeout() const;
        unsigned long GetLastSent() const;
        
    private:
        Status _status;

        uint32_t _uid;
        static uint32_t _uidCounter;

        String _payload;

        unsigned long _timeout;
        unsigned long _lastSent;
        
        Callback _callback;
    };

    class Intercom {
    public:
        Intercom();

        void Initialize();

        void SendMessage(const char* message);
        void SendMessage(const String& message);

        void SendRequest(Request& req);
        void SendRequest(const String& payload, Request::Callback callback = nullptr, long timeout = 5000);

        void Update();

        void OnPingReceived(); 
        void OnConnectionSuccess();
        void OnConnectionLost();

        inline bool IsConnected(){return _connected;}

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

}