#include "neo/MapObj/HipDropNail.h"
#include "Game/NameObj/NameObjFactory.h"
#include "Game/Util/ActionSoundUtil.h"
#include "Game/Util/ActorInitUtil.h"
#include "Game/Util/ActorMovementUtil.h"
#include "Game/Util/ActorSensorUtil.h"
#include "Game/Util/ActorSwitchUtil.h"
#include "Game/Util/JMapUtil.h"
#include "Game/Util/LiveActorUtil.h"
#include "Game/Util/ObjUtil.h"
#include "Game/Util/SceneUtil.h"

namespace neo {
    HipDropNail::HipDropNail(const char *pName)
        : LiveActor(pName)
    {
        mHost = nullptr;
        mYOffset = 100.0f;
        mMoveTime = 60;
        mUpVec.set(0.0f, 1.0f, 0.0f);
        mOrgPos.zero();
    }

    void HipDropNail::init(const JMapInfoIter &rIter) {
        MR::processInitFunction(this, rIter, false);
        MR::calcUpVec(&mUpVec, this);

        MR::getJMapInfoArg0NoInit(rIter, &mYOffset);
        MR::getJMapInfoArg1NoInit(rIter, &mMoveTime);

        MR::validateCollisionParts(this);

        initNerve(NrvHipDropNailWait, 0);
        makeActorAppeared();
    }

    bool HipDropNail::receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
        if (MR::isMsgPlayerHipDrop(msg) || MR::isMsgPlayerHipDropFloor(msg)) {
            if (isNerve(NrvHipDropNailWait)) {
                TVec3f dirToPlayer;
                MR::calcSensorDirection(&dirToPlayer, pReceiver, pSender);

                if (mUpVec.dot(dirToPlayer) >= 0.0f) {
                    setNerve(NrvHipDropNailMove);
                    return true;
                }
            }
        }

        return false;
    }

    void HipDropNail::setHost(HipDropNailWatcher *pHost) {
        mHost = pHost;
    }

    bool HipDropNail::isMoved() const {
        return isNerve(NrvHipDropNailMoved);
    }

    void HipDropNail::exeMove() {
        if (MR::isFirstStep(this)) {
            mOrgPos.set(mTranslation);
        }

        f32 yOffset = MR::calcNerveValue(this, mMoveTime, 0.0f, mYOffset);
        TVec3f newPosition = mOrgPos + mUpVec * -yOffset;
        MR::setPosition(this, newPosition);

        if (MR::isStep(this, mMoveTime)) {
            if (MR::isValidSwitchA(this)) {
                MR::onSwitchA(this);
            }

            setNerve(NrvHipDropNailMoved);
        }
    }

    void HipDropNail::exeMoved() {
        if (MR::isFirstStep(this)) {
            MR::invalidateCollisionParts(this);

            if (MR::isValidSwitchA(this)) {
                MR::onSwitchA(this);
            }

            if (mHost) {
                mHost->setSpawnPos(mTranslation);
            }
        }
    }

    // -----------------------------------------------------------------------------------------------------------------

    HipDropNailWatcher::HipDropNailWatcher(const char *pName)
        : LiveActor(pName)
    {
        mNails = nullptr;
        mNailsCount = 0;
        mScenarioId = -1;
        mSpawnPos.zero();
    }

    void HipDropNailWatcher::init(const JMapInfoIter &rIter) {
        MR::initDefaultPos(this, rIter);
        MR::connectToSceneMapObjMovement(this);
        MR::invalidateClipping(this);

        MR::getJMapInfoArg0NoInit(rIter, &mScenarioId);
        MR::getJMapInfoArg1NoInit(rIter, &mPlayFanfare);
        MR::useStageSwitchReadA(this, rIter);

        mNailsCount = MR::getChildObjNum(rIter);
        mNails = new HipDropNail*[mNailsCount];

        for (s32 i = 0; i < mNailsCount; i++) {
            mNails[i] = (HipDropNail *)NameObjFactory::initChildObj(rIter, i);
            mNails[i]->setHost(this);
        }

        initNerve(NrvHipDropNailWatcherWatch, 0);

        if (mScenarioId != -1) {
            MR::declarePowerStar(this, mScenarioId);
        }

        if (mPlayFanfare) {
            initSound(2, "ExterminationGeneralChecker", nullptr, TVec3f(0.0f, 0.0f, 0.0f));
        }

        makeActorAppeared();
    }

    void HipDropNailWatcher::setSpawnPos(const TVec3f &rSpawnPos) {
        mSpawnPos.set(rSpawnPos);
    }

    void HipDropNailWatcher::exeWatch() {
        bool spawn = true;

        for (s32 i = 0; i < mNailsCount; i++) {
            if (!mNails[i]->isMoved()) {
                spawn = false;
                break;
            }
        }

        if (spawn) {
            setNerve(NrvHipDropNailWatcherComplete);
        }
    }

    void HipDropNailWatcher::exeComplete() {
        if (MR::isFirstStep(this)) {
            if (MR::isValidSwitchA(this)) {
                MR::onSwitchA(this);
            }

            if (mPlayFanfare) {
                MR::startActionSound(this, "CompleteInstant", -1, -1, -1);
            }
        }

        if (!mPlayFanfare || MR::isStep(this, 105)) {
            if (mScenarioId != -1) {
                MR::requestAppearPowerStar(this, mScenarioId, mSpawnPos);
            }

            kill();
        }
    }

    // -----------------------------------------------------------------------------------------------------------------

    namespace NrvHipDropNail {
        NrvWait(NrvWait::sInstance);
        NrvMove(NrvMove::sInstance);
        NrvMoved(NrvMoved::sInstance);

        void NrvWait::execute(Spine *pSpine) const {}

        void NrvMove::execute(Spine *pSpine) const {
            HipDropNail *pActor = (HipDropNail*)pSpine->mExecutor;
            pActor->exeMove();
        }

        void NrvMoved::execute(Spine *pSpine) const {
            HipDropNail *pActor = (HipDropNail*)pSpine->mExecutor;
            pActor->exeMoved();
        }
    }

    // -----------------------------------------------------------------------------------------------------------------

    namespace NrvHipDropNailWatcher {
        NrvWatch(NrvWatch::sInstance);
        NrvComplete(NrvComplete::sInstance);

        void NrvWatch::execute(Spine *pSpine) const {
            HipDropNailWatcher *pActor = (HipDropNailWatcher*)pSpine->mExecutor;
            pActor->exeWatch();
        }

        void NrvComplete::execute(Spine *pSpine) const {
            HipDropNailWatcher *pActor = (HipDropNailWatcher*)pSpine->mExecutor;
            pActor->exeComplete();
        }
    }
}
