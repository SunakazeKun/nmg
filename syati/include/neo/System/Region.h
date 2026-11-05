#pragma once

#include "revolution/types.h"

namespace neo {
    enum GameRegion {
        GAME_REGION_JAPAN,   // J
        GAME_REGION_AMERICA, // E
        GAME_REGION_EUROPE,  // P
        GAME_REGION_CHINA,   // C
        GAME_REGION_KOREA,   // K
        GAME_REGION_ASIA,    // A
        GAME_REGION_UNKNOWN,
    };

    GameRegion getGameRegionFromDVDDiskID();
}
