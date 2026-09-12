#include "neo/MapObj/IllustItem.h"
#include "neo/Screen/IllustItemCounter.h"
#include "neo/System/IllustItemAccess.h"

namespace {
	/*********************************************************************************************/
	/* Galaxy progress accounts for Stamp                                                        */
	/*********************************************************************************************/
#if defined(PAL) || defined(USA) || defined(JPN)
	kmCall(0x804D1E18, neo::hasIllustItemAndTicoCoinInStage); // GalaxyStatusAccessor::isCompletedNoGreen
	kmCall(0x804D1EA8, neo::hasIllustItemAndTicoCoinInStage); // GalaxyStatusAcessor::isCompletedWithGreen
	kmCall(0x804E1A94, neo::hasIllustItemAndTicoCoinInStage); // Layout stuff
#elif defined(TWN) || defined(KOR)
	kmCall(0x804D1EA8, neo::hasIllustItemAndTicoCoinInStage); // GalaxyStatusAccessor::isCompletedNoGreen
	kmCall(0x804D1F38, neo::hasIllustItemAndTicoCoinInStage); // GalaxyStatusAcessor::isCompletedWithGreen
	kmCall(0x804E1B24, neo::hasIllustItemAndTicoCoinInStage); // Layout stuff
#endif

	bool isResultNotifyTicoCoinOrIllustItem() {
		PlayResultInStageHolder* playResult = GameSequenceFunction::getGameSequenceInGame()->getPlayResultInStageHolder();
		return playResult->mNotifyTicoCoin || playResult->_63 & IN_TEMP_STAGE_ILLUST_ITEM_FLAG_NOTIFY;
	}

#if defined(PAL) || defined(USA) || defined(JPN)
	kmCall(0x80496798, isResultNotifyTicoCoinOrIllustItem);
#elif defined(TWN) || defined(KOR)
	kmCall(0x80496808, isResultNotifyTicoCoinOrIllustItem);
#endif


	/*********************************************************************************************/
	/* Store IllustItem after collecting a Power Star                                            */
	/*********************************************************************************************/
	void afterStageSaveCollectionItems(PlayResultInStageHolder *pPlayResult) {
		GameSequenceFunction::collectTicoCoinIfSaved();
		neo::tryIllustItemStoreAndNotifyInTempStage();

		if (pPlayResult->mSavedTicoCoin) {
			GameDataFunction::onGalaxyFlagTicoCoin(pPlayResult->mStageName);
		}
		if (pPlayResult->_63 & IN_TEMP_STAGE_ILLUST_ITEM_FLAG_SAVED) {
			neo::onGalaxyFlagIllustItem(pPlayResult->mStageName);
		}
	}

#if defined(PAL) || defined(USA) || defined(JPN)
	kmWrite32(0x804D8730, 0x7FC3F378); // r3 = PlayResultInStageHolder*
	kmCall(0x804D8734, afterStageSaveCollectionItems);
	kmWrite32(0x804D8738, 0x48000010); // skip garbage
#elif defined(TWN) || defined(KOR)
	kmWrite32(0x804D87C0, 0x7FC3F378); // r3 = PlayResultInStageHolder*
	kmCall(0x804D87C4, afterStageSaveCollectionItems);
	kmWrite32(0x804D87C8, 0x48000010); // skip garbage
#endif

	// Reset temporary collected IllustItem flags when swapping PlayResultInStageHolder data
#if defined(PAL) || defined(USA) || defined(JPN)
	kmWrite32(0x804C8DC8, 0xB3FE0062); // clears fields 0x62 and 0x63
#elif defined(TWN) || defined(KOR)
	kmWrite32(0x804C8E58, 0xB3FE0062); // clears fields 0x62 and 0x63
#endif


	/*********************************************************************************************/
	/* Store IllustItem after collecting a Checkpoint                                            */
	/*********************************************************************************************/
	void saveRestartObjCollectionItems() {
		GameSequenceFunction::collectTicoCoinIfSaved();
		neo::tryIllustItemStoreAndNotifyInTempStage();
	}

