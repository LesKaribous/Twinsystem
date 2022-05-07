#include "Debugger.h"
#include "Twinsystem.h"

namespace Debugger{

    String lastCmd = "";

    void init(){
        Serial.begin(115200);
    
        while (!Serial && millis() < 500) {
            //Waiting for serial
            //21-04-2022 - @Nadar - Diminution de 5000ms à 500ms
        }

        printHeader();
        Serial.print("Preparing system...");
        
        delay(200);
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
            String command = Serial.readStringUntil('\n');
            Serial.println("Received :" + command);
            parseCommand(command);
        }
    }
    
    void parseCommand(String command){
        if(command == "start"){
            lastCmd = "start";
            log("Starting program...");
        }else if(command == "restart"){
            //lastCmd = command;
            log("Not supported yet.");
        }else if(command == "reboot"){
            System::reboot();
        }else if(command == "probe"){
            lastCmd = command;
            log("Zeroing robot...");
            Strategy::recalage();
        }else if(command == "help" || command == "?"){
            log("-------- Commands list ---------");
            log("help : Arm the robot before start");
            log("arm : Arm the robot before start");
            log("start : Arm the robot before start");
        }else{
            log("Unknown command : " + command);
            log("Please run <help> to see available commands");
        }
    }

    String lastCommand(){
        String buf = lastCmd;
        lastCmd = "";
        return buf;
    }


    void log(String message){
        Serial.println(message);
    }

    void log(float data){
        Serial.println(data);
    }

    void log(Vec3 v){
        Serial.print("Vec3: {x:");
        Serial.print(v.a);
        Serial.print(", y:");
        Serial.print(v.b);
        Serial.print(", w:");
        Serial.print(v.c);
        Serial.println("}");
    }

} // namespace Debugger
