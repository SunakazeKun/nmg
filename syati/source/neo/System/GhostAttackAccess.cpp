#include "neo/System/GhostAttackAccess.h"
#include "Game/System/AllData/GalaxyStatusAccessor.h"
#include "Game/System/AllData/GameDataFunction.h"
#include "Game/System/ScenarioDataParser.h"

#define TOTAL_GHOST_ATTACKS (21)

namespace neo {
    s32 calcGhostAttackNum() {
        s32 ghosts = 0;

        ScenarioDataIter iter;
        MR::makeBeginScenarioDataIter(&iter);

        for (; !iter.isEnd(); iter.goNext()) {
            GalaxyStatusAccessor accessor = iter.makeAccessor();
            s32 powerStarNum = accessor.getPowerStarNum();

            if (powerStarNum <= 0) {
                continue;
            }

            for (s32 scenario = 1; scenario <= powerStarNum; scenario++) {
                if (accessor.isGreenStar(scenario)) {
                    continue;
                }

                if (GameDataFunction::isOnGalaxyScenarioFlagGhostLuigi(accessor.getName(), scenario)) {
                    ghosts++;
                }
            }
        }

        return ghosts;
    }

    bool hasAllGhostAttack() {
        return GameDataFunction::isOnGameEventFlagCoinCounterStop() || calcGhostAttackNum() == TOTAL_GHOST_ATTACKS;
    }
}
