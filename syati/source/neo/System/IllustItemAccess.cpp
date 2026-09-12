#include "neo/System/IllustItemAccess.h"

namespace neo {
	/*********************************************************************************************************************/
	/* Temporary in-stage item collection handling                                                                       */
	/*********************************************************************************************************************/
	bool isIllustItemInTempStageGet() {
		PlayResultInStageHolder* playResult = GameSequenceFunction::getGameSequenceInGame()->getPlayResultInStageHolder();
		return playResult->_63 & (IN_TEMP_STAGE_ILLUST_ITEM_FLAG_COLLECTED | IN_TEMP_STAGE_ILLUST_ITEM_FLAG_SAVED);
	}

	bool isIllustItemInTempStageCollected() {
		PlayResultInStageHolder* playResult = GameSequenceFunction::getGameSequenceInGame()->getPlayResultInStageHolder();
		return playResult->_63 & IN_TEMP_STAGE_ILLUST_ITEM_FLAG_COLLECTED;
	}

	bool isIllustItemInTempStageNotify() {
		PlayResultInStageHolder* playResult = GameSequenceFunction::getGameSequenceInGame()->getPlayResultInStageHolder();
		return playResult->_63 & IN_TEMP_STAGE_ILLUST_ITEM_FLAG_NOTIFY;
	}

	void collectIllustItemInTempStage() {
		PlayResultInStageHolder* playResult = GameSequenceFunction::getGameSequenceInGame()->getPlayResultInStageHolder();
		playResult->_63 |= IN_TEMP_STAGE_ILLUST_ITEM_FLAG_COLLECTED;
	}

	void tryIllustItemStoreAndNotifyInTempStage() {
		PlayResultInStageHolder* playResult = GameSequenceFunction::getGameSequenceInGame()->getPlayResultInStageHolder();

		if (playResult->_63 & IN_TEMP_STAGE_ILLUST_ITEM_FLAG_COLLECTED) {
			playResult->_63 |= IN_TEMP_STAGE_ILLUST_ITEM_FLAG_SAVED;

			if (!isOnGalaxyFlagIllustItem(playResult->mStageName)) {
				playResult->_63 |= IN_TEMP_STAGE_ILLUST_ITEM_FLAG_NOTIFY;
			}
		}
	}


	/*********************************************************************************************************************/
	/* Galaxy item collection flags                                                                                      */
	/*********************************************************************************************************************/
	bool isOnGalaxyFlagIllustItem(const char *pStageName) {
		GameDataSomeGalaxyStorage* storage = GameDataFunction::makeSomeGalaxyStorage(pStageName);
		return storage->mFlag & SOME_GALAXY_STORAGE_FLAG_ILLUST_ITEM;
	}

	void onGalaxyFlagIllustItem(const char *pStageName) {
		GameDataSomeGalaxyStorage* storage = GameDataFunction::makeSomeGalaxyStorage(pStageName);
		storage->mFlag |= SOME_GALAXY_STORAGE_FLAG_ILLUST_ITEM;
	}

	s32 calcIllustItemNum() {
		UserFile* userFile = GameDataFunction::getSaveDataHandleSequence()->getCurrentUserFile();
		GameDataAllGalaxyStorage* allGalaxyStorage = userFile->mGameDataHolder->mAllGalaxyStorage;

		s32 illustItemNum = 0;

		for (int i = 0; i < allGalaxyStorage->mNumGalaxies; i++) {
			if (allGalaxyStorage->mGalaxyStorages[i]->mFlag & SOME_GALAXY_STORAGE_FLAG_ILLUST_ITEM) {
				illustItemNum++;
			}
		}

		return illustItemNum;
	}

	bool hasIllustItemAndTicoCoinInStage(const char *pStageName) {
		GameDataSomeGalaxyStorage* storage = GameDataFunction::makeSomeGalaxyStorage(pStageName);
		u8 mask = SOME_GALAXY_STORAGE_FLAG_TICO_COIN | SOME_GALAXY_STORAGE_FLAG_ILLUST_ITEM;
		return (storage->mFlag & mask) == mask;
	}
}
