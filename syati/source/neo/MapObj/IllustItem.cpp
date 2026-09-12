#include "neo/MapObj/IllustItem.h"
#include "neo/System/IllustItemAccess.h"
#include "neo/Screen/ExtInformationObserver.h"

namespace neo {
	IllustItem::IllustItem(const char *pName) : LiveActor(pName) {
		mConnector = NULL;
		mRotateY = 0.0f;
		mCollected = false;
		mNearPointsCount = 0;

		PSMTXCopy(MR::tmpMtxRotXDeg(35.0f), mRotateXMtx);
	}

	void IllustItem::init(const JMapInfoIter &rIter) {
		MR::processInitFunction(this, rIter, "IllustItem", false);

		mCollected = neo::isOnGalaxyFlagIllustItem(MR::getCurrentStageName());
		MR::startBva(this, mCollected ? "Empty" : "Normal");

		f32 shadowLength = -1.0;
		s32 poseType = 0;
		MR::getJMapInfoArg0NoInit(rIter, &shadowLength);
		MR::getJMapInfoArg1NoInit(rIter, &poseType);
		MR::getJMapInfoArg2NoInit(rIter, &mNearPointsCount);

		for (int i = 0; i < mNearPointsCount; i++) {
			char linkPosName[32];
			sprintf(linkPosName, "IllustItemPos%02d", i);
			MR::tryRegisterNamePosLinkObj(this, rIter, linkPosName);
		}

		if (shadowLength >= 0.0f) {
			MR::setShadowDropLength(this, NULL, shadowLength);
		}

		switch (poseType) {
		default:
			MR::calcGravity(this);
			break;
		case 1:
			MR::calcUpVec(&mGravity, this);
			mGravity.negate();
			break;
		case 2:
			MR::calcGravity(this);
			mConnector = new MapObjConnector(this);
			break;
		}

		initNerve(NrvIllustItemShow, 0);

		MR::tryRegisterDemoCast(this, rIter);

		if (MR::useStageSwitchSyncAppear(this, rIter)) {
			MR::invalidateClipping(this);
		}

		if (neo::isIllustItemInTempStageGet()) {
			makeActorDead();
		}
	}

	void IllustItem::initAfterPlacement() {
		if (mConnector) {
			mConnector->attachToUnder();
		}
	}

	void IllustItem::appear() {
		if (mNearPointsCount > 0) {
			f32 closestDistance = 999999999999.0f;
			f32 potentialDistance;
			TVec3f potentialPosition;
			TVec3f potentialRotation;

			for (int i = 0; i < mNearPointsCount; i++) {
				char linkPosName[32];
				sprintf(linkPosName, "IllustItemPos%02d", i);

				if (MR::tryFindLinkNamePos(this, linkPosName, &potentialPosition, &potentialRotation)) {
					potentialDistance = MR::calcDistanceToPlayer(potentialPosition);

					if (potentialDistance < closestDistance) {
						closestDistance = potentialDistance;
						mTranslation = potentialPosition;
						mRotation = potentialRotation;
					}
				}
			}
		}

		LiveActor::appear();
	}

	void IllustItem::control() {
		mRotateY = fmod(mRotateY + 2.0f, 360.0f);
	}

	void IllustItem::calcAndSetBaseMtx() {
		TPos3f baseMtx;
		MR::makeMtxUpNoSupportPos(&baseMtx, -mGravity, mTranslation);
		PSMTXConcat(baseMtx, MR::tmpMtxRotYDeg(mRotateY), baseMtx);
		PSMTXConcat(baseMtx, mRotateXMtx, baseMtx);
		MR::setBaseTRMtx(this, baseMtx);

		if (mConnector) {
			mConnector->connect();
		}
	}

	bool IllustItem::receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
		return MR::isMsgStarPieceReflect(msg);
	}

	bool IllustItem::receiveOtherMsg(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
		if (MR::isMsgItemGet(msg) && isNerve(NrvIllustItemShow)) {
			doGetItem();
			return true;
		}

		if (MR::isMsgItemHide(msg) && isNerve(NrvIllustItemShow)) {
			setNerve(NrvIllustItemHide);
			return true;
		}

		if (MR::isMsgItemShow(msg) && isNerve(NrvIllustItemHide)) {
			setNerve(NrvIllustItemShow);
			return true;
		}

		return false;
	}

	void IllustItem::doGetItem() {
		if (MR::isStageScoreAttack()) {
			ScoreAttackFunction::score1000Points(this);
		}

		neo::collectIllustItemInTempStage();

		MR::startActionSound(this, "Get", -1, -1, -1);
		MR::emitEffect(this, "Get");

		if (MR::isGreaterStep(this, 2)) {
			neo::explainIllustItemIfAtFirst(this);
			makeActorDead();
		}
	}

	void IllustItem::exeShow() {
		if (MR::isFirstStep(this)) {
			MR::showModel(this);
			MR::validateShadowAll(this);
			MR::validateHitSensors(this);
			MR::emitEffect(this, mCollected ? "GlowEmpty" : "Glow");
		}

		MR::startActionSound(this, "Rotate", -1, -1, -1);
	}

	void IllustItem::exeHide() {
		if (MR::isFirstStep(this)) {
			MR::hideModel(this);
			MR::invalidateShadowAll(this);
			MR::invalidateHitSensors(this);
			MR::forceDeleteEffectAll(this);
		}
	}

	namespace NrvIllustItem {
		void NrvShow::execute(Spine *pSpine) const {
			IllustItem *pActor = (IllustItem*)pSpine->mExecutor;
			pActor->exeShow();
		}

		void NrvHide::execute(Spine *pSpine) const {
			IllustItem *pActor = (IllustItem*)pSpine->mExecutor;
			pActor->exeHide();
		}

		NrvShow(NrvShow::sInstance);
		NrvHide(NrvHide::sInstance);
	}
}