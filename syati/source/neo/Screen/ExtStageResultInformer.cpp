#include "neo/Screen/ExtStageResultInformer.h"
#include "neo/System/IllustItemAccess.h"
#include "neo/core.h"

namespace {
	/*****************************************************************************************************************/
	/* Prevent Ghost Appeared message                                                                                */
	/*****************************************************************************************************************/
#if defined(PAL) || defined(USA) || defined(JPN)
	kmWrite32(0x80496144, LI_R3_0);
#elif defined(TWN) || defined(KOR)
	kmWrite32(0x804961B4, LI_R3_0);
#endif


	/*****************************************************************************************************************/
	/* Hide Stamp counter except when needed                                                                         */
	/*****************************************************************************************************************/
	void hideCounterPanesExt(LayoutActor *pActor, const char *pOrgPane) {
		MR::hidePane(pActor, pOrgPane);
		MR::hidePane(pActor, "TotalStamp");
	}

#if defined(PAL) || defined(USA) || defined(JPN)
	kmCall(0x80496680, hideCounterPanesExt);
#elif defined(TWN) || defined(KOR)
	kmCall(0x804966F0, hideCounterPanesExt);
#endif


	/*****************************************************************************************************************/
	/* Decide new nerves                                                                                             */
	/*****************************************************************************************************************/
	const Nerve* getNextNerveExt(StageResultInformer *pActor) {
		// Check sequence
		bool checkMedal = false;
		bool checkStamp = false;
		bool checkComplete = false;

		if (pActor->isNerve(&NrvStageResultInformer::StageResultInformerNrvDisappearGetPowerStar::sInstance)) {
			checkMedal = true;
		}

		if (checkMedal || pActor->isNerve(&NrvStageResultInformer::StageResultInformerNrvDisappearGetCometMedal::sInstance)) {
			checkStamp = true;
		}

		if (checkStamp || pActor->isNerve(NrvStageResultInformerExtDisappearGetStamp)) {
			checkComplete = true;
		}

		// Return appropriate nerves
		if (checkMedal && GameSequenceFunction::isResultNotifyTicoCoin()) {
			return &NrvStageResultInformer::StageResultInformerNrvAppearGetCometMedal::sInstance;
		}

		if (checkStamp && neo::isIllustItemInTempStageNotify()) {
			return NrvStageResultInformerExtAppearGetStamp;
		}

		if (checkComplete && pActor->isCompleted()) {
			return &NrvStageResultInformer::StageResultInformerNrvComplete::sInstance;
		}

		// Remaining sequence handled by original function
		return pActor->getNextNerve();
	}

	bool isPreviousNerveDisappearStampOrCometMedal(StageResultInformer *pActor, const Nerve *pOrgNerve) {
		return pActor->isNerve(pOrgNerve) || pActor->isNerve(NrvStageResultInformerExtDisappearGetStamp);
	}

	void endResultPaneExt(StageResultInformer *pActor, const char *pPaneName, const char *pAnimName, u32 unk, const Nerve *pNextNerve) {
		if (pNextNerve != NrvStageResultInformerExtAppearGetStamp) {
			MR::startPaneAnim(pActor, pPaneName, pAnimName, unk);
			pActor->_44 = false;
		}
	}

#if defined(PAL) || defined(USA) || defined(JPN)
	kmCall(0x8049630C, getNextNerveExt);
	kmCall(0x80495F88, isPreviousNerveDisappearStampOrCometMedal);
	kmWrite32(0x80496750, 0x7C070378); // mr r7, r0
	kmCall(0x80496754, endResultPaneExt);
	kmWrite32(0x80496758, NOP);
#elif defined(TWN) || defined(KOR)
	kmCall(0x8049637C, getNextNerveExt);
	kmCall(0x80495FF8, isPreviousNerveDisappearStampOrCometMedal);
	kmWrite32(0x804967C0, 0x7C070378); // mr r7, r0
	kmCall(0x804967C4, endResultPaneExt);
	kmWrite32(0x804967C8, NOP);
#endif


	/*****************************************************************************************************************/
	/* Nerve functionality                                                                                           */
	/*****************************************************************************************************************/
	void exeStageResultInformerExtAppearGetStamp(StageResultInformer *pActor) {
		if (MR::isFirstStep(pActor)) {
			MR::showLayout(pActor);
			pActor->hideCounterPanes();
			MR::showPane(pActor, "TotalStamp");
			MR::startPaneAnim(pActor, "PlayResult", "Appear", 0);
			MR::setTextBoxGameMessageRecursive(pActor, "Result", "System_ResultGetStamp");
			MR::startAnim(pActor, "Line", 1);
			MR::setAnimFrameAndStopAdjustTextHeight(pActor, "Result", 1);
			MR::setTextBoxNumberRecursive(pActor, "ShaStampNum", neo::calcIllustItemNum());
			MR::startSubBGM("BGM_JG_HIGH_SCORE", false);
		}

		MR::setNerveAtPaneAnimStopped(pActor, "PlayResult", NrvStageResultInformerExtWaitBeforeCountUpStamp, 0);
	}

