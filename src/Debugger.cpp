#include "Debugger.h"
#include "Settings.h"

namespace Debugger{
    void init(){
        Serial.begin(115200);
    
        while (!Serial && millis() < 5000) {
            //Waiting for serial
        }
        printHeader();
        Serial.print("Preparing system...");
        
        delay(1000);
        Serial.println("done.");

        Serial.print("Robot select : ");
        if( Settings::ROBOT == Settings::PRIMARY ) Serial.println("PRIMARY");
        else Serial.println("SECONDARY");
    }

    void printHeader(){
        Serial.println("  _______       _                     _"                        );
        Serial.println(" |__   __|     (_)                   | |"                       );
        Serial.println("    | |_      ___ _ __  ___ _   _ ___| |_ ___ _ __ ___  "       );
        Serial.println("    | \\ \\ /\\ / / | '_ \\/ __| | | / __| __/ _ \\ '_ ` _ \\"  );
        Serial.println("    | |\\ V  V /| | | | \\__ \\ |_| \\__ \\ ||  __/ | | | | |"  );
        Serial.println("    |_| \\_/\\_/ |_|_| |_|___/\\__, |___/\\__\\___|_| |_| |_|"  );
        Serial.println("                             __/ |"                             );
        Serial.println("                            |___/"                              );
        Serial.println();
        Serial.println("Author  : Nadarbreicq, JulesTopart ");
        Serial.println();
        Serial.print("Twinsystem... compiled  ");
        Serial.print(__DATE__);
        Serial.print(" at ");
        Serial.println(__TIME__);

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
