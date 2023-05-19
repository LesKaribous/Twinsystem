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
        BROWN = true,
        NOBROWN = false,

        BLUE   = false,
        GREEN  = true;

        const unsigned long 
        DURATION = 100*1000,//100 * 1000,
        ENDMATCH = 200;
        
        const float 
        FEEDRATE = 100; //%
    }

    namespace Inputs{
        namespace Turbine{
            const bool
            LOW_MODE = true,
            HIGH_MODE = true,
            STANDBY = false;
        }

        namespace Trap{
            const bool
            UP = true,
            DOWN = true;
        }
    }

    namespace Actuators{
        namespace Gripper{
            namespace AB{
                const int 
                right_Close = 30,
                left_Close = 160,
                cherry_Close = 100,
                right_Open = 140,
                left_Open = 50,
                cherry_Open = 30,
                right_Grab = 80,
                left_Grab = 110;
            }
            
            namespace BC{
                const int 
                right_Close = 30,
                left_Close = 160,
                cherry_Close = 100,
                right_Open = 140,
                left_Open = 50,
                cherry_Open = 30,
                right_Grab = 80,
                left_Grab = 110;
            }

            namespace CA{
                const int 
                right_Close = 30,
                left_Close = 160,
                cherry_Close = 100,
                right_Open = 140,
                left_Open = 50,
                cherry_Open = 30,
                right_Grab = 80,
                left_Grab = 110;
            }
        }
        
        namespace cherryPicker{
            const int
            trap_Close = 50,
            trap_Open = 140,
            trap_Grab = 50;
        }
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
            ACCEL = 6000, // Old : 3000
            SPEED = 8000; // Old : 4000
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

    namespace Lidar{
        const unsigned long persitency = 1000;//ms
    } // namespace Lidar
    

    namespace Calibration{
        const CalibrationProfile Primary = {
            { 1.0f, 1.0f, 1.0f }, //Holonomic : ABC
            { 1.165f, 1.165f, -0.747} //Cartesian : XYROT
        };

        const CalibrationProfile Secondary = {
            { 1.0f, 1.0f, 1.0f }, //Holonomic : ABC
            { 1.165f, 1.165f, -0.747} //Cartesian : XYROT
        };
    }
}



