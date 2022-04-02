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

        EQUIPE_JAUNE = 1,
        EQUIPE_BLEU  = 0,

        STRATEGIE_HOMOLOGATION = 1,
        STRATEGIE_MATCH        = 0,

        PRIMARY   = 1,
        SECONDARY = 0 ;

    const int 
        TEMPS_MATCH = 101000 ;

    const float 
        FEEDRATE = 100, //%
        RADIUS = 90,
        WHEEL_RADIUS = 30;

    const uint32_t 
        ACCEL = 1000, // Old : 5000              
        SPEED = 3000; // Old : 5000


    extern bool ROBOT;
    void init();

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
            
    } // namespace Stepper 

    namespace Calibration{
        const CalibrationProfile Primary = {
            { 8.71f, 8.71f, 8.71f }, //Holonomic : ABC
            { 1.0f, 1.0f, 1.0f } //Cartesian : XYROT
        };

        const CalibrationProfile Secondary = {
            { 8.71f, 8.71f, 8.71f }, //Holonomic : ABC
            { 1.17f, 1.17f, 1.045f } //Cartesian : XYROT
        };
    }
}



