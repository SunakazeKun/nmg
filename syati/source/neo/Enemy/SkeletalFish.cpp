#include "neo/Enemy/SkeletalFish.h"
#include "Game/Enemy/AnimScaleController.h"
#include "Game/Util/ActionSoundUtil.h"
#include "Game/Util/ActorAnimUtil.h"
#include "Game/Util/ActorInitUtil.h"
#include "Game/Util/ActorMovementUtil.h"
#include "Game/Util/ActorSensorUtil.h"
#include "Game/Util/ActorSwitchUtil.h"
#include "Game/Util/EffectUtil.h"
#include "Game/Util/JMapUtil.h"
#include "Game/Util/JointUtil.h"
#include "Game/Util/LiveActorUtil.h"
#include "Game/Util/MathUtil.h"
#include "Game/Util/MtxUtil.h"
#include "Game/Util/ObjUtil.h"
#include "Game/Util/RailUtil.h"
#include "Game/Util/SoundUtil.h"
#include "Game/Util/SupportPlayUtil.h"
#include "pt/Util/ActorUtil.h"

namespace neo {
    SkeletalFish::SkeletalFish(const char *pName)
        : LiveActor(pName)
    {
        mPushedNerve = nullptr;
        mAnimScaleCtrl = nullptr;
        mRailClippingCenter.zero();
        mUpVec.set(0.0f, 1.0f, 0.0f);
        mFrontVec.set(0.0f, 0.0f, 1.0f);
    }

    void SkeletalFish::init(const JMapInfoIter &rIter) {
        MR::processInitFunction(this, rIter, false);
        MR::addToAttributeGroupSearchTurtle(this);

        MR::invalidateHitSensor(this, "Explosion");
        MR::initLightCtrl(this);

        MR::addEffect(this, "PointerTouchManual");
        MR::setEffectHostMtx(this, "PointerTouchManual", MR::getJointMtx(this, "Head"));
        MR::setEffectBaseScale(this, "PointerTouchManual", 1.5f);

        f32 speed = 15.0f;
        bool noStarPiece = false;
        bool noCalcGravity = false;
        MR::getJMapInfoArg0NoInit(rIter, &speed);
        MR::getJMapInfoArg1NoInit(rIter, &noStarPiece);
        MR::getJMapInfoArg2NoInit(rIter, &noCalcGravity);

        MR::moveCoordAndTransToNearestRailPos(this);
        MR::setRailCoordSpeed(this, speed);
        MR::initAndSetRailClipping(&mRailClippingCenter, this, 50.0f, 300.0f);

        if (!noStarPiece) {
            MR::declareStarPiece(this, 5);
        }

        if (!noCalcGravity) {
            MR::onCalcGravity(this);
            mUpVec = -mGravity;
        }
        else {
            TVec3f mUpVec;
            MR::calcUpVec(&mUpVec, this);
            mGravity = -mUpVec;
        }

        mAnimScaleCtrl = new AnimScaleController(nullptr);

        initNerve(NrvSkeletalFishSwim, 0);
        MR::useStageSwitchSyncAppear(this, rIter);
    }

    void SkeletalFish::control() {
        mAnimScaleCtrl->update();
    }

    void SkeletalFish::calcAndSetBaseMtx() {
        TPos3f baseMtx;
        MR::makeMtxFrontUpPos(&baseMtx, mFrontVec, mUpVec, mTranslation);
        MR::setBaseTRMtx(this, baseMtx);
        MR::extractMtxYDir(baseMtx, &mUpVec);
        MR::updateBaseScale(this, mAnimScaleCtrl);
    }

    void SkeletalFish::attackSensor(HitSensor *pSender, HitSensor *pReceiver) {
        if (MR::isEqualSensor(pSender, this, "Explosion")) {
            MR::sendMsgEnemyAttackExplosion(pReceiver, pSender);
        }
        else if ((isNerve(NrvSkeletalFishSwim)
            || isNerve(NrvSkeletalFishNotice)
            || isNerve(NrvSkeletalFishLaunch))) {
            if (MR::sendMsgEnemyAttackStrong(pReceiver, pSender)) {
                setNerve(NrvSkeletalFishExplode);
            }
        }
        else {
            MR::sendMsgPush(pReceiver, pSender);
        }
    }

