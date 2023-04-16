#pragma once
#include "event/Event.h"
#include <Arduino.h>

namespace TwinSystem{

    class Intercom {
    public:
        Intercom();

        void Initialize();

        void SendMessage(const char* message);
        void SendCommand(const char* message);
        void Update();

        void OnPingReceived();
        void OnPongReceived();
        void OnConnectionSuccess();
        void OnConnectionLost();
        void OnStopReceived() ;

        bool IsSomethingAtAngle(int angle);
        
        inline bool IsConnected(){return _connected;}



    private:
        Stream& _Stream;

        bool stopReceived = false;

        long _lastStream = 0;
        long _lastPing = 0;
        bool _connected = false;
    };


}