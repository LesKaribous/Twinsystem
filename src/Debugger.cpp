#include "Debugger.h"
#include "Twinsystem.h"

namespace Debugger{

    String lastCmd = "";
    bool enabled = true;
    Level debugLevel = WARN;

    void init(Level l){
        if(enabled){
            Serial.begin(9600);
            debugLevel = l;

            if(Serial.available() <= 0) {}

            header();
            Serial.print("Preparing system...");
            
            Serial.print("Robot select : ");
            if( Settings::ROBOT == Settings::PRIMARY ) Serial.println("PRIMARY");
            else Serial.println("SECONDARY");

            delay(200);
            Serial.println("done.");
        }


    }

    Level level(){
        return debugLevel;
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
        }else if(command == "focus"){
            Intercom::focus();
        }else if(command == "sleep"){
            lastCmd = command;
            println("Disabled motors.");
            Controller::sleep();
        }else if(command.startsWith("PAUSE")){
            Motion::pause();
        }else if(command.startsWith("RESUME")){
            Motion::resume();
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

            float x = xStr.toFloat();
            float y = yStr.toFloat();
            float a = rotStr.toFloat();

            Motion::goTurn(x, y, a);

        }else if(command.startsWith("SetPosition(")){
            lastCmd = command;
            String argString = command.substring(command.indexOf("(") +1, command.indexOf(")"));
            String xStr = argString.substring(0, argString.indexOf(','));
            String yStr = argString.substring(argString.indexOf(',')+1, argString.lastIndexOf(','));
            String rotStr  = argString.substring(argString.lastIndexOf(',')+1, argString.length());

            float x = xStr.toFloat();
            float y = yStr.toFloat();
            float a = rotStr.toFloat();
            Vec3 pos(x,y,a);
            Motion::SetPosition(pos);

        }else if(command.startsWith("setFOV(")){
            lastCmd = command;
            String argString = command.substring(command.indexOf("(") +1, command.indexOf(")"));
            String angleStr = argString.substring(0, argString.indexOf(','));
            String distStr  = argString.substring(argString.indexOf(',')+1, argString.length());

            float angle = float(angleStr.toInt());
            float dist = float(distStr.toInt());
            Intercom::setFOV(angle);
        }else if(command.startsWith("heading(")){
            lastCmd = command;
            String argString = command.substring(command.indexOf("(") +1, command.indexOf(")"));
            String xStr = argString.substring(0, argString.indexOf(','));
            String yStr  = argString.substring(argString.indexOf(',')+1, argString.length());

            float x = xStr.toFloat();
            float y = yStr.toFloat();
            Vec2 v(x,y);
            Debugger::println(float(v.heading()*RAD_TO_DEG));
        }else if(command.startsWith("lookAt(")){
            lastCmd = command;
            String argString = command.substring(command.indexOf("(") +1, command.indexOf(")"));
            String angleStr = argString.substring(0, argString.indexOf(','));
            String distStr  = argString.substring(argString.indexOf(',')+1, argString.length());

            float angle = float(angleStr.toFloat());
            float dist = float(distStr.toFloat());
            Intercom::lookAt(angle, dist);
        }else if(command.startsWith("go(")){
            lastCmd = command;
            String argString = command.substring(command.indexOf("(") +1, command.indexOf(")"));
            String xStr = argString.substring(0, argString.indexOf(','));
            String yStr  = argString.substring(argString.indexOf(',')+1, argString.length());

            float x = float(xStr.toFloat());
            float y = float(yStr.toFloat());

            Motion::go(x, y);

        }else if(command.startsWith("goPolar(")){
            lastCmd = command;
            String argString = command.substring(command.indexOf("(") +1, command.indexOf(")"));
            String xStr = argString.substring(0, argString.indexOf(','));
            String yStr  = argString.substring(argString.indexOf(',')+1, argString.length());

            float x = float(xStr.toFloat());
            float y = float(yStr.toFloat());

            Motion::goPolar(x, y);

        }else if(command.startsWith("turn(")){
            lastCmd = command;
            String argString = command.substring(command.indexOf("(") +1, command.indexOf(")"));
            float a = float(argString.toInt());

            Motion::turn(a);
        }else if(command.startsWith("align(")){
            lastCmd = command;
            String argString = command.substring(command.indexOf("(") +1, command.indexOf(")"));
            String xStr = argString.substring(0, argString.indexOf(','));
            //String yStr = argString.substring(argString.indexOf(',')+1, argString.lastIndexOf(','));
            String rotStr  = argString.substring(argString.lastIndexOf(',')+1, argString.length());

            float x = float(xStr.toFloat());
            //float y = float(yStr.toFloat());
            float a = float(rotStr.toFloat());
            //Vec2 pos(x,y);
            //Motion::align(pos, a);
            Motion::align(x, a);
        }else if(command.startsWith("team(")){
            lastCmd = command;
            String argString = command.substring(command.indexOf("(") +1, command.indexOf(")"));
            if(argString == "YELLOW")Settings::setTeam(Settings::Team::YELLOW);
            else if(argString == "PURPLE") Settings::setTeam(Settings::Team::PURPLE);
            
            IHM::setTeam(Settings::TEAM);

        }else if(command == "help" || command == "?"){
            println("-------- Commands list ---------");
            println("help : show help message");
            println("start : Start the match");
            println("reboot : reboot the robot");
            println("probe : lauch the probe sequence");
            println("sleep : Disable steppers");
            println("SetAbsolute : Set robot to absolute mode");
            println("SetRelative : Set robot to relative mode");
            println("goTurn(int X, int Y, int angle) : Go to <X Y> position and turn by <angle>");
            println("go(int X, int Y) : Go to <X Y> position");
            println("turn(int X) : Turn by <angle>");
            println("SetPosition(int X, int Y, int angle) : Set Robot absolute position");
            println("setFOV(int angleRange) : Arm the robot before start : Set lidar range");
            println("lookAt(int angle, int distance) : Arm the robot before start : Lidar look at position");
        }else{
            log("Unknown command : ", command, INFO);
            println("Please run <help> to see available commands");
        }

