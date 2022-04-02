#include "Settings.h"
#include "Pin.h"

namespace Settings{

    bool ROBOT;

    void init(){ 
        ROBOT = digitalRead(Pin::robotSelect) ? PRIMARY : SECONDARY ;
    }

}

