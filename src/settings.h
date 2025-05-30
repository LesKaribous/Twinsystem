#pragma once

#include "pin.h"
#include "utils/geometry.h"

#define INTERCOM_SERIAL Serial1
#define INTERCOM_BAUDRATE 31250

//#define XBEE
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


//Occupancy map size	
#define GRID_WIDTH 20
#define GRID_HEIGHT 13
#define GRID_CELLSIZE 150
#define GRID_BITS   (GRID_WIDTH * GRID_HEIGHT)
#define GRID_BYTES  ((GRID_BITS + 7) / 8)

#define TABLE_SIZE_X 3000 //mm
#define TABLE_SIZE_Y 2000 //mm


// Struct definition
struct  CalibrationProfile{
    Vec3 Holonomic,
         Cartesian;
};

typedef bool Color;

// Namespace
namespace Settings{
    namespace Threads{
        constexpr long DEFAULT_REFRESH = 100; //ms 
        constexpr int DEFAULT_STACKSIZE = 1024; //ms 
    }
    
    constexpr bool
    PRIMARY   = false,
    SECONDARY = !PRIMARY,

    COLOR_A  = true,
    COLOR_B  = !COLOR_A,
    YELLOW = COLOR_A,
    BLUE = COLOR_B;

    namespace Match{
        constexpr bool 
        AVOIDANCE = true,
        NO_AVOIDANCE = !AVOIDANCE;

        constexpr short
        STRAT_PRIMARY_A = false,
        STRAT_PRIMARY_B = !STRAT_PRIMARY_A,
        STRAT_SECONDARY_A = false,
        STRAT_SECONDARY_B = !STRAT_SECONDARY_A;

        constexpr unsigned long 
        DURATION = 100*1000,            //100s
        NEARLY_FINISH = 10 * 1000,      //15s before the end
        ENDMATCH = 200;                 //200ms before the end
        
        //constexpr float 
        //FEEDRATE = 100; //%
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

        constexpr int VELOCITY_SAMPLES = 0,
        MIN_CRUISE_DISTANCE = 600; // 600

        // Kp is used to improve the transient response rise time and settling time of course.
        // Ki works to improve steady-state response. Kd is used to improve the transient response by way of predicting error
        // will occur in the future. 

        //https://en.wikipedia.org/wiki/PID_controller
        //const Vec3 kP = Vec3(3.0, 3.0, 3.5);
        //const float kP = 0.8f;
        //const float kI = 0.0;
        //const float kD = 0.0;

        //const float OTOS_UNCERTAINTY = 0.04;

        const float MAX_PID_DELAY = 10;

        const float 
        MAX_ROT_ACCEL = 50.0f * 3.14f, // rad/s^2 5.6
        MAX_ROT_SPEED = 4.0f * 3.14f ,//3.14, // rad/s
        MIN_ANGLE = 2.0 * DEG_TO_RAD; // Target stop thresold //mm

        const float 
        //STOP_DECCEL = 10000, // mm/s^2
        MAX_ACCEL = 30000, // mm/s^2 300
       // MAX_SPEED = 300, // mm/s 1600      otos max Tracking speed: 2.5m/s
        MAX_SPEED = 6000,//3800 // mm/s 1600      otos max Tracking speed: 2.5m/s
        MIN_DISTANCE = 20; // Target stop thresold //mm
        
        const int
        /*
        PID_MAX_PERIOD = 10,//ms
        PID_MIN_PERIOD = 5,//ms
        */
        PID_INTERVAL = 16000, //microseconds
        PID_MIN_INTERVAL = PID_INTERVAL/2; //microseconds
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
        STOP_DECCEL = 3000, // equivalent fullsteps/s^2
        MAX_ACCEL = 3000, // equivalent fullsteps/s^2
        MAX_SPEED = 15000, // equivalent fullsteps/s
        MIN_SPEED = 20,
        STEPPER_DELAY = 50,//µs Steps every 1000 µs seconds
        MIN_STEP_DELAY = 20,
        MIN_STEPS = 5,
        STEPPER_COMPUTE_DELAY = 1000,//µs Steps every 1000 µs seconds
        PULL_IN = 100,//fullsteps/s
        PULL_OUT = 100; //fullsteps/s

        constexpr u_int8_t 
        STEP_MODE = 8;
    }

    namespace Lidar{
        constexpr unsigned long persitency = 1000;//ms
    } // namespace Lidar
    
    namespace Calibration{
        const CalibrationProfile Primary = {
            { 1.0f, 1.0f, 1.0f }, //Holonomic : ABC
            { 1.089f, -1.089f, 0.831f} //Cartesian : XYROT
        };
    }
}



