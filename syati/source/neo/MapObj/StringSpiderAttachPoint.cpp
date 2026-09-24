#include "neo/MapObj/StringSpiderAttachPoint.h"
#include "Game/Enemy/StringSpider.h"
#include "Game/Util/ActorInitUtil.h"
#include "Game/Util/ActorMovementUtil.h"
#include "Game/Util/ActorSensorUtil.h"
#include "Game/Util/EffectUtil.h"
#include "Game/Util/JMapUtil.h"
#include "Game/Util/LiveActorUtil.h"
#include "Game/Util/ObjUtil.h"

namespace neo {
    StringSpiderAttachPoint::StringSpiderAttachPoint(const char *pName)
        : LiveActor(pName)
    {
        mNumStarPiece = 3;
        mStringSpider = nullptr;
        mClippingCenter.zero();
    }

    void StringSpiderAttachPoint::init(const JMapInfoIter &rIter) {
        MR::processInitFunction(this, rIter, false);
        
        MR::getJMapInfoArg2NoInit(rIter, &mNumStarPiece);
        MR::declareStarPiece(this, mNumStarPiece);

        mStringSpider = new StringSpider("ストリングスパイダー");
        mStringSpider->init(rIter);

        initClipping(rIter);

        makeActorAppeared();
    }

    void StringSpiderAttachPoint::initClipping(const JMapInfoIter &rIter) {
        f32 stringLength = 300.0f;
        f32 clippingRadius = 1000.0f;

        MR::getJMapInfoArg0NoInit(rIter, &stringLength);
        MR::getJMapInfoArg1NoInit(rIter, &clippingRadius);

        clippingRadius += stringLength;

        TVec3f upVec;
        MR::calcUpVec(&upVec, this);

        mClippingCenter = mTranslation + upVec * (stringLength * -0.5f);

        MR::setClippingTypeSphere(this, clippingRadius, &mClippingCenter);
        MR::setClippingTypeSphere(mStringSpider, clippingRadius, &mClippingCenter);
    }

    void StringSpiderAttachPoint::control() {
        if (MR::isDead(mStringSpider)) {
            if (mNumStarPiece > 0) {
                MR::appearStarPiece(this, mTranslation, mNumStarPiece, 10.0f, 20.0f, false);
            }
            
            MR::emitEffect(this, "Disappear");
            kill();
        }
    }

    void StringSpiderAttachPoint::attackSensor(HitSensor *pSender, HitSensor *pReceiver) {
        if (MR::getSensorHost(pReceiver) == mStringSpider) {
            return;
        }

        if (MR::isSensorPlayerOrYoshiOrRide(pReceiver) || MR::isSensorEnemy(pReceiver)) {
            MR::sendMsgPush(pReceiver, pSender);
        }
    }

    bool StringSpiderAttachPoint::receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
        return MR::isMsgStarPieceReflect(msg);
    }
}