	void exeStageResultInformerExtCountUpStamp(StageResultInformer *pActor) {
		if (MR::isFirstStep(pActor)) {
			if (MR::testSystemPadTriggerDecide()) {
				pActor->_38 = true;
			}
		}

		MR::setNerveAtAnimStopped(pActor, NrvStageResultInformerExtDisplayGetStamp, 0);
	}

	void exeStageResultInformerExtDisplayGetStamp(StageResultInformer *pActor) {
		if (MR::isFirstStep(pActor)) {
			pActor->mIconAButton->openWithoutMessage();
		}

		pActor->tryWaitSystemPadTriggerDecide(NrvStageResultInformerExtDisappearGetStamp);
	}

	void exeStageResultInformerExtDisappearGetStamp(StageResultInformer *pActor) {
		if (MR::isFirstStep(pActor)) {
			pActor->endResultPane();
			pActor->mIconAButton->term();
		}

		if (MR::isPaneAnimStopped(pActor, "PlayResult", 0)) {
			pActor->decideNextNerve();
		}
	}


	/*****************************************************************************************************************/
	/* Choose correct Power Star information message                                                                 */
	/*****************************************************************************************************************/
	const char* getResultStarMessageName(GalaxyStatusAccessor &gsa, s32 scenarioId) {
		if (gsa.isOwnBronzeStar(scenarioId)) {
			return "System_ResultGetBronzeStar";
		}

		if (gsa.isGrandStar(scenarioId)) {
			return "System_ResultGetGrandStar";
		}

		if (gsa.isHiddenStar(scenarioId)) {
			return "System_ResultGetHiddenStar";
		}

		if (gsa.isGreenStar(scenarioId)) {
			return "System_ResultGetGreenStar";
		}

		const char* type;
		gsa.mScenarioData->getScenarioString("PowerStarType", scenarioId, &type);

		if (MR::isEqualString(type, "Red")) {
			return "System_ResultGetRedStar";
		}

		return "System_ResultGetPowerStar";
	}

#if defined(PAL) || defined(USA) || defined(JPN)
	kmCall(0x80494F24, getResultStarMessageName);
	kmWrite32(0x80494F28, 0x7C651B78);
	kmWrite32(0x80494F2C, 0x48000060);
#elif defined(TWN) || defined(KOR)
	kmCall(0x80494F94, getResultStarMessageName);
	kmWrite32(0x80494F98, 0x7C651B78);
	kmWrite32(0x80494F9C, 0x48000060);
#endif
}

namespace neo {
	namespace NrvStageResultInformerExt {
		void NrvAppearGetStamp::execute(Spine *pSpine) const {
			StageResultInformer *pActor = (StageResultInformer*)pSpine->mExecutor;
			exeStageResultInformerExtAppearGetStamp(pActor);
		}

		void NrvWaitBeforeCountUpStamp::execute(Spine *pSpine) const {
			StageResultInformer *pActor = (StageResultInformer*)pSpine->mExecutor;
			pActor->tryWaitIntervalBeforeKeyWait(NrvStageResultInformerExtCountUpStamp, 45);
		}

		void NrvCountUpStamp::execute(Spine *pSpine) const {
			StageResultInformer *pActor = (StageResultInformer*)pSpine->mExecutor;
			exeStageResultInformerExtCountUpStamp(pActor);
		}

		void NrvDisplayGetStamp::execute(Spine *pSpine) const {
			StageResultInformer *pActor = (StageResultInformer*)pSpine->mExecutor;
			exeStageResultInformerExtDisplayGetStamp(pActor);
		}

		void NrvDisappearGetStamp::execute(Spine *pSpine) const {
			StageResultInformer *pActor = (StageResultInformer*)pSpine->mExecutor;
			exeStageResultInformerExtDisappearGetStamp(pActor);
		}

		NrvAppearGetStamp(NrvAppearGetStamp::sInstance);
		NrvWaitBeforeCountUpStamp(NrvWaitBeforeCountUpStamp::sInstance);
		NrvCountUpStamp(NrvCountUpStamp::sInstance);
		NrvDisplayGetStamp(NrvDisplayGetStamp::sInstance);
		NrvDisappearGetStamp(NrvDisappearGetStamp::sInstance);
	}
}
