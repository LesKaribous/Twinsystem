#pragma once

#include "pin.h"
#include "utils/geometry.h"

#define INTERCOM_SERIAL Serial1
#define INTERCOM_BAUDRATE 31250

#define CONSOLE_SERIAL Serial
#define CONSOLE_BAUDRATE 14400

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
        YELLOW  = true;

        constexpr unsigned long 
        DURATION = 90*1000,        //90s
        NEARLY_FINISH = 10 * 1000,       //10s before the end
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
        const int speed = 20;
    }

    namespace Geometry{
        constexpr double 
        RADIUS = 125.98,
        WHEEL_RADIUS = 30;
    }
    
    namespace Motion{
        constexpr bool
        RELATIVE = false,
        ABSOLUTE = true,
        USE_GYROSCOPE = true;

        constexpr int VELOCITY_SAMPLES = 0;

        // Kp is used to improve the transient response rise time and settling time of course.
        // Ki works to improve steady-state response. Kd is used to improve the transient response by way of predicting error
        // will occur in the future. 

        //https://en.wikipedia.org/wiki/PID_controller
        //const Vec3 kP = Vec3(3.0, 3.0, 3.5);
        const Vec3 vkP = Vec3(1.0, 1.0, 1.5);
        const Vec3 kP = Vec3(1.0, 1.0, 1.5);
        const Vec3 kI = Vec3(0.0, 0.0, 0.0);
        const Vec3 kD = Vec3(0.0, 0.0, 0.0);

        //0.28 ok
        //old values 0.0004
        const uint32_t 
        STOP_DECCEL = 5000, // equivalent fullsteps/s^2
        ACCEL = 900, // equivalent fullsteps/s^2
        SPEED = 600, // equivalent fullsteps/s
        TURN_SPEED = 500, // Old : 5000
        PULLIN = 20,
        PID_MAX_PERIOD = 10,//ms
        PID_MIN_PERIOD = 5,//ms
        PID_INTERVAL = 10000;//microseconds
    }

    namespace Stepper{
        constexpr bool 
        ENABLE_POLARITY = false,
        
        STEP_A_POLARITY = true,
        STEP_B_POLARITY = true,
        STEP_C_POLARITY = true,

        DIR_A_POLARITY = true,
        DIR_B_POLARITY = true,
        DIR_C_POLARITY = true;

        constexpr u_int8_t 
        STEP_MODE = 4;
    }

    namespace Lidar{
        constexpr unsigned long persitency = 1000;//ms
    } // namespace Lidar
    
    namespace Calibration{
        const CalibrationProfile Primary = {
            { 1.0f, 1.0f, 1.0f }, //Holonomic : ABC
            { 1.052, -1.052f, 0.6750f} //Cartesian : XYROT
        };
    }
}



