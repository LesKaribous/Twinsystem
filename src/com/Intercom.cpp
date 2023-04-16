#include "com/Intercom.h"
#include "debug/Console.h"

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
        Console::trace("Intercom") << ">" << std::string(message) << Console::endl;
    }

    void Intercom::SendCommand(const char* message) {
        _Stream.print(message);
        _Stream.write('\n');
        Console::info("Intercom") << ">" << std::string(message) << Console::endl;
    }

    void Intercom::Update() {
        _lastStream = millis();
        if (_Stream.available()) {
            char message[256];
            int messageSize = _Stream.readBytesUntil('\n', message, sizeof(message) - 1);
            message[messageSize] = '\0';

            Console::trace("Intercom") << "<" << message << Console::endl;
            if (strcmp(message, "ping") == 0) {
                OnPingReceived();
            }else if (strcmp(message, "pong") == 0) {
                OnPongReceived();
            }else if (strcmp(message, "202") == 0) {
                OnConnectionSuccess();
            }else if (strcmp(message, "STOP") == 0) {
                OnStopReceived();
            }
        }

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

    void Intercom::OnPongReceived() {
        if(!_connected) _connected = true;
    }

    void Intercom::OnConnectionLost() {
        Console::warn("Intercom") << "Connection lost." << Console::endl;
        _connected = false;
    }

    void Intercom::OnConnectionSuccess(){
        Console::info("Intercom") << "Connection successful." << Console::endl;
        _connected = true;
    }

    void Intercom::OnStopReceived() {
        stopReceived = true;
        Console::info("Intercom") << "Stop received from Lidar !" << Console ::endl;
    }

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
        
    }

}