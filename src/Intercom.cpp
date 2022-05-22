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

    void init(){
        Serial4.begin(9600);
    }

    void reboot(){
        Serial2.clear();
        Serial2.begin(9600);
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
        }else if(command.startsWith("detection")){
            String argString = command.substring(command.indexOf("(") +1, command.indexOf(")"));
            count = float(argString.toInt());         
            Debugger::log( "Detected ", count, "points at targeted position");
        }
    }

    void setFOV(float angle, float dist){
        Serial4.print("setFov(");
        Serial4.print(int(angle*100));
        Serial4.print(',');
        Serial4.print(int(dist*100));
        Serial4.println(")");
    }

    void lookAt(float angle, float dist){
        Serial4.print("lookAt(");
        Serial4.print(int(angle*100));
        Serial4.print(',');
        Serial4.print(int(dist*100));
        Serial4.println(")");
    }
    
    
    void focus(){
        Vec3 t3 = Motion::GetTarget();
        Vec2 t2(t3.a, t3.b);
        //Vec2 t2(1, 1);

        Debugger::log("Heading : ", t2.heading(), WARN);

        float angleRange = (Settings::RADIUS*2)/(t2.mag() + Settings::RADIUS);
        float distRange = Settings::RADIUS*2.5 ;

        setFOV(30*RAD_TO_DEG, 200);


        lookAt(t2.heading()*RAD_TO_DEG, t2.mag() + Settings::RADIUS);
    }

    bool collision(){
        return (count > 80);
    }
}
