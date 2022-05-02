#include "Settings.h"
#include "Pin.h"

namespace Settings{

    bool ROBOT;

    void init(){ 
        delay(500);
        pinMode(Pin::robotSelect, INPUT_PULLUP);
        ROBOT = digitalRead(Pin::robotSelect) ? PRIMARY : SECONDARY ;
    }

}

