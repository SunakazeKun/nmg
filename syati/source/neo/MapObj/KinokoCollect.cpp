#include "neo/MapObj/KinokoCollect.h"
#include "Game/MapObj/MapObjConnector.h"
#include "neo/System/DaredevilModeAccess.h"
#include "Game/NPC/ScoreAttackCounter.h"
#include "Game/Util/ActionSoundUtil.h"
#include "Game/Util/ActorAnimUtil.h"
#include "Game/Util/ActorInitUtil.h"
#include "Game/Util/ActorMovementUtil.h"
#include "Game/Util/ActorSensorUtil.h"
#include "Game/Util/ActorShadowUtil.h"
#include "Game/Util/ActorSwitchUtil.h"
#include "Game/Util/DemoUtil.h"
#include "Game/Util/EffectUtil.h"
#include "Game/Util/EventUtil.h"
#include "Game/Util/JMapUtil.h"
#include "Game/Util/LiveActorUtil.h"
#include "Game/Util/MathUtil.h"
#include "Game/Util/MtxUtil.h"
#include "Game/Util/ObjUtil.h"
#include "Game/Util/PlayerUtil.h"
#include "Game/Util/SceneUtil.h"
#include "Game/Util/SupportPlayUtil.h"

namespace neo {
    KinokoCollect::KinokoCollect(const char *pName, KinokoCollectKind kind)
        : LiveActor(pName)
    {
        mKind = kind;
        mConnector = nullptr;
        mAppearType = 0;
        mPoseType = 0;
        mShadowType = 0;
        mRotateY = 0.0f;
        mThrowSpeed = 15.0f;
        mThrowAngle = 15.0f;
        mThrowSrc.zero();
    }

    namespace {
        const char* cKinokoCollectModels[] = {
            "KinokoTreasure",
            "KinokoStar",
            "LifeUpHeart"
        };
    }

    void KinokoCollect::init(const JMapInfoIter &rIter) {
        MR::processInitFunction(this, rIter, cKinokoCollectModels[mKind], false);

        MR::calcFrontVec(&mFrontVec, this);
        MR::calcUpVec(&mGravity, this);
        mGravity.negate();

        // Initialize from BCSV data
        f32 shadowLength = -1.0;

        MR::getJMapInfoArg0NoInit(rIter, &mAppearType);
        MR::getJMapInfoArg1NoInit(rIter, &mPoseType);
        MR::getJMapInfoArg2NoInit(rIter, &mShadowType);
        MR::getJMapInfoArg3NoInit(rIter, &shadowLength);
        MR::getJMapInfoArg4NoInit(rIter, &mThrowSpeed);
        MR::getJMapInfoArg5NoInit(rIter, &mThrowAngle);
        MR::tryRegisterDemoCast(this, rIter);

        // Process BCSV data
        if (mPoseType & KINOKO_COLLECT_POSE_CONNECTOR) {
            mConnector = new MapObjConnector(this);
            MR::onCalcShadowAll(this);
        }

        if (shadowLength >= 0.0f) {
            MR::setShadowDropLength(this, nullptr, shadowLength);
        }

        if (mAppearType == KINOKO_COLLECT_APPEAR_THROW || (mPoseType & KINOKO_COLLECT_POSE_GRAVITY)) {
            MR::onCalcGravity(this);
        }

        switch (mShadowType) {
        case 0:
            MR::onCalcShadowDropGravityOneTime(this, nullptr);
            break;
        case 1:
            MR::onCalcShadowDropGravity(this, nullptr);
            break;
        case 2:
            MR::invalidateShadowAll(this);
            break;
        }

        initNerve(NrvKinokoCollectWait, 0);

        // Hearts shouldn't appear during Daredevil Comets
        if (mKind == Heart && neo::isDaredevilModeOrComet()) {
            makeActorDead();
        }
        else {
            MR::useStageSwitchSyncAppear(this, rIter);
        }
    }

