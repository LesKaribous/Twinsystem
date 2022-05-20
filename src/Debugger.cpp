#include "Debugger.h"
#include "Twinsystem.h"

namespace Debugger{

    String lastCmd = "";

    char buffer[1024];
    EasyStringStream log(buffer, 1024);

    void init(){
        Serial.begin(115200);
    
        while (!Serial && millis() < 500) {
            //Waiting for serial
            //21-04-2022 - @Nadar - Diminution de 5000ms à 500ms
        }

        header();
        Serial.print("Preparing system...");
        
        delay(200);
        Serial.println("done.");

        Serial.print("Robot select : ");
        if( Settings::ROBOT == Settings::PRIMARY ) Serial.println("PRIMARY");
        else Serial.println("SECONDARY");
    }

    void header(){
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
        printBuffer();
        if(Serial.available()){
            String command = Serial.readStringUntil('\n');
            Serial.println("Received :" + command);
            parseCommand(command);
        }
    }
    
    void parseCommand(String command){
        if(command == "start"){
            lastCmd = "start";
            println("Starting program...");
        }else if(command == "restart"){
            //lastCmd = command;
            println("Not supported yet.");
        }else if(command == "reboot"){
            System::reboot();
        }else if(command == "probe"){
            lastCmd = command;
            println("Zeroing robot...");
            Strategy::recalage();
        }else if(command == "sleep"){
            lastCmd = command;
            println("Disable motors...");
            Controller::sleep();
        }else if(command == "count"){
            lastCmd = command;
            println("Asking for point count..");
            Intercom::askOpponent();
        }else if(command.startsWith("SetAbsolute")){
            Motion::SetAbsolute();
            println("Switched to absolute mode.");
        }else if(command.startsWith("SetRelative")){
            Motion::SetRelative();
            println("Switched to relative mode.");
        }else if(command.startsWith("goTurn(")){
            lastCmd = command;
            String argString = command.substring(command.indexOf("(") +1, command.indexOf(")"));
            String xStr = argString.substring(0, argString.indexOf(','));
            String yStr = argString.substring(argString.indexOf(',')+1, argString.lastIndexOf(',')-1);
            String rotStr  = argString.substring(argString.lastIndexOf(',')+1, argString.length());

            float x = float(xStr.toInt());
            float y = float(yStr.toInt());
            float a = float(rotStr.toInt());

            Motion::goTurn(x, y, a);

        }else if(command.startsWith("SetPosition(")){
            lastCmd = command;
            String argString = command.substring(command.indexOf("(") +1, command.indexOf(")"));
            String xStr = argString.substring(0, argString.indexOf(','));
            String yStr = argString.substring(argString.indexOf(',')+1, argString.lastIndexOf(','));
            String rotStr  = argString.substring(argString.lastIndexOf(',')+1, argString.length());

            float x = float(xStr.toInt());
            float y = float(yStr.toInt());
            float a = float(rotStr.toInt());
            Vec3 pos(x,y,a);
            Motion::SetPosition(pos);

        }else if(command.startsWith("go(")){
            lastCmd = command;
            String argString = command.substring(command.indexOf("(") +1, command.indexOf(")"));
            String xStr = argString.substring(0, argString.indexOf(','));
            String yStr  = argString.substring(argString.indexOf(',')+1, argString.length());

            float x = float(xStr.toInt());
            float y = float(yStr.toInt());

            Motion::go(x, y);

        }else if(command.startsWith("turn(")){
            lastCmd = command;
            String argString = command.substring(command.indexOf("(") +1, command.indexOf(")"));
            float a = float(argString.toInt());

            Motion::turn(a);

        }else if(command == "help" || command == "?"){
            log << "-------- Commands list ---------";
            log << "help : Arm the robot before start";
            log << "arm : Arm the robot before start";
            log << "start : Arm the robot before start";
        }else{
            log << "Unknown command : " << command;
            log << "Please run <help> to see available commands";
        }
    }

    String lastCommand(){
        String buf = lastCmd;
        lastCmd = "";
        return buf;
    }


    void println(String message){
        print(message);
        Serial.println();
    }

    void println(float message){
        print(message);
        Serial.println();
    }

    void println(int message){
        print(message);
        Serial.println();
    }

    void println(Vec3 message){
        print(message);
        Serial.println();
    }
    

    void print(String message){
        Serial.println(message);
    }

    void print(float data){
        Serial.println(data);
    }

    void print(int data){
        Serial.println(data);
    }

    void print(Vec3 v){
        Serial.print("Vec3: {x:");
        Serial.print(v.a);
        Serial.print(", y:");
        Serial.print(v.b);
        Serial.print(", w:");
        Serial.print(v.c);
        Serial.print("}");
    }

    void printBuffer(){
        if(log.getCursor() != 0)
            Serial.println(buffer);
        log.reset();
    }

} // namespace Debugger
