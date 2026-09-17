#include "Game/Util/JMapInfo.h" // to fix missing identifiers
#include "Game/Util/SceneUtil.h"
#include "kamek/hooks.h"
#include "neo/core.h"

namespace {
    // =================================================================================================================
    // Star Chance music prohibition
    // =================================================================================================================

    struct StarChanceExceptInfo {
        const char* mStageName;
        int mPowerStarId;
    };

    const StarChanceExceptInfo cStarChanceExceptInfos[] = {
        { STAGE_GLOOMY_GALLEON_GALAXY, 0 },
        { STAGE_TURNLOG_GALAXY,        3 },
        { STAGE_COMET_OBSERVATORY,     0 }
    };

    const s32 cStarChanceExceptInfosCount = sizeof(cStarChanceExceptInfos) / sizeof(StarChanceExceptInfo);

    bool extIsStagePlayStarChance() {
        register s32 powerStarId;
        __asm {
            mr powerStarId, r30
        }

        for (s32 i = 0; i < cStarChanceExceptInfosCount; i++) {
            StarChanceExceptInfo exceptInfo = cStarChanceExceptInfos[i];

            if (MR::isEqualStageName(exceptInfo.mStageName)) {
                if (exceptInfo.mPowerStarId == powerStarId || exceptInfo.mPowerStarId <= 0) {
                    return false;
                }
            }
        }

        return true;
    }

    kmCall(0x80056E90, extIsStagePlayStarChance);


    // =================================================================================================================
    // Prevent playing BGM_BOSSRUSH02 after defeating SMG1 bosses
    // =================================================================================================================

    kmWrite32(0x8001BB20, PPC_BLR);
}
