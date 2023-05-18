#pragma once

#include "math/Geometry.h"

namespace POI{

    // ------------------------------------------
		// Zones de départ
		const Vec2 b1 = Vec2(225,1775);  //BLUE
        const Vec2 b2 = Vec2(1875,1775); //BLUE
        const Vec2 b3 = Vec2(2775,1275); //BLUE
        const Vec2 b4 = Vec2(2775,225);  //BLUE
        const Vec2 b5 = Vec2(1125,225);  //BLUE

		const Vec2 v1 = Vec2(225,225);   //GREEN
		const Vec2 v2 = Vec2(1875,225);  //GREEN
		const Vec2 v3 = Vec2(2775,725);  //GREEN
		const Vec2 v4 = Vec2(2775,1775); //GREEN
		const Vec2 v5 = Vec2(1125,1775); //GREEN

		// ------------------------------------------
		// Placement des cakes
		const Vec2 cakePinkSE = Vec2(575,1775);  //PINK
		const Vec2 cakePinkNE = Vec2(2425,1775); //PINK
		const Vec2 cakePinkNW = Vec2(2425,225);  //PINK
		const Vec2 cakePinkSW = Vec2(575,225);   //PINK

		const Vec2 cakeYellowSE = Vec2(775,1775);     //YELLOW
		const Vec2 cakeYellowNE = Vec2(2225,1775);    //YELLOW
		const Vec2 cakeYellowNW = Vec2(2225,225);     //YELLOW
		const Vec2 cakeYellowSW = Vec2(775,225);      //YELLOW

		const Vec2 cakeBrownSE = Vec2(1125,1275);   //BROWN
		const Vec2 cakeBrownNE = Vec2(1875,1275);   //BROWN
		const Vec2 cakeBrownNW = Vec2(1875,725);    //BROWN
		const Vec2 cakeBrownSW = Vec2(1125,725);    //BROWN

		// ------------------------------------------
		// Distributeurs de balles
		// Distributeur SOUTH
		const Vec2 ballBeginSE  = Vec2(300,1125); //RED
		const Vec2 ballEndSE    = Vec2(200,1125);   //RED

		const Vec2 ballBeginSW  = Vec2(300,875);  //RED
		const Vec2 ballEndSW    = Vec2(200,875);    //RED

		// Distributeur NORTH
		const Vec2 ballBeginNE  = Vec2(2700,1125);//RED
		const Vec2 ballEndNE    = Vec2(2825,1125);  //RED

		const Vec2 ballBeginNW  = Vec2(2700,875); //RED
		const Vec2 ballEndNW    = Vec2(2825,875);   //RED

		// Distributeur WEST
		const Vec2 ballWestN    = Vec2(1650,150);       //RED
		const Vec2 ballWestCenter = Vec2(1500,150);  //RED
		const Vec2 ballWestS    = Vec2(1350,150);       //RED

		// Distributeur EAST
		const Vec2 ballEastN    = Vec2(1650,1850);      //RED
		const Vec2 ballEastCenter = Vec2(1500,1850); //RED
		const Vec2 ballEastS    = Vec2(1350,1850);      //RED

		// ------------------------------------------
		// Dépose des balles
		const Vec2 ballBlueBasket   = Vec2(200,1675);  //ORANGE
		const Vec2 ballGreenBasket  = Vec2(200,325);  //ORANGE
		
		const Vec2 ballBlueBasketPlus   = Vec2(120,1675);  //ORANGE
		const Vec2 ballGreenBasketPlus  = Vec2(120,325);  //ORANGE

		const Vec2 blueEndPrimary   = Vec2(900,400);       //BLUE
		const Vec2 blueEndSecondary = Vec2(1350,400);    //BLUE

		const Vec2 greenEndPrimary   = Vec2(900,1600);       //GREEN
		const Vec2 greenEndSecondary = Vec2(1350,1600);    //GREEN

		// ------------------------------------------
		// Dépose des cakes
		// Blue B1
		const Vec2 dropBlue1 = Vec2(180,1760);
		const Vec2 dropBlue2 = Vec2(320,1760);
		const Vec2 dropBlue3 = Vec2(460,1760);
		
		const Vec2 retreatBlue1 = Vec2(180,1600);
		const Vec2 retreatBlue2 = Vec2(320,1600);
		const Vec2 retreatBlue3 = Vec2(460,1600);

		// Green V1
		const Vec2 dropGreen1 = Vec2(180,240);
		const Vec2 dropGreen2 = Vec2(320,240);
		const Vec2 dropGreen3 = Vec2(460,240);
				
		const Vec2 retreatGreen1 = Vec2(180,400);
		const Vec2 retreatGreen2 = Vec2(320,400);
		const Vec2 retreatGreen3 = Vec2(460,400);

		// Blue B2
		const Vec2 dropB2_01 = Vec2(1875,1775);
		const Vec2 dropB2_02 = Vec2(1875,1635);
		const Vec2 dropB2_03 = Vec2(1875,1495);

		// Blue B3
		const Vec2 dropB3_01 = Vec2(2775,1275);
		const Vec2 dropB3_02 = Vec2(2635,1275);
		const Vec2 dropB3_03 = Vec2(2495,1275);

		// Green V2
		const Vec2 dropV2_01 = Vec2(1875,225);
		const Vec2 dropV2_02 = Vec2(1875,365);
		const Vec2 dropV2_03 = Vec2(1875,505);

		// Green V3
		const Vec2 dropV3_01 = Vec2(2775,725);
		const Vec2 dropV3_02 = Vec2(2635,725);
		const Vec2 dropV3_03 = Vec2(2495,725);

		// ------------------------------------------
}