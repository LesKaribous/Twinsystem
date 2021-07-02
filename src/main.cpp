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
//     Version : 1.0.0
// Last update : 01 / 07 / 2021
                                    
#include "Intercom.h"
#include "Actuators.h"
#include "IHM.h"

void setup(){
    Intercom::init();
    delay(500);               //Pause de demarrage avant lancement
    IHM::init();
    Actuators::init();

    //Motion::init();

    //Match::waitLaunch();
}

void loop(){
    //Match::wait();

    //Strategy::homologationPrimaire();
    //homologationSecondaire();
    //matchPrimaire();
    //matchSecondaire();

    //Match::waitFinMatch();
    //Match::finMatch();
}