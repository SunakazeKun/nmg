#include "Game/LiveActor/LiveActor.h"
#include "Game/Util/JMapUtil.h"
#include "Game/Util/SceneUtil.h"
#include "kamek/hooks.h"
#include "neo/core.h"

namespace {
    struct GhostAttackGhostEndingNerveOverride {
        const char* mStageName;
        s32 mScenarioNo;
        s32 mNerveType;
    };

    const GhostAttackGhostEndingNerveOverride cEndingNerveOverrideInfos[] = {
        { STAGE_SWEET_TREAT_GALAXY,  3, 0 },
        { STAGE_BATTLE_BLAST_GALAXY, 3, 0 },
        { STAGE_TURNLOG_GALAXY,      3, 1 },
    };

    void getEndingNerveFromJMap(const JMapInfoIter &rIter, s32 *pDest) {
        for (int i = 0; i < ARRAY_SIZE(cEndingNerveOverrideInfos); i++) {
            const GhostAttackGhostEndingNerveOverride& info = cEndingNerveOverrideInfos[i];

            if (!MR::isEqualStageName(info.mStageName)) {
                continue;
            }
            if (MR::getCurrentSelectedScenarioNo() != info.mScenarioNo) {
                continue;
            }
            
            *pDest = info.mNerveType;
            return;
        }

        MR::getJMapInfoArg0NoInit(rIter, pDest);
    }

    kmCall(0x80349460, getEndingNerveFromJMap);
}
