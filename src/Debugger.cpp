#include "Debugger.h"
#include "Twinsystem.h"

namespace Debugger{

    String lastCmd = "";

    void init(){
        Serial.begin(9600);
    
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
        }else if(command == "rebootLidar"){
            println("Trying to reboot Lidar...");
            Intercom::reboot();
            long unsigned int time = millis();
            while (millis() - time < 5000){
                Intercom::checkSerial();
            }
            
            println(Intercom::connected ? "Lidar connected" : "Lidar is not responding...");
        }else if(command == "probe"){
            lastCmd = command;
            println("Zeroing robot...");
            Strategy::recalage();
        }else if(command == "sleep"){
            lastCmd = command;
            println("Disabled motors.");
            Controller::sleep();
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

        }else if(command.startsWith("setFOV(")){
            lastCmd = command;
            String argString = command.substring(command.indexOf("(") +1, command.indexOf(")"));
            String angleStr = argString.substring(0, argString.indexOf(','));
            String distStr  = argString.substring(argString.indexOf(',')+1, argString.length());

            float angle = float(angleStr.toInt());
            float dist = float(distStr.toInt());
            Intercom::setFOV(angle, dist);
        }else if(command.startsWith("lookAt(")){
            lastCmd = command;
            String argString = command.substring(command.indexOf("(") +1, command.indexOf(")"));
            String angleStr = argString.substring(0, argString.indexOf(','));
            String distStr  = argString.substring(argString.indexOf(',')+1, argString.length());

            float angle = float(angleStr.toInt());
            float dist = float(distStr.toInt());
            Intercom::lookAt(angle, dist);
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
            log("-------- Commands list ---------");
            log("help : show help message");
            log("start : Start the match");
            log("reboot : reboot the robot");
            log("probe : lauch the probe sequence");
            log("sleep : Disable steppers");
            log("SetAbsolute : Set robot to absolute mode");
            log("SetRelative : Set robot to relative mode");
            log("goTurn(int X, int Y, int angle) : Go to <X Y> position and turn by <angle>");
            log("go(int X, int Y) : Go to <X Y> position");
            log("turn(int X) : Turn by <angle>");
            log("SetPosition(int X, int Y, int angle) : Set Robot absolute position");
            log("setFOV(int angleRange, int distRange) : Arm the robot before start : Set lidar range");
            log("lookAt(int angle, int distance) : Arm the robot before start : Lidar look at position");
        }else{
            log("Unknown command : ", command);
            log("Please run <help> to see available commands");
        }
    }

    String lastCommand(){
        String buf = lastCmd;
        lastCmd = "";
        return buf;
    }


    void println(String message){
        Serial.println(message);
    }

    void println(char c){
        Serial.println(c);
    }

    void println(float data){
        Serial.println(data);
    }

    void println(int data){
        Serial.println(data);
    }

    void println(bool data){
        Serial.println(data);
    }

    void println(Vec3 v){
        print(v);
        Serial.println();
    }

    void print(String message){
        Serial.print(message);
    }

    void print(char c){
        Serial.print(c);
    }

    void print(float data){
        Serial.print(data);
    }

    void print(int data){
        Serial.print(data);
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


    void log(int data){
        println(data);
    }

    void log(char data){
        println(data);
    }

    void log(float data){
        println(data);
    }

    void log(String data){
        println(data);
    }

    void log(Vec3 data){
        println(data);
    }


    void log(String prefix, String data, String suffix){
        Serial.print(prefix);
        Serial.print(data);
        Serial.println(suffix);
    }

    void log(String prefix,   int data, String suffix){
        Serial.print(prefix);
        Serial.print(data);
        Serial.println(suffix);
    }

    void log(String prefix, float data, String suffix){
        Serial.print(prefix);
        Serial.print(data);
        Serial.println(suffix);
    }

    void logArray(String prefix, int array[], size_t size, char separator, String suffix = ""){
        if(size > 0){
            print(prefix);
            for (size_t i = 0; i < size-1; i++){
                print(array[i]);
                print(separator);
            }
            print(array[size-1]);
            println(suffix);
        }
    }

    void logArrayN(String prefix, int element, String interFix, int array[], size_t size, char separator = ',', String suffix = ""){
        if(size > 0){
            print(prefix);
            print(element);
            print(interFix);
            for (size_t i = 0; i < size-1; i++){
                print(array[i]);
                print(separator);
            }
            print(array[size-1]);
            println(suffix);
        }else println("Invalid array printed !");
    }
} // namespace Debugger
