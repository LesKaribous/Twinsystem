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
    Controller::init();
    delay(1000);

    Settings::init();
    Debugger::init();

    delay(500); //Pause de demarrage avant lancement
    IHM::init();
    delay(1000);

    //Strategy::waitLaunch();

    while(digitalRead(Pin::initButton)) {IHM::menu();}
    while(!digitalRead(Pin::initButton)){IHM::menu();}
    while(digitalRead(Pin::initButton)) {IHM::menu();}
    IHM::LCD::goScreen();
}

void loop(){
    IHM::LCD::matchScreen(0,0,0);
    //Test::calibrationRotation();
    //Test::calibrationX();
    //Test::calibrationY();
}