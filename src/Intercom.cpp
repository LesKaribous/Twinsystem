#include "Intercom.h"
#include "Debugger.h"
#include "IHM.h"

namespace Intercom{

    bool connected = false;
    unsigned long timeout = 0;
    unsigned long ping = 0;

    void init(){
        Serial4.begin(9600);
    }

    void checkSerial(){
        if(Serial4.available() > 0){
            String command = Serial4.readStringUntil('\n');
            parseRequest(command);
        }
        
        if(millis() - ping > 1000){

            Serial4.println("ping");
            ping = millis();

        }else if(connected && millis() - timeout > 5000){
            connected = false;
            Debugger::log("Lidar connection timed out");
        }
    }
    
    void parseRequest(String command){
        if(command.startsWith("ping")){

            Serial4.println("pong");
            
        }else if(command.startsWith("pong")){
            connected = true;
            timeout = millis();
        }else if(command.startsWith("count")){
            String argString = command.substring(command.indexOf("(") +1, command.indexOf(")"));
            int count = float(argString.toInt());

            Debugger::log(count);
            
            //Lidar::setFOV(angle, width);
        }
    }

    void setFOV(float angle, float bandWidth, float maxDist){
        Serial4.print("setFov(");
        Serial4.print(int(angle*100));
        Serial4.print(',');
        Serial4.print(int(bandWidth*100));
        Serial4.print(',');
        Serial4.print(int(maxDist*100));
        Serial4.println(")");
    }

    void askOpponent(){
        Serial4.print("getPointCount(");
        Serial4.println(")");
        
        setFOV(90, 10, 1000);
    }


} // namespace Intercom
