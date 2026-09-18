#include "neo/MapObj/MarblePlanet.h"
#include "Game/LiveActor/ModelObj.h"
#include "Game/Util/ActionSoundUtil.h"
#include "Game/Util/ActorAnimUtil.h"
#include "Game/Util/ActorInitUtil.h"
#include "Game/Util/ActorMovementUtil.h"
#include "Game/Util/ActorSensorUtil.h"
#include "Game/Util/ActorSwitchUtil.h"
#include "Game/Util/EffectUtil.h"
#include "Game/Util/JMapUtil.h"
#include "Game/Util/LiveActorUtil.h"
#include "Game/Util/MathUtil.h"
#include "Game/Util/MtxUtil.h"
#include "Game/Util/ObjUtil.h"
#include "Game/Util/SoundUtil.h"

namespace neo {
    MarblePlanet::MarblePlanet(const char *pName)
        : LiveActor(pName)
    {
        mMarblePlanetCore = nullptr;
        mMarblePlanetElectrons = nullptr;
        mWaterMelonCollision = nullptr;
        mNumElectrons = 3;
        mRemainingElectrons = 3;
    }

    void MarblePlanet::init(const JMapInfoIter &rIter) {
        MR::processInitFunction(this, "MarblePlanet", false);
        MR::initDefaultPos(this, rIter);
        MR::connectToScenePlanet(this);

        initHitSensor(1);
        HitSensor *pCoreSensor = MR::addHitSensorEnemy(this, "Core", 16, 50.0f, TVec3f(0.0f, 0.0f, 0.0f));
        MR::initCollisionParts(this, "MarblePlanet", pCoreSensor, nullptr);
        mWaterMelonCollision = MR::createCollisionPartsFromLiveActor(this, "WaterMelon", pCoreSensor, MR::CollisionScaleType_Default);

        initEffectKeeper(0, nullptr, false);
        initSound(4, nullptr, false, TVec3f(0.0f, 0.0f, 0.0f));

        MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
        MR::setClippingFar(this, -1.0f);
        MR::useStageSwitchWriteDead(this, rIter);

        MR::getJMapInfoArg0NoInit(rIter, &mNumElectrons);
        mRemainingElectrons = mNumElectrons;
        initCoreAndElectron();
        MR::declarePowerStar(this);

        initNerve(NrvMarblePlanetWait, 0);
        makeActorAppeared();
    }

    void MarblePlanet::kill() {
        if (MR::isValidSwitchDead(this)) {
            MR::onSwitchDead(this);
        }

        mMarblePlanetCore->kill();
        LiveActor::kill();
    }

    void MarblePlanet::startClipped() {
        mMarblePlanetCore->startClipped();

        for (int i = 0; i < mNumElectrons; i++) {
            mMarblePlanetElectrons[i]->startClipped();
        }

        LiveActor::startClipped();
    }

    void MarblePlanet::endClipped() {
        mMarblePlanetCore->endClipped();

        for (int i = 0; i < mNumElectrons; i++) {
            mMarblePlanetElectrons[i]->endClipped();
        }

        LiveActor::endClipped();
    }

    bool MarblePlanet::receiveMsgEnemyAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
        if (isNerve(NrvMarblePlanetWait)) {
            setNerve(NrvMarblePlanetScaleUpCore);
            return true;
        }

