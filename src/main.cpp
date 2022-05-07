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
    System::init();
}

void loop(){
    //--- START MATCH---
    Match::start();
    if(IHM::getStrategie() == Settings::STRATEGIE_MATCH ) 
        Strategy::match();
    else if(IHM::getStrategie() == Settings::STRATEGIE_HOMOLOGATION) 
        Strategy::homologation();
    //--- END MATCH---
    Match::end();
}
