#pragma once

#include "pin.h"
#include "utils/geometry.h"

#define INTERCOM_SERIAL Serial1
#define INTERCOM_BAUDRATE 31250


#ifdef XBEE
#define CONSOLE_SERIAL Serial3
#define CONSOLE_BAUDRATE 9600
#else
#define CONSOLE_SERIAL Serial
#define CONSOLE_BAUDRATE 115200
#endif

#define TW_STRAT1 CHERRY
#define TW_STRAT2 CAKE
#define TW_STRAT_OPT1 BROWN
#define TW_STRAT_OPT2 NOBROWN

// Struct definition
struct  CalibrationProfile{
    Vec3 Holonomic,
         Cartesian;
};

// Namespace
namespace Settings{
    namespace Threads{
        constexpr long DEFAULT_REFRESH = 100; //ms 
        constexpr int DEFAULT_STACKSIZE = 1024; //ms 
    }
    
    namespace Match{
        constexpr bool 
        AVOIDANCE = true,
        NO_AVOIDANCE = !AVOIDANCE,

        PRIMARY   = false,
        SECONDARY = !PRIMARY,

        COLOR_A  = false,
        COLOR_B  = !COLOR_A;

        constexpr short
        STRAT_PRIMARY_A = false,
        STRAT_PRIMARY_B = !STRAT_PRIMARY_A,
        STRAT_SECONDARY_A = false,
        STRAT_SECONDARY_B = !STRAT_SECONDARY_A;

        constexpr unsigned long 
        DURATION = 90*1000,        //90s
        NEARLY_FINISH = 10 * 1000,       //10s before the end
        ENDMATCH = 200;             //200ms before the end
        
        constexpr float 
        FEEDRATE = 100; //%
    }

    namespace Inputs{

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
        const float kP = 0.8f;
        const float kI = 0.0;
        const float kD = 0.0;

        const float MAX_PID_DELAY = 10;

        const float 
        MAX_ROT_ACCEL = 1.0, // rad/s^2
        MAX_ROT_SPEED = 1.0, // rad/s
        MIN_ANGLE = 0.05; // Target stop thresold //mm

        const int 
        STOP_DECCEL = 100, // mm/s^2
        MAX_ACCEL = 80, // mm/s^2
        MAX_SPEED = 300, // mm/s
        MIN_DISTANCE = 1, // Target stop thresold //mm
        TURN_SPEED = 1, // rad/s
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

        DIR_A_POLARITY = false,
        DIR_B_POLARITY = false,
        DIR_C_POLARITY = false;

        constexpr int
        PULSE_WIDTH = 14, //https://www.analog.com/media/en/technical-documentation/data-sheets/tmc2209_datasheet_rev1.09.pdf   page 63
        STEPS_PER_REVOLUTION = 200,
        STOP_DECCEL = 5000, // equivalent fullsteps/s^2
        MAX_ACCEL = 10000, // equivalent fullsteps/s^2
        MAX_SPEED = 3000, // equivalent fullsteps/s
        MIN_SPEED = 20,
        STEPPER_DELAY = 100,//µs Steps every 1000 µs seconds
        STEPPER_COMPUTE_DELAY = 1000,//µs Steps every 1000 µs seconds
        PULLIN = 10; //fullsteps/s

        constexpr u_int8_t 
        STEP_MODE = 8;
    }

    namespace Lidar{
        constexpr unsigned long persitency = 1000;//ms
    } // namespace Lidar
    
    namespace Calibration{
        const CalibrationProfile Primary = {
            { 1.0f, 1.0f, 1.0f }, //Holonomic : ABC
            { 1.1f, -1.1f, 1.705f} //Cartesian : XYROT
        };
    }
}



