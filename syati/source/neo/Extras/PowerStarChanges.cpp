#include "neo/Extras/GreenStarNoBootOut.h"
#include "neo/System/InStageSave.h"


namespace neo {
	void GreenStarExeGot(PowerStar *pStar) {
		pStar->processWait(30.0f);

		if (MR::isFirstStep(pStar)) {
			MR::tryRumblePadMiddle(pStar, 0);
			MR::startActionSound(pStar, "SyStarGet", -1, -1, -1);
		}

		if (MR::isStep(pStar, 20) && !MR::isPlayerDead()) {
			MR::tryStartDemo(pStar, "GreenStarDemo");
			GameDataFunction::setPowerStar(MR::getCurrentStageName(), pStar->mPowerStarId);
			neo::performSaveInStage();
		}

		if (MR::isGreaterEqualStep(pStar, 20) && !GameSequenceFunction::isActiveSaveDataHandleSequence()) {
			if (MR::isDemoActive("GreenStarDemo")) {
				MR::endDemo(pStar, "GreenStarDemo");
			}

			pStar->setNerve(NrvGreenStarGotDisappear);
		}
	}

	void GreenStarExeGotDisappear(PowerStar *pStar) {
		pStar->processWait(30.0f);

		f32 xz = MR::calcNerveValue(pStar, 20, 1.0f, 0.0001f);
		f32 y = MR::calcNerveValue(pStar, 20, 1.0f, 5.0f);

		pStar->mScale.set(xz, y, xz);

		if (MR::isGreaterEqualStep(pStar, 20)) {
			pStar->makeActorDead();
		}
	}

	namespace NrvGreenStar {
		void NrvGot::execute(Spine *pSpine) const {
			PowerStar* pActor = (PowerStar*)pSpine->mExecutor;
			GreenStarExeGot(pActor);
		}

		void NrvGotDisappear::execute(Spine *pSpine) const {
			PowerStar* pActor = (PowerStar*)pSpine->mExecutor;
			GreenStarExeGotDisappear(pActor);
		}

		NrvGot(NrvGot::sInstance);
		NrvGotDisappear(NrvGotDisappear::sInstance);
	}
}

namespace {
	bool tryDoStarGet(PowerStar* pStar) {
		if (MR::isPowerStarGreenInCurrentStage(pStar->mPowerStarId)) {
			const char* stageName = MR::getCurrentStageName();
			s32 scenarioNo = MR::getCurrentScenarioNo();
			GalaxyStatusAccessor gsa = MR::makeGalaxyStatusAccessor(stageName);

			s32 greenStarsScenarioTotal = 0;
			s32 greenStarsScenarioOwned = 0;

			for (int i = 0; i < gsa.getPowerStarNum(); i++) {
				s32 starId = i + 1;

				if (gsa.isGreenStar(starId) && gsa.isExistPowerStarAtScenario(scenarioNo, starId)) {
					greenStarsScenarioTotal++;

					if (gsa.hasPowerStar(starId)) {
						greenStarsScenarioOwned++;
					}
				}
			}

			if ((greenStarsScenarioTotal - greenStarsScenarioOwned) > 1) {
				if (pStar->isEndAppearDemo()) {
					MR::invalidateHitSensors(pStar);
					pStar->setNerve(NrvGreenStarGot);
				}
				return true;
			}
		}
		
		return false;
	}

	bool canStarMsgRushTakeOver(PowerStar* pStar, u32 msg) {
		return MR::isMsgIsRushTakeOver(msg) ? !tryDoStarGet(pStar) : false;
	}

	kmWrite32(0x802E0000, 0x60000000);
	kmCall(0x802E0008, canStarMsgRushTakeOver);


	bool doGetStar(PowerStar* pStar, const Nerve* pNerve) {
		return tryDoStarGet(pStar) ? false : pStar->isNerve(pNerve);
	}

	kmCall(0x802E0034, doGetStar);
}
