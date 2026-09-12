#include "neo/MapObj/SpiderCocoon.h"

namespace neo {
	SpiderCocoon::SpiderCocoon(const char *pName) : LiveActor(pName) {

	}

	void SpiderCocoon::init(const JMapInfoIter &rIter) {
		MR::processInitFunction(this, rIter, "SpiderCocoon", false);

		if (MR::isValidSwitchB(this)) {
			MR::listenStageSwitchOnB(this, MR::Functor(this, &openCocoon));
		}

		initNerve(NrvSpiderCocoonWait, 0);
		makeActorAppeared();
	}

	void SpiderCocoon::attackSensor(HitSensor *pSender, HitSensor *pReceiver) {
		if (MR::isSensorPlayerOrYoshiOrRide(pReceiver) || MR::isSensorEnemy(pReceiver)) {
			MR::sendMsgPush(pReceiver, pSender);
		}
	}

	bool SpiderCocoon::receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
		if (MR::isMsgPlayerHipDrop(msg) || MR::isMsgPlayerTrample(msg)) {
			if (isNerve(NrvSpiderCocoonWait) ||
				isNerve(NrvSpiderCocoonShake) && MR::isGreaterEqualStep(this, 30))
			{
				setNerve(NrvSpiderCocoonShake);
				MR::sendMsgAwayJump(pSender, pReceiver);
			}

			return true;
		}

		if (MR::isMsgPlayerHitAll(msg)) {
			if (isNerve(NrvSpiderCocoonWait) ||
				isNerve(NrvSpiderCocoonShake) && MR::isGreaterEqualStep(this, 60))
			{
				setNerve(NrvSpiderCocoonShake);
			}

			return true;
		}

		return false;
	}

	void SpiderCocoon::openCocoon() {
		setNerve(NrvSpiderCocoonOpen);
	}

	void SpiderCocoon::exeShake() {
		if (MR::isFirstStep(this)) {
			MR::startAction(this, "Shake");
		}

		if (MR::isActionEnd(this)) {
			setNerve(NrvSpiderCocoonWait);
		}
	}

	void SpiderCocoon::exeOpen() {
		if (MR::isFirstStep(this)) {
			MR::startAction(this, "Open");
			MR::setBckRate(this, 2.0f);
			MR::setBrkRate(this, 2.0f);

			if (MR::isValidSwitchDead(this)) {
				MR::onSwitchDead(this);
			}
		}

		if (MR::isStep(this, 30)) {
			MR::invalidateHitSensors(this);
		}

		if (MR::isActionEnd(this)) {
			kill();
		}
	}

	namespace NrvSpiderCocoon {
		void NrvWait::execute(Spine *pSpine) const {}

		void NrvShake::execute(Spine *pSpine) const {
			SpiderCocoon *pActor = (SpiderCocoon*)pSpine->mExecutor;
			pActor->exeShake();
		}

		void NrvOpen::execute(Spine *pSpine) const {
			SpiderCocoon *pActor = (SpiderCocoon*)pSpine->mExecutor;
			pActor->exeOpen();
		}

		NrvWait(NrvWait::sInstance);
		NrvShake(NrvShake::sInstance);
		NrvOpen(NrvOpen::sInstance);
	}
}
