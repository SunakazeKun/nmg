#include "neo/Effect/EffectObjLine.h"

namespace neo {
	EffectObjLine::EffectObjLine(const char *pName) : LiveActor(pName) {
		mGroupName = NULL;
		mPointsArray = NULL;
		mPointsCount = 0;
	}

	void EffectObjLine::init(const JMapInfoIter &rIter) {
		MR::initDefaultPos(this, rIter);
		MR::connectToSceneMapObjMovement(this);

		MR::getObjectName(&mGroupName, rIter);
		initRailRider(rIter);
		initPoints(rIter);
		initClipping(rIter);

		MR::useStageSwitchAwake(this, rIter);
		MR::useStageSwitchSyncAppear(this, rIter);
	}

	void EffectObjLine::initPoints(const JMapInfoIter &rIter) {
		s32 middleCount = 0;
		MR::getJMapInfoArg0NoInit(rIter, &middleCount);

		if (middleCount < 0) {
			middleCount = 0;
		}

		mPointsCount = 2 + middleCount;
		mPointsArray = new EffectObjLinePoint*[mPointsCount];

		f32 totalLength = mRailRider->getTotalLength();
		f32 dividedLength = totalLength / (mPointsCount - 1);

		for (s32 i = 0; i < mPointsCount; i++) {
			EffectObjLinePoint::PointType pointType;

			if (i == 0) {
				pointType = EffectObjLinePoint::StartPoint;
			}
			else if (i == (mPointsCount - 1)) {
				pointType = EffectObjLinePoint::EndPoint;
			}
			else {
				pointType = EffectObjLinePoint::MiddlePoint;
			}

			EffectObjLinePoint* point = new EffectObjLinePoint(this, pointType, i * dividedLength);
			point->initWithoutIter();
			mPointsArray[i] = point;
		}
	}

	void EffectObjLine::initClipping(const JMapInfoIter &rIter) {
		TVec3f dirStartToEnd = mRailRider->mEndPos - mRailRider->mStartPos;
		f32 distStartEnd = PSVECMag(dirStartToEnd);

		f32 clippingRadius = distStartEnd * 0.5f + 500.0f;
		mTranslation = mRailRider->mStartPos + dirStartToEnd * 0.5f;

		MR::setClippingTypeSphere(this, clippingRadius);
		MR::setClippingFar50m(this);
	}

	void EffectObjLine::makeActorAppeared() {
		LiveActor::makeActorAppeared();

		for (s32 i = 0; i < mPointsCount; i++) {
			mPointsArray[i]->makeActorAppeared();
		}
	}

	void EffectObjLine::makeActorDead() {
		LiveActor::makeActorDead();

		for (s32 i = 0; i < mPointsCount; i++) {
			mPointsArray[i]->makeActorDead();
		}
	}

	void EffectObjLine::startClipped() {
		LiveActor::startClipped();

		for (s32 i = 0; i < mPointsCount; i++) {
			mPointsArray[i]->startClipped();
		}
	}

	void EffectObjLine::endClipped() {
		LiveActor::endClipped();

		for (s32 i = 0; i < mPointsCount; i++) {
			mPointsArray[i]->endClipped();
		}
	}

	// ----------------------------------------------------------------------------------------------------------------

	EffectObjLinePoint::EffectObjLinePoint(EffectObjLine *pHost, PointType pointType, f32 pathCoord) : LiveActor("EffectObjLinePoint") {
		mHost = pHost;
		mPointType = pointType;
		mPathCoord = pathCoord;
		mEffectMtx.identity();
	}

	void EffectObjLinePoint::init(const JMapInfoIter &rIter) {
		MR::connectToSceneMapObjMovement(this);
		MR::invalidateClipping(this);

		MR::calcRailPosAtCoord(&mTranslation, mHost, mPathCoord);

		initEffectKeeper(0, mHost->mGroupName, false);
		initSound(4, mHost->mGroupName, NULL, TVec3f(0.0f, 0.0f, 0.0f));

		MR::calcGravity(this);
		MR::makeMtxUpNoSupportPos(&mEffectMtx, -mGravity, mTranslation);
		MR::setEffectHostMtx(this, getTriggerName(), mEffectMtx);

		makeActorAppeared();
	}

	void EffectObjLinePoint::control() {
		const char* triggerName = getTriggerName();
		MR::emitEffect(this, triggerName);
		MR::startActionSound(this, triggerName, -1, -1, -1);
	}

	const char* EffectObjLinePoint::getTriggerName() const {
		switch (mPointType) {
		case StartPoint:
			return "Start";
		case EndPoint:
			return "End";
		case MiddlePoint:
			return "Middle";
		}

		return NULL;
	}
};
