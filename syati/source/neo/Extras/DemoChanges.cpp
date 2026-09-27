#include "Game/Util/EventUtil.h"
#include "Game/Util/StringUtil.h"
#include "Game/Util/SceneUtil.h"
#include "kamek/hooks.h"
#include "neo/core.h"

namespace {
    struct SkipBossDemoPowerStarInfo {
        const char* mStageName;
        s32 mPowerStarId;
        const char* mBossName;
    };

    const SkipBossDemoPowerStarInfo cSkipBossDemoPowerStarInfos[] = {
        { STAGE_GALACTIC_GARDEN_GALAXY,  1, "ディノパックンVs1" },
        { STAGE_KRAKEN_KING_GALAXY,      1, "オタキング" },
        { STAGE_KRAKEN_KING_GALAXY,      2, "オタキング" },
    };

    bool isAllowSkipBossDemo(const char *pBossName) {
        for (int i = 0; i < ARRAY_SIZE(cSkipBossDemoPowerStarInfos); i++) {
            const SkipBossDemoPowerStarInfo& info = cSkipBossDemoPowerStarInfos[i];

            if (!MR::isEqualString(info.mBossName, pBossName)) {
                continue;
            }
            if (!MR::isEqualStageName(info.mStageName)) {
                continue;
            }
            
            s32 powerStarId = MR::getCurrentScenarioNo();

            if (info.mPowerStarId == powerStarId) {
                return MR::hasPowerStarInCurrentStage(powerStarId);
            }
        }

        return false;
    }


    // =================================================================================================================

    bool extIsAllowSkipBossDemo(const char *pDeclarer, s32 powerStarId) {
        if (MR::hasPowerStarInCurrentStageWithDeclarer(pDeclarer, powerStarId)) {
            return true;
        }

        return isAllowSkipBossDemo(pDeclarer);
    }

    kmCall(0x8002074C, extIsAllowSkipBossDemo);
}
