#pragma once

#include "pin.h"
#include "system/math/geometry.h"

// Struct definition
struct  CalibrationProfile{
    Vec3 Holonomic,
         Cartesian;
};

// Namespace
namespace Settings{

    namespace Match{
        constexpr bool 
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

        constexpr unsigned long 
        DURATION = 100*1000,        //100s
        NEARLY_FINISH = 8000,       //8s before the end
        ENDMATCH = 200;             //200ms before the end
        
        constexpr float 
        FEEDRATE = 100; //%
    }

    namespace Inputs{
        namespace Turbine{
            constexpr bool
            LOW_MODE = true,
            HIGH_MODE = true,
            STANDBY = false;
        }

        namespace Trap{
            constexpr bool
            UP = true,
            DOWN = true;
        }
    }

    namespace Actuators{
        namespace Gripper{
            namespace AB{
                constexpr int 
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
                constexpr int 
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
                constexpr int 
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
            constexpr int
            trap_Close = 50,
            trap_Open = 140,
            trap_Grab = 50;
        }
    }

    namespace Geometry{
        constexpr float 
        RADIUS = 125.98f,
        WHEEL_RADIUS = 30;
    }
    
    namespace Motion{
        constexpr bool
        RELATIVE = false,
        ABSOLUTE = true;


        // Kp is used to improve the transient response rise time and settling time of course.
        // Ki works to improve steady-state response. Kd is used to improve the transient response by way of predicting error
        // will occur in the future. 

        //https://en.wikipedia.org/wiki/PID_controller
        const Vec3 kP = Vec3(3.0, 3.0, 3.5) / 10000.0;
        const Vec3 kI = Vec3(0.0, 0.0, 0.0) / 10000.0;
        const Vec3 kD = Vec3(0.0, 0.0, 0.0) / 10000.0;

        //0.28 ok
        //old values 0.0004
        const uint32_t 
            ACCEL = 20000, // Old : 5000
            SPEED = 5000, // Old : 5000
            PULLIN = 1000,
            PID_MAX_PERIOD = 10,//ms
            PID_MIN_PERIOD = 50;//ms
    }

    namespace Stepper{
        constexpr bool 
        ENABLE_POLARITY = false,
        
        STEP_A_POLARITY = true,
        STEP_B_POLARITY = true,
        STEP_C_POLARITY = true,

        DIR_A_POLARITY = false,
        DIR_B_POLARITY = false,
        DIR_C_POLARITY = false;

        constexpr u_int8_t 
        STEP_MODE = 8;
    }

    namespace Lidar{
        constexpr unsigned long persitency = 1000;//ms
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



