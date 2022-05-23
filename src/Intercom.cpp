#include "Intercom.h"
#include "Debugger.h"
#include "IHM.h"
#include "Motion.h"
#include "Settings.h"

namespace Intercom{

    bool connected = false;
    unsigned long timeout = 0;
    unsigned long ping = 0;
    unsigned long countTimer = 0;

    int count = 0;
    int threshold = 8;

    void init(){
        Serial4.begin(9600);
    }

    void reboot(){
        Serial4.clear();
        Serial4.begin(9600);
        Serial4.println("reboot()");
        connected = false;
    }

    void checkSerial(){
        if(Serial4.available() > 0){
            String command = Serial4.readStringUntil('\n');
            parseRequest(command);
        }
        
        if(millis() - ping > 2000){

            Serial4.println("ping");
            Debugger::log("ping at ", int(ping), "ms");
            ping = millis();

            if(connected && millis() - timeout > 5000){
                connected = false;
                Debugger::println("Lidar connection timed out");
            }
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
            count = float(argString.toInt());         
            Debugger::log( "Detected ", count, "points at targeted position");
        }
    }

    void setFOV(float angle){
        Serial4.print("setFov(");
        Serial4.print(int(angle*100));
        Serial4.println(")");
    }

    void lookAt(float angle, float dist){
        Serial4.print("lookAt(");
        Serial4.print(int(angle*100));
        Serial4.print(',');
        Serial4.print(int(dist*100));
        Serial4.println(")");
    }

    void askCount(){
        Serial4.print("getPointCount()");
    }
    
    
    void focus(){
        Vec3 t = Motion::GetAbsTarget();
        Vec3 p = Motion::GetPosition();
        t.sub(p);

        Vec2 t2d = t;

        Debugger::log("Heading : ", t2d.heading(), VERBOSE);

        float angleRange = (Settings::RADIUS*2)/(t2d.mag() + Settings::RADIUS);
        float distRange = Settings::RADIUS*3 ;

        setFOV(angleRange*RAD_TO_DEG);
        lookAt(t2d.heading()*RAD_TO_DEG, t2d.mag() + Settings::RADIUS);
    }


    void setThreshold(float t){
        threshold = t;
    }

    bool collision(){
        askCount();
        return (count >= threshold);
    }
}
