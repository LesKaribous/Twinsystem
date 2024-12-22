#pragma once
#include <string>

namespace Text{
    constexpr char
    //Colors
    COLOR_A[] = "Yellow",
    COLOR_B[] = "Blue",

    //Strategy
    STRAT_PRIMARY_A[] = "HOMOLOGATION",
    STRAT_PRIMARY_B[] = "MATCH",
    STRAT_SECONDARY_A[] = "HOMOLOGATION",
    STRAT_SECONDARY_B[] = "MATCH",

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