#pragma once

#include "Geometry.h"

struct  CalibrationData;

namespace Settings{

    const bool 
        ADVERSAIRE_NON = 1,
        ADVERSAIRE_OUI = 0,

        EQUIPE_JAUNE = 1,
        EQUIPE_BLEU  = 0,

        STRATEGIE_HOMOLOGATION = 1,
        STRATEGIE_MATCH        = 0 ;

    const int 
        TEMPS_MATCH = 101000 ;

    namespace Robot{
        const float 
            FEEDRATE = 100, //%
            RADIUS = 150; //??

        const uint32_t 
            ACCEL = 2500, // Old : 5000              
            SPEED = 5000; // Old : 5000
    
        const bool 
            PRIMARY   = 1,
            SECONDARY = 0 ;
    }

    namespace Calibration{
        const CalibrationProfile Primary = {
            { 8.71f, 8.71f, 8.71f }, //ABC
            { 18.545f, 0.85f, 1.50f } //ROTXY
        };

        const CalibrationProfile Secondary = {
            { 8.71f, 8.71f, 8.71f }, //ABC
            { 18.545f, 0.85f, 1.50f } //ROTXY
        };
    }
}



// Struct definition
struct  CalibrationProfile{
    Vec3 Holonomic,
         Cartesian;
};