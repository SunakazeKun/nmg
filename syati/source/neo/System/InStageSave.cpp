#include "neo/System/InStageSave.h"
#include "neo/System/IllustItemAccess.h"

namespace neo {
	void performSaveInStage() {
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
		resultInStage->mClearedStarPieceNum = 0;
		GameDataFunction::tryOnGameEventFlagCoinCounterStop(resultInStage->mClearedCoinNum);
		resultInStage->mClearedCoinNum = 0;

		// Save the game
		GameSequenceFunction::startGameDataSaveSequence(true, false, false);
	}
}
