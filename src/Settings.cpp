#include "Settings.h"
#include "Pin.h"

namespace Settings{

    bool ROBOT;

    void init(){ 
        pinMode(Pin::robotSelect, INPUT_PULLUP);
        ROBOT = digitalRead(Pin::robotSelect) ? PRIMARY : SECONDARY ;
    }

}

