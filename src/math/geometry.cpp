#include "geometry.h"

float getCompassOrientation(TableCompass tc){
    switch (tc){
    case TableCompass::NORTH:
        return 0;
        break;
    case TableCompass::SOUTH:
        return 180;
        break;
    case TableCompass::EAST:
        return -90;
        break;
    case TableCompass::WEST:
        return 90;
        break;
    default:
        return 0;
        break;
    }
    
}

float getCompassOrientation(RobotCompass rc){
    switch (rc){
    case RobotCompass::A:
        return 0;
        break;
    case RobotCompass::B:
        return 120;
        break;
    case RobotCompass::C:
        return -120;
        break;
    case RobotCompass::AB:
        return 60;
        break;
    case RobotCompass::BC:
        return 180;
        break;
    case RobotCompass::CA:
        return -60;
        break;
    default:
        return 0;
        break;
    }
}

float getOffsets(RobotCompass rc){
    switch (rc){
    case RobotCompass::A:
        return 157.01;
        break;
    case RobotCompass::B:
        return 136.28;
        break;
    case RobotCompass::C:
        return 136.28;
        break;
    case RobotCompass::AB:
        return 134.06;
        break;
    case RobotCompass::BC:
        return 134.06;
        break;
    case RobotCompass::CA:
        return 134.06;
        break;
    default:
        return 0;
        break;
    }
}