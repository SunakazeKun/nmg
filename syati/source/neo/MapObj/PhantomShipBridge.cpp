#include "neo/MapObj/PhantomShipBridge.h"
#include "Game/Util/ActionSoundUtil.h"
#include "Game/Util/ActorAnimUtil.h"
#include "Game/Util/ActorInitUtil.h"
#include "Game/Util/ActorSensorUtil.h"
#include "Game/Util/ActorSwitchUtil.h"
#include "Game/Util/JMapUtil.h"
#include "Game/Util/JointUtil.h"
#include "Game/Util/LiveActorUtil.h"
#include "Game/Util/ObjUtil.h"
#include "Game/Util/PlayerUtil.h"

namespace neo {
    PhantomShipBridge::PhantomShipBridge(const char *pName)
        : LiveActor(pName)
    {
        mMoveCollision = nullptr;
    }

    void PhantomShipBridge::init(const JMapInfoIter &rIter) {
        MR::processInitFunction(this, rIter, false);
        MR::initDefaultPos(this, rIter);

        const char *objectName;
        MR::getObjectName(&objectName, rIter);

        MR::connectToSceneCollisionMapObj(this);
        initNerve(NrvPhantomShipBridgeWait, 0);

        initEffectKeeper(0, nullptr, false);

        initHitSensor(2);
        HitSensor *bodySensor = MR::addBodyMessageSensorMapObj(this);
        HitSensor *moveSensor = MR::addHitSensor(this, "Move", ATYPE_MAP_OBJ_MOVE_COLLISION, 0, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));

        MR::initCollisionParts(this, objectName, getSensor("Body"), nullptr);
        mMoveCollision = MR::createCollisionPartsFromLiveActor(this, "Move", moveSensor, MR::getJointMtx(this, "Move"), MR::CollisionScaleType_Default);
        MR::validateCollisionParts(mMoveCollision);

        initSound(4, objectName, nullptr, TVec3f(0.0f, 0.0f, 0.0f));
        MR::setClippingTypeSphere(this, 2000.0f);

        makeActorAppeared();

        if (MR::useStageSwitchReadB(this, rIter)) {
            MR::listenStageSwitchOnB(this, MR::Functor(this, &startMoveB));
        }

        if (MR::useStageSwitchReadA(this, rIter)) {
            MR::listenStageSwitchOnA(this, MR::Functor(this, &startMoveA));
        }
        else {
            setStateMoveA();
        }
    }

    void PhantomShipBridge::calcAnim() {
        LiveActor::calcAnim();
        mMoveCollision->setMtx();
    }

    void PhantomShipBridge::startMoveA() {
        setNerve(NrvPhantomShipBridgeMoveA);
    }

    void PhantomShipBridge::startMoveB() {
        setNerve(NrvPhantomShipBridgeMoveB);
    }

    void PhantomShipBridge::setStateMoveA() {
        MR::startAction(this, "MoveA");
        MR::setBckFrameEndAndStop(this);

        MR::calcAnimDirect(this);

        TPos3f mtx;
        JMath::gekko_ps_copy12(&mtx, mMoveCollision->_0);
        mMoveCollision->makeEqualScale(mtx);
        mMoveCollision->resetAllMtxPrivate(mtx);
        mMoveCollision->_CE = true;

        setNerve(NrvPhantomShipBridgeWait);
    }

    void PhantomShipBridge::exeMoveA() {
        if (MR::isFirstStep(this)) {
            MR::invalidateClipping(this);

            if (!MR::isPlayerDead()) {
                MR::startActionSound(this, "Riddle", -1, -1, -1);
            }

            MR::startAction(this, "MoveA");
        }

        if (MR::isLessStep(this, 55)) {
            MR::startActionSound(this, "Move", -1, -1, -1);
        }

        if (MR::isStep(this, 55)) {
            MR::startActionSound(this, "Land", -1, -1, -1);
            MR::shakeCameraNormal();
            MR::validateClipping(this);
        }
    }

    void PhantomShipBridge::exeMoveB() {
        if (MR::isFirstStep(this)) {
            MR::invalidateClipping(this);
            MR::startActionSound(this, "Fall", -1, -1, -1);
            MR::startAction(this, "MoveB");
        }

        if (MR::isLessStep(this, 55)) {
            MR::startActionSound(this, "Move", -1, -1, -1);
        }

        if (MR::isStep(this, 55)) {
            MR::startActionSound(this, "Land", -1, -1, -1);
            MR::shakeCameraNormal();
            MR::validateClipping(this);
        }
    }

    // -----------------------------------------------------------------------------------------------------------------

    namespace NrvPhantomShipBridge {
        NrvWait(NrvWait::sInstance);
        NrvMoveA(NrvMoveA::sInstance);
        NrvMoveB(NrvMoveB::sInstance);

        void NrvWait::execute(Spine *pSpine) const {}

        void NrvMoveA::execute(Spine *pSpine) const {
            PhantomShipBridge* pActor = (PhantomShipBridge*)pSpine->mExecutor;
            pActor->exeMoveA();
        }

        void NrvMoveB::execute(Spine *pSpine) const {
            PhantomShipBridge* pActor = (PhantomShipBridge*)pSpine->mExecutor;
            pActor->exeMoveB();
        }
    }
}
