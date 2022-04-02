#include "Twinsystem.h"
#include "Test.h"

namespace Test{

    void calibrationRotation(){
        Motion::go({0,0,1080});
        delay(10000);
        Motion::go({0,0,-1080});
        delay(10000);
    }

    void calibrationX(){
        Motion::go({200,0,0});
        delay(10000);
        Motion::go({-200,0,0});
        delay(10000);
    }

    void calibrationY(){
        Motion::go({0,200,0});
        delay(10000);
        Motion::go({0,-200,0});
        delay(10000);
    }


} // namespace Test