    void KinokoCollect::initAfterPlacement() {
        if (mConnector) {
            mConnector->attachToUnder();
        }
    }

    void KinokoCollect::appear() {
        LiveActor::appear();

        mRotateY = 0.0f;
        MR::validateClipping(this);
        MR::validateHitSensors(this);
        MR::onBind(this);

        if (mAppearType == KINOKO_COLLECT_APPEAR_THROW || (mPoseType & KINOKO_COLLECT_POSE_GRAVITY)) {
            MR::onCalcGravity(this);
        }

        switch (mAppearType) {
        case KINOKO_COLLECT_APPEAR_THROW:
            setNerve(NrvKinokoCollectAppearThrow);
            break;
        default:
            setNerve(NrvKinokoCollectAppearFix);
            break;
        }
    }

    void KinokoCollect::calcAndSetBaseMtx() {
        TPos3f baseMtx;

        if (mConnector) {
            mConnector->connect();
            PSMTXCopy((MtxPtr)getBaseMtx(), baseMtx);
        }
        else {
            TVec3f upVec;

            if (MR::isNearZero(mGravity, 0.001f)) {
                upVec.set(0.0f, 1.0f, 0.0f);
            }
            else {
                upVec = -mGravity;
            }

            MR::makeMtxUpFrontPos(&baseMtx, upVec, mFrontVec, mTranslation);
        }

        if ((mPoseType & KINOKO_COLLECT_POSE_ROTATE) && isNerve(NrvKinokoCollectWait)) {
            PSMTXConcat(baseMtx, (MtxPtr)MR::tmpMtxRotYDeg(mRotateY), baseMtx);
        }

        MR::setBaseTRMtx(this, baseMtx);
    }

