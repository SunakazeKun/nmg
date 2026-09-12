#include "neo/MapObj/AstroCore.h"

namespace neo {
	AstroCore::AstroCore(const char *pName) : LiveActor(pName) {
		mStarSpawned = false;
	}

	void AstroCore::init(const JMapInfoIter &rIter) {
		MR::processInitFunction(this, rIter, false);

		if (MR::useStageSwitchReadA(this, rIter)) {
			MR::declarePowerStar(this);
			MR::listenStageSwitchOnA(this, MR::Functor(this, &appearPowerStar));
		}

		makeActorAppeared();
	}

	void AstroCore::attackSensor(HitSensor *pSender, HitSensor *pReceiver) {
		MR::sendMsgPush(pReceiver, pSender);
	}

	void AstroCore::appearPowerStar() {
		if (!mStarSpawned) {
			MR::requestAppearPowerStar(this);
			mStarSpawned = true;
		}
	}
}