	kmCall(0x802F1954, saveRestartObjCollectionItems);


	/*****************************************************************************************************************/
	/* Clears Stamp & Star Bits after dying                                                                          */
	/*****************************************************************************************************************/
	__asm void afterMissClearCollectionItems() {
		// Clear IllustItemFlagCollected, preserved IllustItemFlagSaved & IllustItemFlagNotify
		lbz r0, 0x63(r3)
			li r4, 0x6
			and r0, r0, r4
			stb r0, 0x63(r3)

			// Clear collected Star Bits & 1-Up threshold
			li r0, 0
			stw r0, 0x4C(r3) // Star Bits
			stw r0, 0x50(r3) // 1-Up threshold

			beqlr
			stw r5, 0x84(r3)
			blr
	}

#if defined(PAL) || defined(USA) || defined(JPN)
	kmBranch(0x804C92C8, afterMissClearCollectionItems);
#elif defined(TWN) || defined(KOR)
	kmBranch(0x804C9358, afterMissClearCollectionItems);
#endif


	/*********************************************************************************************/
	/* Unlock Comet Observatory after collecting at least 7 Stamps                               */
	/*********************************************************************************************/
	bool tryOnOpenAstroGalaxy() {
		return neo::calcIllustItemNum() >= 7;
	}

#if defined(PAL) || defined(USA) || defined(JPN)
	kmCall(0x804D8878, tryOnOpenAstroGalaxy);
#elif defined(TWN) || defined(KOR)
	kmCall(0x804D8908, tryOnOpenAstroGalaxy);
#endif


	/*********************************************************************************************/
	/* Include counter layout                                                                    */
	/*********************************************************************************************/
	neo::IllustItemCounter* createIllustItemCounter() {
		neo::IllustItemCounter* counter = new neo::IllustItemCounter();
		counter->initWithoutIter();
		return counter;
	}

	void appearIllustItemCounter(neo::IllustItemCounter *pCounter) {
		pCounter->forceAppear();
	}

	void disappearIllustItemCounter(neo::IllustItemCounter *pCounter) {
		pCounter->disappear();
	}

	bool isWaitIllustItemCounter(neo::IllustItemCounter *pCounter) {
		return pCounter->isWait();
	}

	// CounterLayoutController creates IllustItemCounter instead of PlayerLeft
	kmCall(0x804657F0, createIllustItemCounter);
	kmWrite32(0x804657F4, 0x48000014); // skip garbage

	// Control visibility
	kmCall(0x80465B2C, appearIllustItemCounter);
	kmCall(0x80465BAC, disappearIllustItemCounter);
	kmCall(0x80465CC8, isWaitIllustItemCounter);

	// Make CometMedalCounter show up in all scenarios
	kmWrite32(0x80464E3C, 0x4800000C);


	/*********************************************************************************************/
	/* Patch existing layouts                                                                    */
	/*********************************************************************************************/
	// All Stamps count message
	const wchar_t cIllustItemCounterMessage[] = {
		0x000E, 0x0003, 0x004E, 0x0002, 0x004E, // Stamp icon
		0x000E, 0x0003, 0x0010, 0x0002, 0x0010, // x icon
		0x000E, 0x0006, 0x0001, 0x0008, 0x0000, 0x0000, 0x0000, 0x0000, // Number arg
		0x0000 // eof
	};


	// PauseMenu
	void showOrHideIllustItemOnPauseMenu(LayoutActor *pActor) {
		if (MR::isStageNoPauseMenuStars()) {
			MR::hidePaneRecursive(pActor, "ShaIllust");
		}
		else {
			s32 illustItemCount = neo::calcIllustItemNum();

			if (neo::isIllustItemInTempStageGet()) {
				illustItemCount += 1;
			}

			MR::setTextBoxMessageRecursive(pActor, "ShaIllust", cIllustItemCounterMessage);
			MR::setTextBoxArgNumberRecursive(pActor, "ShaIllust", illustItemCount, 0);
		}
	}

#if defined(PAL) || defined(USA) || defined(JPN)
	kmWrite32(0x80487140, 0x7F63DB78); // mr r3, r27
	kmCall(0x80487144, showOrHideIllustItemOnPauseMenu);
	kmWrite32(0x80487148, 0x48000010); // skip garbage
#elif defined(TWN) || defined(KOR)
	kmWrite32(0x80487150, 0x7F63DB78); // mr r3, r27
	kmCall(0x80487154, showOrHideIllustItemOnPauseMenu);
	kmWrite32(0x80487158, 0x48000010); // skip garbage
#endif


