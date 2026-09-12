#include "neo/MapObj/StringSpiderAttachPoint.h"

namespace neo {
	StringSpiderAttachPoint::StringSpiderAttachPoint(const char *pName) : LiveActor(pName) {

	}

	void StringSpiderAttachPoint::init(const JMapInfoIter &rIter) {
		MR::processInitFunction(this, rIter, false);
		MR::declareStarPiece(this, 3);

		mStringSpider = new StringSpider("StringSpiderForCocoon");
		mStringSpider->init(rIter);

		initClipping(rIter);

		makeActorAppeared();
	}

	void StringSpiderAttachPoint::initClipping(const JMapInfoIter &rIter) {
		// Calculate clipping radius
		f32 stringLength = 300.0f;
		f32 clippingRadius = 1000.0f;

		MR::getJMapInfoArg0NoInit(rIter, &stringLength);
		MR::getJMapInfoArg1NoInit(rIter, &clippingRadius);

		if (stringLength < 0.0f) {
			stringLength = 0.0f;
		}

		if (clippingRadius < 0.0f) {
			clippingRadius = 0.0f;
		}

		clippingRadius += stringLength;

		// Calculate clipping position
		TVec3f upVec;
		MR::calcUpVec(&upVec, this);

		mClippingCenter = mTranslation + upVec * (stringLength * -0.5f);

		// Apply clipping
		MR::setClippingTypeSphere(this, clippingRadius, &mClippingCenter);
		MR::setClippingTypeSphere(mStringSpider, clippingRadius, &mClippingCenter);
	}

	void StringSpiderAttachPoint::control() {
		if (MR::isDead(mStringSpider)) {
			MR::emitEffect(this, "Disappear");
			MR::appearStarPiece(this, mTranslation, 3, 10.0f, 20.0f, false);
			kill();
		}
	}
}
