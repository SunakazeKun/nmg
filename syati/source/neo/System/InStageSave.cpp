#include "neo/System/InStageSave.h"
#include "neo/System/IllustItemAccess.h"
#include "Game/System/AllData/GameDataFunction.h"
#include "Game/System/AllData/GameSequenceFunction.h"
#include "Game/System/AllData/GalaxyStatusAccessor.h"
#include "Game/System/GameSequenceInGame.h"
#include "Game/System/PlayResultInStageHolder.h"
#include "Game/Util/EventUtil.h"
#include "Game/Util/SceneUtil.h"
#include "Game/Util/StringUtil.h"
#include "neo/core.h"

namespace neo {
    const s32 cOtherGreenStarsCount = 2;

    struct AllowInStageSaveInfo {
        const char* mStageName;
        s32 mGreenStarId;
        s32 mOtherGreenStarIds[cOtherGreenStarsCount];
    };

    const AllowInStageSaveInfo cAllowInStageSaveInfos[] = {
        { STAGE_SWEET_TREAT_GALAXY,     4, {  5,  6 } },
        { STAGE_GLOOMY_GALLEON_GALAXY,  4, {  5,  6 } },
        { STAGE_GLOOMY_GALLEON_GALAXY,  5, {  4,  6 } },
        { STAGE_GLOOMY_GALLEON_GALAXY,  6, {  4,  5 } },
        { STAGE_BATTLE_BLAST_GALAXY,    4, {  5,  6 } },
        { STAGE_BATTLE_BLAST_GALAXY,    5, {  6, -1 } },
        { STAGE_TURNLOG_GALAXY,         4, {  5,  6 } },
        { STAGE_TURNLOG_GALAXY,         5, {  4,  6 } },
        { STAGE_TURNLOG_GALAXY,         6, {  4,  5 } },
        { STAGE_KRAKEN_KING_GALAXY,     3, {  4, -1 } },
    };

    bool allowInStageSave(s32 powerStarId) {
        if (!MR::isPowerStarGreenInCurrentStage(powerStarId)) {
            return false;
        }

        const char* stageName = MR::getCurrentStageName();
        s32 scenarioNo = MR::getCurrentScenarioNo();
        GalaxyStatusAccessor gsa = MR::makeGalaxyStatusAccessor(stageName);

        for (int i = 0; i < ARRAY_SIZE(cAllowInStageSaveInfos); i++) {
            const AllowInStageSaveInfo& info = cAllowInStageSaveInfos[i];

            if (!MR::isEqualString(info.mStageName, stageName)) {
                continue;
            }
            if (info.mGreenStarId != powerStarId) {
                continue;
            }

            for (int j = 0; j < cOtherGreenStarsCount; j++) {
                s32 otherGreenStarId = info.mOtherGreenStarIds[j];

                if (otherGreenStarId < 1) {
                    continue;
                }
                if (!gsa.isExistPowerStarAtScenario(scenarioNo, otherGreenStarId)) {
                    continue;
                }

                if (!gsa.hasPowerStar(otherGreenStarId)) {
                    return true;
                }
            }
        }

        return false;
    }

    // -----------------------------------------------------------------------------------------------------------------

    void performInStageSave() {
        PlayResultInStageHolder* resultInStage = GameSequenceFunction::getGameSequenceInGame()->getPlayResultInStageHolder();
        const char* currentStage = resultInStage->mName;

        // Store Comet Medal and Stamp if necessary
        if (resultInStage->mCollectTicoCoin) {
            GameDataFunction::onGalaxyFlagTicoCoin(currentStage);
            resultInStage->mNotifyTicoCoin = false;
        }

        if (resultInStage->_63 & IN_TEMP_STAGE_ILLUST_ITEM_FLAG_COLLECTED) {
            neo::onGalaxyFlagIllustItem(currentStage);
            resultInStage->_63 &= ~IN_TEMP_STAGE_ILLUST_ITEM_FLAG_NOTIFY;
        }

        // Store Star Bits & Coins and clear their in-stage counts
        GameDataFunction::tryOnGameEventFlagStarPieceCounterStop(resultInStage->mClearedStarPieceNum);
        GameDataFunction::tryOnGameEventFlagCoinCounterStop(resultInStage->mClearedCoinNum);

        resultInStage->mClearedStarPieceNum = 0;
        resultInStage->mClearedCoinNum = 0;

        // Save the game
        GameSequenceFunction::startGameDataSaveSequence(true, false, false);
    }
}