	// GalaxyInfo
	LayoutActor* initIllustIconsGalaxyInfo(LayoutActor *pActor) {
		MR::setTextBoxMessageRecursive(pActor, "TxtIllust", L"\u007E");
		MR::setTextBoxMessageRecursive(pActor, "TxtIllustHole", L"\u007F");
		return pActor;
	}

#if defined(PAL) || defined(USA) || defined(JPN)
	kmBranch(0x804A9548, initIllustIconsGalaxyInfo);
#elif defined(TWN) || defined(KOR)
	kmBranch(0x804A95B8, initIllustIconsGalaxyInfo);
#endif

	void showOrHideTicoCoinAndIllustItemOnGalaxyInfo(const char *pStageName, LayoutActor *pActor) {
		if (GameDataFunction::isOnGalaxyFlagTicoCoin(pStageName)) {
			MR::showPaneRecursive(pActor, "TxtMedal");
		}
		else {
			MR::hidePaneRecursive(pActor, "TxtMedal");
		}

		if (neo::isOnGalaxyFlagIllustItem(pStageName)) {
			MR::showPaneRecursive(pActor, "TxtIllust");
		}
		else {
			MR::hidePaneRecursive(pActor, "TxtIllust");
		}
	}

#if defined(PAL) || defined(USA) || defined(JPN)
	kmWrite32(0x804A9650, 0x7F64DB78);
	kmCall(0x804A9654, showOrHideTicoCoinAndIllustItemOnGalaxyInfo);
	kmWrite32(0x804A9658, 0x48000020); // skip garbage
#elif defined(TWN) || defined(KOR)
	kmWrite32(0x804A96C0, 0x7F64DB78);
	kmCall(0x804A96C4, showOrHideTicoCoinAndIllustItemOnGalaxyInfo);
	kmWrite32(0x804A96C8, 0x48000020); // skip garbage
#endif


	// AllStarList
	void showTicoCoinAndIllustItemOnAllStarList(LayoutActor *pActor, GalaxyStatusAccessor *pAccessors, s32 numGalaxies) {
		wchar_t strMedals[210], strIllusts[210];
		wchar_t* pMedals = strMedals;
		wchar_t* pIllusts = strIllusts;

		for (int i = 0; i < numGalaxies; i++) {
			if (pAccessors[i].isOpened()) {
				const char *galaxyName = pAccessors[i].getName();
				pMedals = MR::addPictureFontCode(pMedals, GameDataFunction::isOnGalaxyFlagTicoCoin(galaxyName) ? 0x6A : 0x6F);
				pIllusts = MR::addPictureFontCode(pIllusts, neo::isOnGalaxyFlagIllustItem(galaxyName) ? 0x7E : 0x7F);
			}

			if (i < numGalaxies - 1) {
				pMedals = MR::addNewLine(pMedals);
				pIllusts = MR::addNewLine(pIllusts);
			}
		}

		*pMedals = 0;
		*pIllusts = 0;
		MR::setTextBoxMessageRecursive(pActor, "Medal", strMedals);
		MR::setTextBoxMessageRecursive(pActor, "Illust", strIllusts);
	}

	kmWrite32(0x8045E4E4, 0x7F63DB78);
	kmWrite32(0x8045E4E8, 0x388107E0);
	kmWrite32(0x8045E4EC, 0x7FC5F378);
	kmCall(0x8045E4F0, showTicoCoinAndIllustItemOnAllStarList);
	kmWrite32(0x8045E4F4, 0x48000098); // skip garbage
}
