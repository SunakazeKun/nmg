#include "neo/Screen/ExtInformationObserver.h"
#include "neo/core.h"

namespace {
	const InformationObserverExtInfo cInformationObserverExtInfos[] = {
		{ "InformationObserverStamp", false, NULL, NULL }
	};

	const s32 cInformationObserverExtInfosCount = sizeof(cInformationObserverExtInfos) / sizeof(InformationObserverExtInfo);

	// ----------------------------------------------------------------------------------------------------------------

	const Nerve* getNerveForInformationObserverExt(s32 extType) {
		switch (extType) {
		case INFORMATION_OBSERVER_EXT_ILLUST_ITEM:
			return NrvInformationObserverExtDispIllustItem;
		default:
			return NULL;
		}
	}

	typedef bool (*BoolFunc)(void);

	void setupInformationObserverExt(InformationObserver *pActor, s32 extType, LiveActor *pHost, bool pauseTimeKeepHostDemo) {
		pActor->mHost = pHost;
		pActor->mPauseTimeKeepHostDemo = false;

		const Nerve* pNerve = ::getNerveForInformationObserverExt(extType);

		// Too lazy to document the symbol
		if (((BoolFunc)0x800210B0)()) {
			if (pauseTimeKeepHostDemo) {
				pActor->mPauseTimeKeepHostDemo = true;
				MR::pauseTimeKeepDemo(pActor->mHost);
			}

			pActor->setNerve(pNerve);
		}
		else {
			MR::requestStartDemoWithoutCinemaFrame(pActor, "‰o•\\Ž¦", pNerve, &NrvInformationObserver::InformationObserverNrvWait::sInstance);
		}

		pActor->makeActorAppeared();
	}

	// ----------------------------------------------------------------------------------------------------------------

	void exeInformationObserverDispExt(InformationObserver *pActor, s32 extType) {
		InformationObserverExtInfo info = cInformationObserverExtInfos[extType];

		if (MR::isFirstStep(pActor)) {
			MR::appearInformationMessageUp(info.mMessageName, true);
			MR::disableCloseInformationMessage();

			if (info.mDispBgmName) {
				MR::stopSubBGM(0);
				MR::startSubBGM(info.mDispBgmName, false);
			}

			pActor->mAButtonDelay = 30;
		}

		if (pActor->mAButtonDelay >= 0) {
			pActor->mAButtonDelay--;
		}

		if (MR::testCorePadTriggerA(0) && pActor->mAButtonDelay < 0) {
			if (info.mAfterBgmName) {
				MR::startSubBGM(info.mAfterBgmName, false);
			}

			pActor->setNerve(&NrvInformationObserver::InformationObserverNrvEnd::sInstance);
		}
	}

	// ----------------------------------------------------------------------------------------------------------------

	bool isInformationObserverActiveCheckNerve(InformationObserver *pActor) {
		return !pActor->isNerve(&NrvInformationObserver::InformationObserverNrvWait::sInstance);
	}

	kmCall(0x80479CD8, isInformationObserverActiveCheckNerve);
}

namespace neo {
	bool explainIllustItemIfAtFirst(LiveActor *pHost) {
		if (!GameDataFunction::isOnGameEventFlag(GAME_EVENT_FLAG_EXPLAIN_ILLUST_ITEM_AT_FIRST)) {
			InformationObserver *pObserver = (InformationObserver*)MR::getSceneObjHolder()->getObj(SCENE_OBJ_INFORMATION_OBSERVER);
			::setupInformationObserverExt(pObserver, INFORMATION_OBSERVER_EXT_ILLUST_ITEM, pHost, true);

			GameDataFunction::onGameEventFlag(GAME_EVENT_FLAG_EXPLAIN_ILLUST_ITEM_AT_FIRST);
			return true;
		}

		return false;
	}

	namespace NrvInformationObserverExt {
		void NrvDispIllustItem::execute(Spine *pSpine) const {
			InformationObserver *pActor = (InformationObserver*)pSpine->mExecutor;
			::exeInformationObserverDispExt(pActor, INFORMATION_OBSERVER_EXT_ILLUST_ITEM);
		}

		NrvDispIllustItem(NrvDispIllustItem::sInstance);
	}
}
