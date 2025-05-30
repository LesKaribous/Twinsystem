#include "commands.h"
#include "robot.h"
#include "routines.h"
#include "strategy.h"
#include "poi.h"

void registerCommands() {
    CommandHandler::registerCommand("start", "Start Match", command_start);
    CommandHandler::registerCommand("stop", "Stop Robot", command_stop);
    CommandHandler::registerCommand("reboot", "Reboot Robot", command_reboot);

    CommandHandler::registerCommand("enable(service)", "Enable a specific service", command_enable);
    CommandHandler::registerCommand("disable(service)", "Disable a specific service", command_disable);
    CommandHandler::registerCommand("status(service)", "Display single status", command_status);
    CommandHandler::registerCommand("wait(duration)", "Wait a bit for duration", command_wait);
    CommandHandler::registerCommand("lidarMode(mode)", "Change neopixel display mode on lidar", command_lidarMode);
    CommandHandler::registerCommand("go(x,y)", "Move to a specific position", command_go);
    CommandHandler::registerCommand("goPolar(angle,dist)", "Move to a relative polar position", command_goPolar);
    CommandHandler::registerCommand("goAlign(x,y, rc, tc)", "Move to a x y position with orientation", command_goAlign);
    CommandHandler::registerCommand("gPolarAlign(angle,dist, rc, tc)", "Move to relative polar position with orientation", command_goAlign);
    CommandHandler::registerCommand("move(x,y,angle)", "Move to a specific position", command_move);
    CommandHandler::registerCommand("turn(angle)", "Turn to a specific angle", command_turn);
    CommandHandler::registerCommand("rawTurn(angle)", "Turn to a specific angle without optimization", command_rawTurn);
    CommandHandler::registerCommand("pause", "Pause motion", command_pause);
    CommandHandler::registerCommand("resume", "Resume motion", command_resume);
    CommandHandler::registerCommand("cancel", "Cancel motion", command_cancel);
    CommandHandler::registerCommand("probe(tableCompass, side)", "Probe border", command_probe);
    CommandHandler::registerCommand("sleep", "Put motion to sleep", command_sleep);
    CommandHandler::registerCommand("wake", "Wake up motion", command_wake);
    CommandHandler::registerCommand("align(side,angle)", "Align to a specific side and angle", command_align);
    CommandHandler::registerCommand("setAbsolute", "Set motion to absolute mode", command_setAbsolute);
    CommandHandler::registerCommand("setRelative", "Set motion to relative mode", command_setRelative);
    CommandHandler::registerCommand("setAbsPosition(x,y,angle)", "Set absolute position", command_setAbsPosition);
    CommandHandler::registerCommand("setAbsolutePosition(x,y,angle)", "Set absolute position", command_setAbsPosition);
    CommandHandler::registerCommand("resetCompass", "Reset compass and set to 0", command_resetCompass);
    CommandHandler::registerCommand("elevator(side, pose)", "Raise elevator to desired pose", command_elevator);
    CommandHandler::registerCommand("moveElevator(side,angle)", "Raise elevator to desired angle", command_move_elevator);
    CommandHandler::registerCommand("raise(side)", "Raise elevator", command_raise);
    CommandHandler::registerCommand("lower(side)", "Lower elevator", command_lower);
    CommandHandler::registerCommand("grab(side)", "Grab object using actuator", command_grab);
    CommandHandler::registerCommand("drop(side)", "Drop object using actuator", command_drop);
    CommandHandler::registerCommand("grabPlank(side)", "Grab Plank using actuator", command_grabPlank);
    CommandHandler::registerCommand("dropPlank(side)", "Drop Plank using actuator", command_dropPlank);
    CommandHandler::registerCommand("storePlank(side)", "Store Plank using actuator", command_storePlank);
    CommandHandler::registerCommand("pump(side)", "enable pump", command_pump);
    CommandHandler::registerCommand("ev(side)", " disable pump", command_ev);
    CommandHandler::registerCommand("initPump", " Init Pump", command_initPump);
    CommandHandler::registerCommand("cruise", " Enable cruise mode", command_cruise);
    CommandHandler::registerCommand("feed(feedrate)", " Set Move feedrate", command_feed);
    CommandHandler::registerCommand("music", " Play a sound", command_music);
    CommandHandler::registerCommand("radar", " Toogle radar view on neopixel", command_radar);
    CommandHandler::registerCommand("test", " Dummy Test function ", command_test);

    //CommandHandler::registerCommand("open(side)", "Open actuator on a specific side", command_open);
    //CommandHandler::registerCommand("close(side)", "Close actuator on a specific side", command_close);
    CommandHandler::registerCommand("recalage()", "Execute recalage routine", command_recalage);
    CommandHandler::registerCommand("print(value)", "Print the result of an expression in the terminal", command_print);
    CommandHandler::registerCommand("help", "Display help", command_help);

    
}

