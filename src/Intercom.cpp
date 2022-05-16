#include "Intercom.h"
#include "Debugger.h"
#include "IHM.h"

namespace Intercom{

    bool connected = false;
    unsigned long timeout = 0;

    void init(){
        Serial4.begin(9600);
    }

    void checkSerial(){
        if(Serial4.available() > 0){
            String command = Serial4.readStringUntil('\n');
            parseRequest(command);
            timeout = millis();
            connected = true;
        }
        if((connected && millis() - timeout > 5000) || (!connected && millis() - timeout > 1000)){
            connected = false;
            timeout = millis();
            Serial4.println("ping");
            Debugger::log("ping");
        }
    }
    
    void parseRequest(String command){
        Debugger::log(command);
        if(command == "ping"){
            Serial4.println("pong");
        }
    }

    void setFOV(float angle, float width){
        Serial4.print("setFov(");
        Serial4.print(int(angle*100));
        Serial4.print(int(width*100));
        Serial4.println(")");
    }


} // namespace Intercom
