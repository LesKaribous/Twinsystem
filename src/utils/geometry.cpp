#include "geometry.h"

float getCompassOrientation(TableCompass tc){
    switch (tc){
    case TableCompass::NORTH:
        return 0 + 90;
        break;
    case TableCompass::SOUTH:
        return 180 + 90;
        break;
    case TableCompass::EAST:
        return -90 + 90;
        break;
    case TableCompass::WEST:
        return 90 + 90;
        break;
    default:
        return 0 + 90;
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

bool validCompassString(const String& str){
    return str.equalsIgnoreCase("A") || str.equalsIgnoreCase("AB") || str.equalsIgnoreCase("B") || str.equalsIgnoreCase("BC") || str.equalsIgnoreCase("C") || str.equalsIgnoreCase("CA");
}

RobotCompass compassFromString(const String& str){
    if(str.equalsIgnoreCase("A"))         return RobotCompass::A;
    else if(str.equalsIgnoreCase("AB"))   return RobotCompass::AB;
    else if(str.equalsIgnoreCase("B"))    return RobotCompass::B;
    else if(str.equalsIgnoreCase("BC"))   return RobotCompass::BC;
    else if(str.equalsIgnoreCase("C"))    return RobotCompass::C;
    else if(str.equalsIgnoreCase("CA"))   return RobotCompass::CA;
    else return RobotCompass::A; //Default
}

String compassToString(RobotCompass rc){
    switch (rc){
    case RobotCompass::A:
        return "A";
        break;
    case RobotCompass::B:
        return "B";
        break;
    case RobotCompass::C:
        return "C";
        break;
    case RobotCompass::AB:
        return "AB";
        break;
    case RobotCompass::BC:
        return "BC";
        break;
    case RobotCompass::CA:
        return "CA";
        break;
    default:
        return "";
        break;
    }
}

float getOffsets(RobotCompass rc){
    switch (rc){
    case RobotCompass::A:
        return 137.00;
        break;
    case RobotCompass::B:
        return 137.00;
        break;
    case RobotCompass::C:
        return 137.00;
        break;
    case RobotCompass::AB:
        return 107.00; //With Bearings
        break;
    case RobotCompass::BC:
        return 107.00; //With Bearings
        break;
    case RobotCompass::CA:
        return 107.00; //With Bearings
        break;
    default:
        return 0;
        break;
    }
}