void command_enable(const args_t& args){
    if(args.size() != 1) return;
    ServiceID serviceID = Service::toID(args[0]);
    if(serviceID != ID_NOT_A_SERVICE){
        os.enable(serviceID);
        Console::info("Interpreter") << args[0] <<  " enabled" << Console::endl;
    }else  Console::error("Interpreter") << "unknown service" << Console::endl;
}

void command_disable(const args_t& args){
    if(args.size() != 1) return;
    ServiceID serviceID = Service::toID(args[0]);
    if(serviceID != ID_NOT_A_SERVICE){
        os.disable(serviceID);
        Console::info("Interpreter") << args[0] <<  " disabled" << Console::endl;
    }else  Console::error("Interpreter") << "unknown service" << Console::endl;
}

void command_status(const args_t& args){
    if(args.size() != 1){
        for ( int id = 0; id != ServiceID::ID_NOT_A_SERVICE; id++ ){
           ServiceID sID = static_cast<ServiceID>(id);
           Console::info("Interpreter") << Service::toString(sID) <<  " : " << (OS::instance().statusService(sID) ? "ON" : "OFF") << Console::endl;
        }
    }else{
        ServiceID serviceID = Service::toID(args[0]);
        Console::info("Interpreter") << args[0] <<  " : " << (OS::instance().statusService(serviceID) ? "ON" : "OFF") << Console::endl;
    }
} 


void command_debug(const args_t& args){
    if(args.size() != 1){
        for ( int id = 0; id != ServiceID::ID_NOT_A_SERVICE; id++ ){
           ServiceID sID = static_cast<ServiceID>(id);
           os.toggleDebug(sID);
           Console::info("Interpreter") << Service::toString(sID) <<  " debug : " << (OS::instance().debug(sID) ? "ON" : "OFF") << Console::endl;
        }
    }else{
        ServiceID serviceID = Service::toID(args[0]);
        os.toggleDebug(serviceID);
        Console::info("Interpreter") << args[0] <<  " debug : "  << (OS::instance().debug(serviceID) ? "ON" : "OFF") << Console::endl;
    }
}


//Lidar

void command_lidarMode(const args_t& args){
    if(args.size() != 1) return;
    if(args[0].equalsIgnoreCase("radar"))lidar.showRadarLED();
    else if(args[0].equalsIgnoreCase("intercom"))lidar.showStatusLED();
}

void command_wait(const args_t &args){
    if(args.size() != 1) return;
    float duration = args[0].toFloat();
    os.wait(duration);
}

void command_start(const args_t &args){
    robotArmed();
    os.start();
}

void command_stop(const args_t &args){
    os.stop();
}

void command_reboot(const args_t &args){
    os.reboot();
}

void command_cruise(const args_t& args){
    motion.enableCruiseMode();
}

void command_feed(const args_t &args){
    if(args.size() != 1) return;
    float feedrate = args[0].toFloat();
    std::clamp(feedrate, 0.05f, 1.0f);
    motion.setFeedrate(feedrate);
    //motion.feed(1000);
}

void command_music(const args_t &args){
    ihm.playTone(659, 167); // E5, 8th note
    ihm.playTone(587, 167); // D5
    ihm.playTone(370, 333); // F#4, quarter note
    ihm.playTone(415, 333); // G#4

    ihm.playTone(554, 167); // C#5
    ihm.playTone(494, 167); // B4
    ihm.playTone(294, 333); // D4
    ihm.playTone(330, 333); // E4

    ihm.playTone(494, 167); // B4
    ihm.playTone(440, 167); // A4
    ihm.playTone(280, 333); // C4
    //ihm.playTone(262, 333); // C4
    ihm.playTone(330, 333); // E4

    ihm.playTone(440, 667); // A4, half note

}

void command_radar(const args_t &args){
    static bool on = false;
    on = !on;
    Console::println( on ? "on" : "off");
    intercom.sendRequest(on ? "on" : "off",100);
}

