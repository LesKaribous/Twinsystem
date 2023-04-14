#include "com/Intercom.h"

#define LIDAR_SERIAL Serial1

namespace TwinSystem{
    Intercom::Intercom() : _Stream(LIDAR_SERIAL) {}

    void Intercom::Initialize(){
        LIDAR_SERIAL.begin(115200);
        SendMessage("ping");
    }

    void Intercom::SendMessage(const char* message) {
        _Stream.print(message);
        _Stream.write('\n');
    }

    void Intercom::Update() {
        if (_Stream.available()) {
            char message[256];
            int messageSize = _Stream.readBytesUntil('\n', message, sizeof(message) - 1);
            message[messageSize] = '\0';

            if (strcmp(message, "ping") == 0) {
                OnPingReceived();
            }else if (strcmp(message, "pong") == 0) {
                OnPongReceived();
            }else if (strcmp(message, "202") == 0) {
                OnConnectionSuccess();
            }
            _lastStream = millis();
        }

        if(!_connected && millis() - _lastPing > 500){
            SendMessage("ping");
            _lastPing = millis();
        }
        if(_connected && millis() - _lastStream > 1000){
            OnConnectionLost();
        }
    }

    void Intercom::OnPingReceived() {
        SendMessage("pong");
    }

    void Intercom::OnPongReceived() {
        SendMessage("202");
    }

    void Intercom::OnConnectionLost() {
        _connected = false;
    }

    void Intercom::OnConnectionSuccess() {
        _connected = true;
    }

}