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


float getBorderDistance(Vec2 pos, float angle){

    //TODO Optimize that shit
	Vec2 tableHit = Vec2(3000,0);
	tableHit.rotate(angle);
	tableHit = Vec2::add(pos, tableHit);
	if(tableHit.a > 3000) tableHit.a = 3000;
	if(tableHit.a < 0) tableHit.a = 0;
	if(tableHit.b > 2000) tableHit.b = 2000;
	if(tableHit.b <0) tableHit.b = 0;
	tableHit.sub(pos);
	float maxdist = tableHit.mag();

    return maxdist;

}


float getOffsets(RobotCompass rc){
    switch (rc){
    case RobotCompass::A:
        return 126.00;
        break;
    case RobotCompass::B:
        return 126.00;
        break;
    case RobotCompass::C:
        return 126.00;
        break;
    case RobotCompass::AB:
        return 112.53; //With Bearings
        break;
    case RobotCompass::BC:
        return 112.53; //With Bearings
        break;
    case RobotCompass::CA:
        return 112.53; //With Bearings
        break;
    default:
        return 0;
        break;
    }
}