    bool KinokoCollect::receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
        return MR::isMsgStarPieceReflect(msg);
    }

    bool KinokoCollect::receiveOtherMsg(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
        if (MR::isDead(this)) {
            return false;
        }

        if (MR::isMsgItemGet(msg) && isCollectable()) {
            doGetItem();
            return true;
        }

        if (MR::isMsgItemStartMove(msg) && isCollectable()) {
            setNerve(NrvKinokoCollectBound);
            return true;
        }

        if (MR::isMsgItemSupportTicoDrop(msg) && isNerve(NrvKinokoCollectBound)) {
            setNerve(NrvKinokoCollectThrow);
            return true;
        }

        if (MR::isMsgItemSupportTicoDemoShow(msg)) {
            MR::emitEffect(this, "Glow");
            return false;
        }

        if (MR::isMsgItemSupportTicoDemoHide(msg)) {
            MR::forceDeleteEffect(this, "Glow");
            return false;
        }

        return false;
    }

    void KinokoCollect::doGetItem() {
        switch (mKind) {
        case Treasure:
            if (!MR::isStageScoreAttack()) {
                MR::incCoin(25, this);
            }
            else {
                ScoreAttackFunction::score1000Points(this);
            }
            break;
        case Star:
            if (!MR::isStageScoreAttack()) {
                MR::addStarPiece(25);
            }
            else {
                ScoreAttackFunction::score1000Points(this);
            }
            break;
        case Heart:
            if (!MR::isStageScoreAttack()) {
                s32 playerLife = MR::getPlayerLife();

                if (playerLife == 3 || playerLife == 6) {
                    MR::incCoin(3, this);
                    MR::startActionSound(this, "GetCoin", -1, -1, -1);
                }
            }
            else {
                ScoreAttackFunction::score300Points(this);
            }

            MR::incPlayerLife(1);
            break;
        }

        MR::startActionSound(this, "Get", -1, -1, -1);
        MR::emitEffect(this, "Get");
        makeActorDead();
    }

    void KinokoCollect::exeAppearFix() {
        if (MR::isFirstStep(this)) {
            MR::startAction(this, "Appear");
        }

        if (MR::isActionEnd(this)) {
            setNerve(NrvKinokoCollectWait);
        }
    }

    void KinokoCollect::exeAppearThrow() {
        if (MR::isFirstStep(this)) {
            MR::startAction(this, "Appear");

            TVec3f upVec;
            TVec3f sideVec;
            TVec3f throwDir;

            upVec = -mGravity;
            MR::calcSideVec(&sideVec, this);

            if (MR::isNearZero(upVec, 0.001f)) {
                MR::calcUpVec(&upVec, this);
            }

            MR::rotateVecDegree(&throwDir, upVec, sideVec, mThrowAngle);
            mVelocity = throwDir * mThrowSpeed;
        }

        MR::addVelocityToGravity(this, 1.0f);
        MR::attenuateVelocity(this, 0.995f);

        MR::attachSupportTicoToTarget(this);

        if (MR::isOnGround(this)) {
            setNerve(NrvKinokoCollectWait);
        }
    }

    void KinokoCollect::exeWait() {
        if (MR::isFirstStep(this)) {
            MR::emitEffect(this, "Glow");
        }

        if ((mPoseType & KINOKO_COLLECT_POSE_ROTATE)) {
            mRotateY = fmod(mRotateY + 2.0f, 360.0f);
        }

        MR::attachSupportTicoToTarget(this);
    }

    void KinokoCollect::exeThrow() {
        if (MR::isFirstStep(this)) {
            MR::invalidateClipping(this);
            MR::invalidateHitSensors(this);
            MR::offBind(this);
            MR::offCalcGravity(this);

            mGravity = *MR::getPlayerGravity();
            mThrowSrc = mTranslation;
        }

        MR::vecBlend(&mTranslation, mThrowSrc, *MR::getPlayerCenterPos(), MR::calcNerveEaseOutRate(this, 20));
        f32 offY = 250.0f * MR::calcNerveParabolaValue(this, 30);
        mTranslation += -mGravity * offY;

        if (MR::isGreaterStep(this, 30)) {
            doGetItem();
        }
    }

    bool KinokoCollect::isCollectable() const {
        return isNerve(NrvKinokoCollectWait) || isNerve(NrvKinokoCollectAppearThrow);
    }

    // ----------------------------------------------------------------------------------------------------------------

    NameObj* createKinokoTreasure(const char *pName) {
        return new KinokoCollect(pName, Treasure);
    }

    NameObj* createKinokoStar(const char *pName) {
        return new KinokoCollect(pName, Star);
    }

    NameObj* createLifeUpHeart(const char *pName) {
        return new KinokoCollect(pName, Heart);
    }

    // ----------------------------------------------------------------------------------------------------------------

    namespace NrvKinokoCollect {
        NrvAppearFix(NrvAppearFix::sInstance);
        NrvAppearThrow(NrvAppearThrow::sInstance);
        NrvWait(NrvWait::sInstance);
        NrvBound(NrvBound::sInstance);
        NrvThrow(NrvThrow::sInstance);

        void NrvAppearFix::execute(Spine *pSpine) const {
            KinokoCollect *pActor = (KinokoCollect*)pSpine->mExecutor;
            pActor->exeAppearFix();
        }

        void NrvAppearThrow::execute(Spine *pSpine) const {
            KinokoCollect *pActor = (KinokoCollect*)pSpine->mExecutor;
            pActor->exeAppearThrow();
        }

        void NrvAppearThrow::executeOnEnd(Spine *pSpine) const {
            KinokoCollect *pActor = (KinokoCollect*)pSpine->mExecutor;
            MR::zeroVelocity(pActor);
        }

        void NrvWait::execute(Spine *pSpine) const {
            KinokoCollect *pActor = (KinokoCollect*)pSpine->mExecutor;
            pActor->exeWait();
        }

        void NrvBound::execute(Spine *pSpine) const {

        }

        void NrvThrow::execute(Spine *pSpine) const {
            KinokoCollect *pActor = (KinokoCollect*)pSpine->mExecutor;
            pActor->exeThrow();
        }
    }
}
