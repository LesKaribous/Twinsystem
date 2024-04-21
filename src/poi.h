#pragma once

#include "utils/geometry.h"

namespace POI{

    // ------------------------------------------
    // Departure Areas - Center
    const Vec2 b1 = Vec2(225,225);  //BLUE
    const Vec2 b2 = Vec2(225,1775); //BLUE
    const Vec2 b3 = Vec2(2775,1000); //BLUE

    const Vec2 y1 = Vec2(2775,225); //YELLOW
    const Vec2 y2 = Vec2(2775,1775);  //YELLOW
    const Vec2 y3 = Vec2(225,1000);  //YELLOW

    // End Of Match
    const Vec2 blueArrival = Vec2(450,1550);  //BLUE
    const Vec2 yellowArrival = Vec2(2550,1550); //Yellow
	// ------------------------------------------
    // Plant Supply - Center
    const Vec2 plantSupplySE = Vec2(1015,1300);     //Plant supply SOUTH EAST
    const Vec2 plantSupplySW = Vec2(1015,700);      //Plant supply SOUTH WEST

    const Vec2 plantSupplyNE = Vec2(1985,1300);     //Plant supply NORTH EAST
    const Vec2 plantSupplyNW = Vec2(1985,700);      //Plant supply NORTH WEST

    const Vec2 plantSupplyW  = Vec2(1500,500);      //Plant supply WEST
    const Vec2 plantSupplyE  = Vec2(1500,1500);     //Plant supply EAST
    // ------------------------------------------
    // Solar Panels
    const Vec2 solarPanelBlue_1 = Vec2(275,1800);      //Solar panel Blue 1
    const Vec2 solarPanelBlue_2 = Vec2(500,1800);      //Solar panel Blue 2
    const Vec2 solarPanelBlue_3 = Vec2(725,1800);      //Solar panel Blue 3

    const Vec2 solarPanelOther_1 = Vec2(1275,1800);   //Solar panel No Team 1
    const Vec2 solarPanelOther_2 = Vec2(1500,1800);   //Solar panel No Team 2
    const Vec2 solarPanelOther_3 = Vec2(1725,1800);   //Solar panel No Team 3

    const Vec2 solarPanelYellow_1 = Vec2(2725,1800);    //Solar panel Yellow 1
    const Vec2 solarPanelYellow_2 = Vec2(2500,1800);    //Solar panel Yellow 2
    const Vec2 solarPanelYellow_3 = Vec2(2275,1800);    //Solar panel Yellow 3
    // ------------------------------------------
    // Planters
    const Vec2 planterBlueSouth = Vec2(0,612);          //Planter Blue South
    const Vec2 planterBlueNorth = Vec2(3000,1388);      //Planter Blue North
    const Vec2 planterBlueWest  = Vec2(762,0);          //Planter Blue West

    const Vec2 planterYellowSouth = Vec2(0,1388);       //Planter Yellow South
    const Vec2 planterYellowNorth = Vec2(3000,612);     //Planter Yellow North
    const Vec2 planterYellowWest  = Vec2(2238,0);    //Planter Yellow West
}