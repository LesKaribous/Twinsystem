#pragma once

#include "pin.h"
#include "math/geometry.h"

// Struct definition
struct  CalibrationProfile{
    Vec3 Holonomic,
         Cartesian;
};

// Namespace
namespace Settings{

    namespace Match{
        const bool 
        AVOIDANCE = 1,
        NO_AVOIDANCE = 0,

        PRIMARY   = false,
        SECONDARY = true,

        CHERRY = false,
        CAKE = true,

        BLUE   = false,
        GREEN  = true;

        const unsigned long 
        DURATION = 100000 ;

        const float 
        FEEDRATE = 100; //%
    }


    namespace Geometry{
        const float 
        RADIUS = 125.98f,
        WHEEL_RADIUS = 30;
    }
    
    namespace Motion{
        const bool
        RELATIVE = false,
        ABSOLUTE = true;


        const uint32_t 
            ACCEL = 3000, // Old : 4000
            SPEED = 4000; // Old : 5000
    }

    namespace Stepper{
        const bool 
        ENABLE_POLARITY = false,
        
        STEP_A_POLARITY = true,
        STEP_B_POLARITY = true,
        STEP_C_POLARITY = true,

        DIR_A_POLARITY = false,
        DIR_B_POLARITY = false,
        DIR_C_POLARITY = false;

        const u_int8_t 
        STEP_MODE = 8;
            
    }

    namespace Calibration{
        const CalibrationProfile Primary = {
            { 1.0f, 1.0f, 1.0f }, //Holonomic : ABC
            { 1.165f, 1.165f, -0.755} //Cartesian : XYROT
        };

        const CalibrationProfile Secondary = {
            { 1.0f, 1.0f, 1.0f }, //Holonomic : ABC
            { 1.17f, 1.17f, -0.755} //Cartesian : XYROT
        };
    }
}