void command_test(const args_t &args){
    motion.setAbsPosition({720, 1440, 90 * DEG_TO_RAD});
    delay(1000);
    //bool isYellow = ihm.isColor(Settings::YELLOW);
    initPump();
    actuators.storePlank(RobotCompass::CA);
    actuators.storePlank(RobotCompass::AB);



    takeStock(
    choose(true,
        POI::stock_4,
        POI::stock_5),
    RobotCompass::AB, 
    TableCompass::NORTH
    );

    waitMs(2000);
    motion.goPolar(getCompassOrientation(TableCompass::NORTH), -200);

    buildTribune(
        Vec2(1100-400, 1050+100),
        RobotCompass::AB,
        TableCompass::WEST
    );


    actuators.drop(RobotCompass::AB);
    actuators.drop(RobotCompass::CA);

    actuators.dropPlank(RobotCompass::AB);
    actuators.dropPlank(RobotCompass::CA);

}

void command_probe(const args_t &args){
    if(args.size() != 2)return;
    String tableCompass = args[0];
    String side = args[1];

    RobotCompass rc = RobotCompass::A;
    if(side.equalsIgnoreCase("A"))         rc = RobotCompass::A;
    else if(side.equalsIgnoreCase("AB"))   rc = RobotCompass::AB;
    else if(side.equalsIgnoreCase("B"))    rc = RobotCompass::B;
    else if(side.equalsIgnoreCase("BC"))   rc = RobotCompass::BC;
    else if(side.equalsIgnoreCase("C"))    rc = RobotCompass::C;
    else if(side.equalsIgnoreCase("CA"))   rc = RobotCompass::CA;

    TableCompass tc = TableCompass::NORTH;
    if(tableCompass.equalsIgnoreCase("NORTH"))         tc = TableCompass::NORTH;
    else if(tableCompass.equalsIgnoreCase("EAST"))   tc = TableCompass::EAST;
    else if(tableCompass.equalsIgnoreCase("SOUTH"))    tc = TableCompass::SOUTH;
    else if(tableCompass.equalsIgnoreCase("WEST"))   tc = TableCompass::WEST;

    probeBorder(tc, rc, 100);

}

//Motion
void command_go(const args_t& args){
    /**/
    if(args.size() != 2) return;
    float x = args[0].toFloat();
    float y = args[1].toFloat();
    async motion.go(x, y);
    /**/
}

//Motion
void command_goAlign(const args_t& args){
    /**/
    if(args.size() != 4) return;
    float x = args[0].toFloat();
    float y = args[1].toFloat();
    String rc = args[2];
    TableCompass tc;

    if(args[3] == "NORTH")      tc = TableCompass::NORTH;
    else if(args[3] == "SOUTH") tc = TableCompass::SOUTH;
    else if(args[3] == "EAST")  tc = TableCompass::EAST;
    else if(args[3] == "WEST")  tc = TableCompass::WEST;

    async motion.goAlign({x, y}, rc == "AB" ? RobotCompass::AB : RobotCompass::CA, getCompassOrientation(tc));
    /**/
}

//Motion
void command_goPolar(const args_t& args){
    /**/
    if(args.size() != 2) return;
    float angle = 361; 
    if(args[0] == "NORTH")      angle = getCompassOrientation(TableCompass::NORTH);
    else if(args[0] == "SOUTH") angle = getCompassOrientation(TableCompass::SOUTH);
    else if(args[0] == "EAST")  angle = getCompassOrientation(TableCompass::EAST);
    else if(args[0] == "WEST")  angle = getCompassOrientation(TableCompass::WEST);
    if(angle == 361) angle = args[0].toFloat();

    float dist = args[1].toFloat();
    async motion.goPolar(angle, dist);
}