        if(lastCmd.startsWith("PAUSE")){
            if(Controller::isRunning()){
                Controller::stop();
                lastCmd = command;
            }else{

            }
        }else if(lastCmd.startsWith("RESUME")){
            if(!Controller::isRunning()){
                Controller::resume();
                lastCmd = command;
            }else{
                
            }
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

    void log(int data, Level level){
        if(level < debugLevel) return;
        println(data);
    }

    void log(char data, Level level){
        if(level < debugLevel) return;
        println(data);
    }

    void log(float data, Level level){
        if(level < debugLevel) return;
        println(data);
    }

    void log(String data, Level level){
        if(level < debugLevel) return;
        println(data);
    }

    void log(Vec3 data, Level level){
        if(level < debugLevel) return;
        println(data);
    }

    void log(String prefix,   int data, String suffix, Level level){
        if(level < debugLevel) return;
        Serial.print(prefix);
        Serial.print(data);
        Serial.println(suffix);
    }

    void log(String prefix, float data, String suffix, Level level){
        if(level < debugLevel) return;
        Serial.print(prefix);
        Serial.print(data);
        Serial.println(suffix);
    }

    void log(String prefix,  bool data, String suffix, Level level){
        if(level < debugLevel) return;
        Serial.print(prefix);
        Serial.print(data);
        Serial.println(suffix);
    }

    void log(String prefix,  String data, String suffix, Level level){
        if(level < debugLevel) return;
        Serial.print(prefix);
        Serial.print(data);
        Serial.println(suffix);
    }

    void log(String prefix,  Vec3 data, String suffix, Level level){
        if(level < debugLevel) return;
        Serial.print(prefix);
        print(data);
        Serial.println(suffix);
    }


    void log(String prefix, int data, Level level){
        if(level < debugLevel) return;
        Serial.print(prefix);
        Serial.println(data);
    }

    void log(String prefix, float data, Level level){
        if(level < debugLevel) return;
        Serial.print(prefix);
        Serial.println(data);
    }

    void log(String prefix, bool data, Level level){
        if(level < debugLevel) return;
        Serial.print(prefix);
        Serial.println(data);
    }

    void log(String prefix,  Vec3 data, Level level){
        if(level < debugLevel) return;
        Serial.print(prefix);
        println(data);
    }

    void log(String prefix,  String data, Level level){
        if(level < debugLevel) return;
        Serial.print(prefix);
        Serial.println(data);
    }



    void logArray(String prefix, int array[], size_t size, char separator, String suffix, Level level){
        if(level < debugLevel) return;
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

    void logArrayN(String prefix, int element, String interFix, int array[], size_t size, char separator, String suffix, Level level){
        if(level < debugLevel) return;
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
