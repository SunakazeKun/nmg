#include "neo/System/Region.h"
#include "revolution/os.h"

namespace neo {
    GameRegion getGameRegionFromDVDDiskID() {
        const char* regionCode = (const char *)OSPhysicalToCached(3);

        switch (*regionCode) {
        case 'J':
            return GAME_REGION_JAPAN;
        case 'E':
            return GAME_REGION_AMERICA;
        case 'P':
            return GAME_REGION_EUROPE;
        case 'C':
            return GAME_REGION_CHINA;
        case 'K':
            return GAME_REGION_KOREA;
        case 'W':
            return GAME_REGION_ASIA;
        default:
            return GAME_REGION_UNKNOWN;
        }
    }
}