    bool SkeletalFish::receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
        if (MR::isMsgJetTurtleAttack(msg) || MR::isMsgInvincibleAttack(msg)) {
            if (isNerve(NrvSkeletalFishSwim)
                || isNerve(NrvSkeletalFishNotice)
                || isNerve(NrvSkeletalFishLaunch)
                || isNerve(NrvSkeletalFishFreeze)) {
                setNerve(NrvSkeletalFishBreak);
                return true;
            }
        }
        else if (MR::isMsgStarPieceReflect(msg)) {
            if (isNerve(NrvSkeletalFishSwim)
                || isNerve(NrvSkeletalFishNotice)
                || isNerve(NrvSkeletalFishLaunch)
                || isNerve(NrvSkeletalFishFreeze)) {
                return true;
            }
        }

        return false;
    }

    void SkeletalFish::appear() {
        LiveActor::appear();

        MR::validateClipping(this);
        MR::validateHitSensors(this);
        MR::invalidateHitSensor(this, "Explosion");
        MR::onBind(this);

        MR::moveCoordAndTransToNearestRailPos(this);

        setNerve(NrvSkeletalFishSwim);
    }

    void SkeletalFish::exeSwim() {
        if (MR::isFirstStep(this)) {
            MR::startAction(this, "Swim");
            MR::startBrk(this, "Base");
        }

        trySupportTicoFreeze();

        MR::moveCoordAndFollowTrans(this);
        MR::calcRailDirection(&mFrontVec, this);
        mUpVec = -mGravity;

        if (MR::isInSightFanPlayer(this, mFrontVec, 1800.0f, 90.0f, 170.0f)) {
            setNerve(NrvSkeletalFishNotice);
        }
    }

    void SkeletalFish::exeNotice() {
        if (MR::isFirstStep(this)) {
            MR::startAction(this, "Turn");
            MR::startActionSound(this, "Notice", -1, -1, -1);
        }

        trySupportTicoFreeze();
        startAlarmSound();

        if (MR::isLessStep(this, 60)) {
            rotateToPlayer();
        }

        if (MR::isStep(this, 60)) {
            setNerve(NrvSkeletalFishLaunch);
        }
    }

    void SkeletalFish::exeLaunch() {
        if (MR::isFirstStep(this)) {
            MR::startAction(this, "Attack");
            MR::invalidateClipping(this);
        }

        trySupportTicoFreeze();
        startAlarmSound();

        MR::setVelocity(this, mFrontVec * MR::calcNerveEaseInValue(this, 30, 1.0f, 20.0f));

        if (MR::isStep(this, 200)) {
            MR::startBrk(this, "AttackNear");
        }

        if (MR::isStep(this, 300) || MR::isBindedWall(this) || !MR::isInWater(mTranslation)) {
            MR::zeroVelocity(this);
            setNerve(NrvSkeletalFishExplode);
        }
    }

    void SkeletalFish::exeExplode() {
        if (MR::isFirstStep(this)) {
            MR::startAction(this, "Break");
            MR::emitEffect(this, "Explosion");
            MR::validateHitSensor(this, "Explosion");

            if (MR::calcDistanceToPlayer(this) < 2000.0f) {
                MR::shakeCameraStrong();
            }

            MR::invalidateClipping(this);
            MR::offBind(this);
        }

        if (MR::isActionEnd(this)) {
            kill();
        }
    }

    void SkeletalFish::exeBreak() {
        if (MR::isFirstStep(this)) {
            MR::startAction(this, "Break");

            MR::invalidateClipping(this);
            MR::offBind(this);
        }

        if (MR::isActionEnd(this)) {
            tryAppearStarPiece();
            kill();
        }
    }

    void SkeletalFish::startFreeze() {
        mPushedNerve = mSpine->getCurrentNerve();
        setNerve(NrvSkeletalFishFreeze);
    }

    void SkeletalFish::exeFreeze() {
        if (MR::isFirstStep(this)) {
            MR::zeroVelocity(this);
            MR::emitEffect(this, "PointerTouchManual");
            mAnimScaleCtrl->startDpdHitVibration();
        }

        MR::startDPDFreezeLevelSound(this);

        if (mPushedNerve == NrvSkeletalFishNotice) {
            rotateToPlayer();
        }

        if (!MR::attachSupportTicoToTarget(this)) {
            setNerve(mPushedNerve);
        }
    }

    void SkeletalFish::endFreeze() {
        MR::tryDeleteEffect(this, "PointerTouchManual");
        mAnimScaleCtrl->startAnim();
    }

    void SkeletalFish::rotateToPlayer() {
        // Calculate front vec / launch dir
        TVec3f dirToPlayer;
        MR::calcVecToPlayer(&dirToPlayer, this);
        MR::normalize(&dirToPlayer);
        MR::turnVecToVecDegree(&mFrontVec, mFrontVec, dirToPlayer, 5.0f, mUpVec);
    }

    void SkeletalFish::startAlarmSound() {
        MR::startActionSound(this, "Alarm", (s32)MR::calcDistanceToPlayer(this), -1, -1);
    }

    bool SkeletalFish::tryAppearStarPiece() {
        s32 starPieceCount = MR::clamp(MR::getDeclareRemnantStarPieceCount(this), 0, 5);

        if (starPieceCount > 0) {
            TVec3f starPiecePos;
            MR::copyJointPos(this, "Joint02", &starPiecePos);
            MR::appearStarPiece(this, starPiecePos, starPieceCount, 10.0f, 40.0f, false);
            MR::startActionSound(this, "StarPieceBurst", -1, -1, -1);
            return true;
        }
        else {
            return false;
        }
    }

    bool SkeletalFish::trySupportTicoFreeze() {
        if (MR::attachSupportTicoToTarget(this)) {
            startFreeze();
            return true;
        }
        else {
            return false;
        }
    }

    namespace NrvSkeletalFish {
        NrvSwim(NrvSwim::sInstance);
        NrvNotice(NrvNotice::sInstance);
        NrvLaunch(NrvLaunch::sInstance);
        NrvExplode(NrvExplode::sInstance);
        NrvBreak(NrvBreak::sInstance);
        NrvFreeze(NrvFreeze::sInstance);

        void NrvSwim::execute(Spine *pSpine) const {
            SkeletalFish *pActor = (SkeletalFish*)pSpine->mExecutor;
            pActor->exeSwim();
        }

        void NrvNotice::execute(Spine *pSpine) const {
            SkeletalFish *pActor = (SkeletalFish*)pSpine->mExecutor;
            pActor->exeNotice();
        }

        void NrvLaunch::execute(Spine *pSpine) const {
            SkeletalFish *pActor = (SkeletalFish*)pSpine->mExecutor;
            pActor->exeLaunch();
        }

        void NrvExplode::execute(Spine *pSpine) const {
            SkeletalFish *pActor = (SkeletalFish*)pSpine->mExecutor;
            pActor->exeExplode();
        }

        void NrvBreak::execute(Spine *pSpine) const {
            SkeletalFish *pActor = (SkeletalFish*)pSpine->mExecutor;
            pActor->exeBreak();
        }

        void NrvFreeze::execute(Spine *pSpine) const {
            SkeletalFish *pActor = (SkeletalFish*)pSpine->mExecutor;
            pActor->exeFreeze();
        }

        void NrvFreeze::executeOnEnd(Spine *pSpine) const {
            SkeletalFish *pActor = (SkeletalFish*)pSpine->mExecutor;
            pActor->endFreeze();
        }
    }
}
