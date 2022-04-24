//
//  _______       _                     _            
// |__   __|     (_)                   | |  
//    | |_      ___ _ __  ___ _   _ ___| |_ ___ _ __ ___   
//    | \ \ /\ / / | '_ \/ __| | | / __| __/ _ \ '_ ` _ \ 
//    | |\ V  V /| | | | \__ \ |_| \__ \ ||  __/ | | | | |
//    |_| \_/\_/ |_|_| |_|___/\__, |___/\__\___|_| |_| |_| 
//                             __/ |                                                               
//                            |___/                                                                
//
//     Author  : Nadarbreicq, JulesTopart
//     Version : 2.4.2
//     Last update : 30 / 03 / 2022
                                    
#include "Twinsystem.h"

#include "Test.h"

void setup(){
    //--- INIT ---
    Controller::init();
    Settings::init();
    Debugger::init();
    IHM::init();
    Actuators::init();
    //--- WAIT LAUNCH---
    Strategy::waitLaunch();

}

void loop(){
    if(IHM::getStrategie() == Settings::STRATEGIE_MATCH ) 
        Strategy::match();
    else if(IHM::getStrategie() == Settings::STRATEGIE_HOMOLOGATION) 
        Strategy::homologation();

    Match::end();
}