        return false;
    }

    void MarblePlanet::initCoreAndElectron() {
        // Initialize core
        mMarblePlanetCore = MR::createModelObjMapObj("ビー玉惑星コア", "MarblePlanetCore", (MtxPtr)getBaseMtx());
        MR::invalidateClipping(mMarblePlanetCore);
        MR::startBckAndSetFrameAndStop(mMarblePlanetCore, "MarblePlanetCore", 0.0f);
        MR::startBtkAndSetFrameAndStop(mMarblePlanetCore, "MarblePlanetCore", 0.0f);

        // Initialize electrons
        mMarblePlanetElectrons = new MarblePlanetElectron * [mNumElectrons];
        TVec3f frontVec;
        MR::calcFrontVec(&frontVec, this);

        for (s32 i = 0; i < mNumElectrons; i++) {
            if (i != 0) {
                TVec3f upVec;
                MR::calcUpVec(&upVec, this);
                MR::rotateVecDegree(&frontVec, upVec, 360.0f / mNumElectrons);
            }

            // Calculate translation and rotation
            TVec3f translation;
            JMAVECScaleAdd(frontVec, mTranslation, translation, 1000.0f);

            TVec3f rotation;
            rotation.setAll(i * (360.0f / mNumElectrons));

            // Construct electron
            MarblePlanetElectron* electron = new MarblePlanetElectron(this, translation, rotation, "ビー玉惑星電子");
            electron->initWithoutIter();
            mMarblePlanetElectrons[i] = electron;
        }
    }

    void MarblePlanet::exeScaleUpCore() {
        if (MR::isFirstStep(this)) {
            MR::tryRumblePadMiddle(this, 0);
            MR::shakeCameraNormal();

            mRemainingElectrons--;

            if (mRemainingElectrons == 0) {
                MR::emitEffect(this, "Break");
                MR::startActionSound(this, "OjMarbleHitCore3", -1, -1, -1);
                MR::startSystemSE("SE_SY_MARBLE_HIT_CORE_3", -1, -1);
            }
            else if (mRemainingElectrons == 1) {
                MR::emitEffect(mMarblePlanetCore, "Smoke6f");
                MR::startActionSound(this, "OjMarbleHitCore2", -1, -1, -1);
                MR::startSystemSE("SE_SY_MARBLE_HIT_CORE_2", -1, -1);
            }
            else {
                MR::emitEffect(mMarblePlanetCore, "Smoke3f");
                MR::startActionSound(this, "OjMarbleHitCore1", -1, -1, -1);
                MR::startSystemSE("SE_SY_MARBLE_HIT_CORE_1", -1, -1);
            }

            if (mRemainingElectrons <= 0) {
                setNerve(NrvMarblePlanetBreakCore);
                return;
            }
            else {
                f32 frame = MR::getBckFrameMax(mMarblePlanetCore);
                frame *= mNumElectrons - mRemainingElectrons;
                frame /= mNumElectrons;

                MR::setBckFrameAndStop(mMarblePlanetCore, frame);
                MR::setBtkFrameAndStop(mMarblePlanetCore, frame);
            }
        }

        f32 rate = MR::calcNerveRate(this, 30);
        f32 scale = MR::getScaleWithReactionValueZeroToOne(rate, 0.5f, -0.5f);
        scale = MR::getLinerValue(scale, 1.3f, 1.0f, 1.0f);
        mMarblePlanetCore->mScale.setAll(scale);

        if (MR::isStep(this, 30)) {
            setNerve(NrvMarblePlanetWait);
        }
    }

    void MarblePlanet::exeBreakCore() {
        if (MR::isFirstStep(this)) {
            MR::setBckFrameAndStop(mMarblePlanetCore, MR::getBckFrameMax(mMarblePlanetCore));
            MR::setBtkFrameAndStop(mMarblePlanetCore, MR::getBtkFrameMax(mMarblePlanetCore));
        }
        else if (MR::isStep(this, 1)) {
            MR::tryRumblePadStrong(this, 0);
            MR::shakeCameraStrong();
            MR::requestAppearPowerStar(this, mTranslation);

            MR::hideModel(this);
            MR::invalidateCollisionParts(this);
            MR::validateCollisionParts(mWaterMelonCollision);

            if (MR::isValidSwitchDead(this)) {
                MR::onSwitchDead(this);
            }
        }
    }

    // -----------------------------------------------------------------------------------------------------------------

    MarblePlanetElectron::MarblePlanetElectron(LiveActor *pHost, const TVec3f &rTranslation, const TVec3f &rRotation, const char *pName)
        : LiveActor(pName)
    {
        mHost = pHost;
        mShadow = nullptr;
        mFrontVec.set(0.0f, 0.0f, 1.0f);

        mTranslation.set(rTranslation);
        mRotation.set(rRotation);
    }

    void MarblePlanetElectron::init(const JMapInfoIter &rIter) {
        MR::processInitFunction(this, "MarblePlanetElectron", false);
        MR::connectToScenePlanet(this);

        initHitSensor(1);
        MR::addHitSensorEnemy(this, "Body", 16, 500.0f, TVec3f(0.0f, 0.0f, 0.0f));

        initBinder(200.0f, 0.0f, 0);
        initEffectKeeper(1, nullptr, false);
        MR::addEffectHitNormal(this, nullptr);
        initSound(4, "MarblePlanet", false, TVec3f(0.0f, 0.0f, 0.0f));
        MR::invalidateClipping(this);

        mShadow = new MarblePlanetElectronShadow(this, mHost->mTranslation, "電子影");
        mShadow->initWithoutIter();

        MR::calcGravity(this);
        mGravity.negate();

        TPos3f baseMtx;
        MR::makeMtxUpNoSupportPos(&baseMtx, mGravity, mTranslation);
        MR::setBaseTRMtx(this, baseMtx);
        MR::calcFrontVec(&mFrontVec, this);

        MR::startAction(this, "MarblePlanetElectron");
        initNerve(NrvMarblePlanetElectronMove, 0);
        makeActorAppeared();
    }

    void MarblePlanetElectron::control() {
        MR::calcGravity(this);
        mGravity.negate();

        if (isNerve(NrvMarblePlanetElectronMove)) {
            MR::restrictVelocity(this, 30.0f);
        }
    }

    void MarblePlanetElectron::attackSensor(HitSensor *pSender, HitSensor *pReceiver) {
        if (MR::isSensorEnemy(pReceiver)) {
            if (MR::sendMsgEnemyAttack(pReceiver, pSender)) {
                mShadow->kill();
                kill();
            }
            else if (MR::isNear(this, pReceiver->mActor->mTranslation, 440.0f) && MR::sendMsgPush(pReceiver, pSender)) {
                if (MR::calcDistanceToPlayer(mHost) < 2000.f) {
                    MR::tryRumblePadVeryWeak(this, 0);
                }

                if (!MR::isEffectValid(this, "HitMarkNormal")) {
                    MR::emitEffectHitBetweenSensors(this, pSender, pReceiver, 0.0f, nullptr);
                }

                MR::killVelocityToTarget(this, pReceiver->mActor->mTranslation);
            }
        }
    }

    bool MarblePlanetElectron::receiveMsgPush(HitSensor *pSender, HitSensor *pReceiver) {
        if (MR::isSensorEnemy(pSender)) {
            crashElectron(pSender);
            return true;
        }

        return false;
    }

    bool MarblePlanetElectron::receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
        if (!isNerve(NrvMarblePlanetElectronAttack) && MR::isMsgPlayerHipDrop(msg)) {
            setNerve(NrvMarblePlanetElectronAttack);
            return true;
        }

        return false;
    }

    void MarblePlanetElectron::exeMove() {
        MR::turnDirectionToGround(this, &mFrontVec);
        MR::attenuateVelocity(this, 0.99f);
        f32 mag = PSVECMag((Vec*)&mVelocity);

        if (mag < 13.0f) {
            mag = 13.0f;
        }

        mVelocity.scale(mag, mFrontVec);
        MR::startActionSound(this, "OjLvMarbleRotate", -1, -1, -1);
    }

    void MarblePlanetElectron::exeAttack() {
        if (MR::isFirstStep(this)) {
            MR::startActionSound(this, "OjMarbleFlip", -1, -1, -1);
        }

        TVec3f dirToCore = mHost->mTranslation - mTranslation;
        MR::normalize(&dirToCore);
        mVelocity.scale(40.0f, dirToCore);
    }

    void MarblePlanetElectron::crashElectron(HitSensor *pCollidedSensor) {
        TVec3f dirToCollider = pCollidedSensor->mActor->mTranslation - mTranslation;
        MR::normalize(&dirToCollider);
        JMAVECScaleAdd(dirToCollider, mVelocity, mVelocity, -5.0f);
        MR::normalize(&mFrontVec, mVelocity);
        mVelocity.scale(1.2f);

        MR::startActionSound(this, "OjMarbleHitEach", -1, -1, -1);
    }

    // -----------------------------------------------------------------------------------------------------------------

    MarblePlanetElectronShadow::MarblePlanetElectronShadow(LiveActor *pHost, const TVec3f &rMarblePlanetPos, const char *pName)
        : LiveActor(pName)
    {
        mHost = pHost;
        mMarblePlanetPos = &rMarblePlanetPos;
    }

    void MarblePlanetElectronShadow::init(const JMapInfoIter &rIter) {
        MR::processInitFunction(this, "MarblePlanetElectronShadow", false);
        MR::connectToScenePlanet(this);
        MR::invalidateClipping(this);
        makeActorAppeared();
    }

    void MarblePlanetElectronShadow::calcAndSetBaseMtx() {
        mTranslation.set(*mMarblePlanetPos);

        TVec3f upVec = mHost->mTranslation - *mMarblePlanetPos;
        MR::normalize(&upVec);

        TPos3f baseMtx;
        MR::makeMtxUpNoSupportPos(&baseMtx, upVec, *mMarblePlanetPos);
        MR::setBaseTRMtx(this, baseMtx);
    }

    // -----------------------------------------------------------------------------------------------------------------

    namespace NrvMarblePlanet {
        NrvWait(NrvWait::sInstance);
        NrvScaleUpCore(NrvScaleUpCore::sInstance);
        NrvBreakCore(NrvBreakCore::sInstance);

        void NrvWait::execute(Spine *pSpine) const {}

        void NrvScaleUpCore::execute(Spine *pSpine) const {
            MarblePlanet* pActor = (MarblePlanet*)pSpine->mExecutor;
            pActor->exeScaleUpCore();
        }

        void NrvBreakCore::execute(Spine *pSpine) const {
            MarblePlanet* pActor = (MarblePlanet*)pSpine->mExecutor;
            pActor->exeBreakCore();
        }
    }

    // -----------------------------------------------------------------------------------------------------------------

    namespace NrvMarblePlanetElectron {
        NrvMove(NrvMove::sInstance);
        NrvAttack(NrvAttack::sInstance);

        void NrvMove::execute(Spine *pSpine) const {
            MarblePlanetElectron* pActor = (MarblePlanetElectron*)pSpine->mExecutor;
            pActor->exeMove();
        }

        void NrvAttack::execute(Spine *pSpine) const {
            MarblePlanetElectron* pActor = (MarblePlanetElectron*)pSpine->mExecutor;
            pActor->exeAttack();
        }
    }
}