void command_goPolarAlign(const args_t& args){
    /**/
    if(args.size() != 2) return;
    float angle = 361; 
    if(args[0] == "NORTH")      angle = getCompassOrientation(TableCompass::NORTH);
    else if(args[0] == "SOUTH") angle = getCompassOrientation(TableCompass::SOUTH);
    else if(args[0] == "EAST")  angle = getCompassOrientation(TableCompass::EAST);
    else if(args[0] == "WEST")  angle = getCompassOrientation(TableCompass::WEST);
    if(angle == 361) angle = args[0].toFloat();


    String side = args[2];
    RobotCompass rc;
    if(side.equalsIgnoreCase("A"))         rc = RobotCompass::A;
    else if(side.equalsIgnoreCase("AB"))   rc = RobotCompass::AB;
    else if(side.equalsIgnoreCase("B"))    rc = RobotCompass::B;
    else if(side.equalsIgnoreCase("BC"))   rc = RobotCompass::BC;
    else if(side.equalsIgnoreCase("C"))    rc = RobotCompass::C;
    else if(side.equalsIgnoreCase("CA"))   rc = RobotCompass::CA;
    

    float orientation = 361; 
    if(args[3] == "NORTH")      orientation = getCompassOrientation(TableCompass::NORTH);
    else if(args[3] == "SOUTH") orientation = getCompassOrientation(TableCompass::SOUTH);
    else if(args[3] == "EAST")  orientation = getCompassOrientation(TableCompass::EAST);
    else if(args[3] == "WEST")  orientation = getCompassOrientation(TableCompass::WEST);
    if(orientation == 361) orientation = args[3].toFloat();

    float dist = args[1].toFloat();
    async motion.goPolarAlign(angle, dist, rc, orientation);
}


void command_move(const args_t& args){
    /**/
    if(args.size() != 3) return;
    float x = args[0].toFloat();
    float y = args[1].toFloat();
    float z = args[2].toFloat();
    Console::info("Interpreter") << "Move to (" << x << ", " << y << ", " << z << ")" << Console::endl;
    async motion.move({x, y, z});
    /**/
}


void command_turn(const args_t& args){
    /**/
    if(args.size() != 1)return;
    float x = args[0].toFloat();
    async motion.turn(x);
    /**/
}

void command_rawTurn(const args_t& args){
    /**/
    if(args.size() != 1)return;
    float x = args[0].toFloat();
    motion.disableOptimization();
    async motion.turn(x);
    motion.enableOptimization();
    /**/
}

void command_pause(const args_t& args){
    motion.pause();
}

void command_resume(const args_t& args){
    motion.resume();
}

void command_cancel(const args_t& args){
    motion.cancel();
}


void command_align(const args_t& args){
    /**/
    if(args.size() != 2)return;
    String side = args[0];
    float orientation = args[1].toFloat();
    if(side.equalsIgnoreCase("A"))         async motion.align(RobotCompass::A, orientation);
    else if(side.equalsIgnoreCase("AB"))   async motion.align(RobotCompass::AB, orientation);
    else if(side.equalsIgnoreCase("B"))    async motion.align(RobotCompass::B, orientation);
    else if(side.equalsIgnoreCase("BC"))   async motion.align(RobotCompass::BC, orientation);
    else if(side.equalsIgnoreCase("C"))    async motion.align(RobotCompass::C, orientation);
    else if(side.equalsIgnoreCase("CA"))   async motion.align(RobotCompass::CA, orientation);
    /**/
}



void command_setAbsolute(const args_t& args){
    /**/
    motion.setAbsolute();
    /**/
}


void command_setRelative(const args_t& args){
    /**/
    motion.setRelative();
    /**/
}


void command_setAbsPosition(const args_t& args){
    if(args.size() != 3)return;
    float x = args[0].toFloat();
    float y = args[1].toFloat();
    float angle = args[2].toFloat() * DEG_TO_RAD;
    motion.setAbsPosition({x, y, angle});
}


void command_resetCompass(const args_t& args){
    //Not implemented yet
}



//Actuators

void command_drop(const args_t& args){
    if(args.size() != 1)return;
    const String& side = args[0];
    if(side.equals("AB")) actuators.drop(RobotCompass::AB);
    //else if(side.equals("BC")) actuators.drop(RobotCompass::BC);
    else if(side.equals("CA")) actuators.drop(RobotCompass::CA);
}


void command_grab(const args_t& args){
    if(args.size() != 1)return;
    const String& side = args[0];
    if(side.equals("AB")) actuators.grab(RobotCompass::AB);
    //else if(side.equals("BC")) actuators.grab(RobotCompass::BC);
    else if(side.equals("CA")) actuators.grab(RobotCompass::CA);
}

void command_storePlank(const args_t& args){
    if(args.size() != 1)return;
    const String& side = args[0];
    if(side.equals("AB")) actuators.storePlank(RobotCompass::AB);
    //else if(side.equals("BC")) actuators.drop(RobotCompass::BC);
    else if(side.equals("CA")) actuators.storePlank(RobotCompass::CA);
}


