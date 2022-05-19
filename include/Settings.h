#pragma once

#include "Geometry.h"
#include "Pin.h"

// Struct definition
struct  CalibrationProfile{
    Vec3 Holonomic,
         Cartesian;
};

// Namespace
namespace Settings{

    const bool 
        ADVERSAIRE_NON = 1,
        ADVERSAIRE_OUI = 0,

        EQUIPE_JAUNE    = 1,
        EQUIPE_VIOLET   = 0,

        STRATEGIE_HOMOLOGATION = 1,
        STRATEGIE_MATCH        = 0,

        PRIMARY   = 1,
        SECONDARY = 0,

        ABSOLUTE = true;

    const int 
        TEMPS_MATCH = 101000 ;

    const float 
        FEEDRATE = 100, //%
        RADIUS = 90,
        WHEEL_RADIUS = 30,
        LIDAR_RANGE = 20; //°

    const uint32_t 
        ACCEL = 3000, // Old : 2000              
        SPEED = 5000; // Old : 5000


    extern bool ROBOT;
    void init();

    namespace Geometry{

        const float offset = 125.98f;
    }
    namespace Stepper{
        const bool 
            ENABLE_POLARITY = true,
            
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
            { 1.165f, -1.165f, -1.0525f } //Cartesian : XYROT
        };

        const CalibrationProfile Secondary = {
            { 1.0f, 1.0f, 1.0f }, //Holonomic : ABC
            { 1.17f, -1.17f, -1.0525f } //Cartesian : XYROT
        };
    }

}



