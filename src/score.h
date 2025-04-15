#pragma once

namespace Score {

    // ------------------------------------------
    // PREPARE THE CONCERT HALL
    constexpr int TribuneLevel1Points = 4;      // Per level 1 tribune
    constexpr int TribuneLevel2Points = 8;      // Per level 2 tribune
    constexpr int TribuneLevel3Points = 16;     // Per level 3 tribune

    // ------------------------------------------
    // ENSURE THE PROMOTION OF THE SHOW
    constexpr int BannerPoints = 20;           // By valid banner for the team

    // ------------------------------------------
    // STORE TOOLS
    constexpr int RobotInArrivalZonePoints = 10; // If the main robot is in the arrival area

    // ------------------------------------------
    //(not used for estimation)
    // IT'S SHOW TIME!
    constexpr int GroupieInZonePoints = 5;     // Per area of the pit occupied by at least one team groupie at the end of the match
    constexpr int SuperstarOnStagePoints = 5;  // If the team’s superstar is valid on stage at the end of the match.
    constexpr int AllPAMIsPartyPoints = 10;    // If all SIMA make the party
    constexpr int SceneFreeZoneMax = 15;       // Max number for free zones behind the superstar
    // Points depend on the highest numbered free zone behind the superstar

} 