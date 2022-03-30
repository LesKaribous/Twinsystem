#include "Debugger.h"

namespace Debugger{
    void init(){
        Serial.begin(115200);
        delay(500);

        Serial.print("Twinsystem... compiled  ");
        Serial.print(__DATE__);
        Serial.print(" at ");
        Serial.println(__TIME__);

        Serial.print("Preparing system...");

        delay(1000);
        Serial.println("done.");
    }

    void checkSerial(){
        if(Serial.available()){
            String command = Serial.readStringUntil('(');
            Serial.println("Received :" + command);
        }
    }
    

    void execCommand(String command){

    }

    void log(String message){
        Serial.println(message);
    }

    void log(int data){
        Serial.println(data);
    }

    void log(float data){
        Serial.println(data);
    }

} // namespace Debugger
