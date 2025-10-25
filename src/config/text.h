#pragma once
#include <string>

namespace Text{
    constexpr char
    //Colors
    COLOR_A[] = "Yellow",
    COLOR_B[] = "Blue",

    //Strategy
    STRAT_PRIMARY_A[] = "ETAGE",
    STRAT_PRIMARY_B[] = "MATCH RDC",
    STRAT_SECONDARY_A[] = "ETAGE",
    STRAT_SECONDARY_B[] = "MATCH RDC",

    //Lidar
    LIDAR_DISCONNECTED[] = "Waiting...",
    LIDAR_CONNECTED[] = "Connected",

    //Starter
    STARTER_ARMED[] = "armed !",
    STARTER_UNARMED[] = "unarmed !",

    //Probe State
    PROBE_NOT[] = "Not probed",
    PROBING[] = "Probing ...",
    PROBED[] = "Probed.",
    PROBE_UNKNOWN[] = "?";
}