void command_dropPlank(const args_t& args){
    if(args.size() != 1)return;
    const String& side = args[0];
    if(side.equals("AB")) actuators.dropPlank(RobotCompass::AB);
    //else if(side.equals("BC")) actuators.drop(RobotCompass::BC);
    else if(side.equals("CA")) actuators.dropPlank(RobotCompass::CA);
}

void command_grabPlank(const args_t& args){
    if(args.size() != 1)return;
    const String& side = args[0];
    if(side.equals("AB")) actuators.grabPlank(RobotCompass::AB);
    //else if(side.equals("BC")) actuators.grab(RobotCompass::BC);
    else if(side.equals("CA")) actuators.grabPlank(RobotCompass::CA);
}

void command_pump(const args_t& args){
    if(args.size() != 1)return;
    const String& side = args[0];
    if(side.equals("AB")) startPump(RobotCompass::AB);
    //else if(side.equals("BC")) actuators.grab(RobotCompass::BC);
    else if(side.equals("CA")) startPump(RobotCompass::CA);
}

void command_ev(const args_t& args){
    if(args.size() != 1)return;
    const String& side = args[0];
    if(side.equals("AB")) stopPump(RobotCompass::AB, 500);
    //else if(side.equals("BC")) actuators.grab(RobotCompass::BC);
    else if(side.equals("CA")) stopPump(RobotCompass::CA, 500);
}
void command_initPump(const args_t& args){
    initPump();
}


void command_elevator(const args_t& args){
    if(args.size() != 2)return;
    const String& side = args[0];
    const String& poseStr = args[1];
    int pose = poseStr.toInt();

    if(side.equals("AB")) actuators.moveElevator(RobotCompass::AB, ElevatorPose(pose));
    else if(side.equals("BC")) actuators.moveElevator(RobotCompass::BC, ElevatorPose(pose));
    else if(side.equals("CA")) actuators.moveElevator(RobotCompass::CA, ElevatorPose(pose));
}

void command_move_elevator(const args_t& args){

    if(args.size() != 2)return;
    const String& side = args[0];
    const String& poseStr = args[1];
    int pose = poseStr.toInt();

    if(side.equals("AB")) actuators.moveElevatorAngle(RobotCompass::AB, pose);
    else if(side.equals("BC")) actuators.moveElevatorAngle(RobotCompass::BC, pose);
    else if(side.equals("CA")) actuators.moveElevatorAngle(RobotCompass::CA, pose);
}

void command_raise(const args_t& args){
    if(args.size() != 1)return;
    const String& side = args[0];

    if(side.equals("AB")) actuators.moveElevator(RobotCompass::AB, ElevatorPose::UP);
    else if(side.equals("BC")) actuators.moveElevator(RobotCompass::BC, ElevatorPose::UP);
    else if(side.equals("CA")) actuators.moveElevator(RobotCompass::CA, ElevatorPose::UP);
}

void command_lower(const args_t& args){
    if(args.size() != 1)return;
    const String& side = args[0];

    if(side.equals("AB")) actuators.moveElevator(RobotCompass::AB, ElevatorPose::DOWN);
    else if(side.equals("BC")) actuators.moveElevator(RobotCompass::BC, ElevatorPose::DOWN);
    else if(side.equals("CA")) actuators.moveElevator(RobotCompass::CA, ElevatorPose::DOWN);
}

//Routines 

void command_recalage(const args_t& args){
    recalage();
}


//Terminal
void command_help(const args_t& args){
    auto& commands = CommandHandler::getCommands();
    for (auto i = commands.begin(); i != commands.end(); i++){
        Command c = i->second;
        Console::print(c.getSyntax()); 
        Console::print(" : "); 
        Console::println(c.getDesc()); 
    }
}


void command_print(const args_t& args){
    for(arg_t a : args){
        Console::print(a);   
    }
}

void command_wake(const args_t& args){
    motion.engage();
    actuators.enable();
}

void command_sleep(const args_t& args){
    motion.disengage();
    actuators.disable();
}

//std::vector<String> arguments = extractArguments(args);

/*
 if(arguments.size() == 1){
            Expression e(arguments[0]);
            Vec2 v = Vec2::fromString(e.evaluate());
            execute_go(v);
        }else if(arguments.size() == 2){
            float x = Expression(arguments[0]).evaluate().toFloat();
            float y = Expression(arguments[1]).evaluate().toFloat();
            execute_go(x, y);
        }

    } else if (command == "move") {
        float x = arguments[0].toFloat();
        float y = arguments[1].toFloat();
        float a = arguments[2].toFloat();

*/