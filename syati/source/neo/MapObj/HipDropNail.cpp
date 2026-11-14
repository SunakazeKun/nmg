#include "neo/MapObj/HipDropNail.h"
#include "Game/NameObj/NameObjFactory.h"
#include "Game/Util/ActionSoundUtil.h"
#include "Game/Util/ActorInitUtil.h"
#include "Game/Util/ActorMovementUtil.h"
#include "Game/Util/ActorSensorUtil.h"
#include "Game/Util/ActorSwitchUtil.h"
#include "Game/Util/EventUtil.h"
#include "Game/Util/JMapUtil.h"
#include "Game/Util/LiveActorUtil.h"
#include "Game/Util/ObjUtil.h"
#include "Game/Util/SceneUtil.h"

namespace neo {
    namespace NrvHipDropNail {
        FULL_NERVE(NrvWait, HipDropNail, Wait);
        FULL_NERVE(NrvMove, HipDropNail, Move);
        FULL_NERVE(NrvDone, HipDropNail, Done);
    }

    namespace NrvHipDropNailWatcher {
        FULL_NERVE(NrvWatch, HipDropNailWatcher, Watch);
        FULL_NERVE(NrvComplete, HipDropNailWatcher, Complete);
        FULL_NERVE(NrvDone, HipDropNailWatcher, Done);
    }

#define NrvHipDropNailWait &neo::NrvHipDropNail::NrvWait::sInstance
#define NrvHipDropNailMove &neo::NrvHipDropNail::NrvMove::sInstance
#define NrvHipDropNailDone &neo::NrvHipDropNail::NrvDone::sInstance

#define NrvHipDropNailWatcherWatch &neo::NrvHipDropNailWatcher::NrvWatch::sInstance
#define NrvHipDropNailWatcherComplete &neo::NrvHipDropNailWatcher::NrvComplete::sInstance
#define NrvHipDropNailWatcherDone &neo::NrvHipDropNailWatcher::NrvDone::sInstance

    // -----------------------------------------------------------------------------------------------------------------

    HipDropNail::HipDropNail(const char *pName)
        : LiveActor(pName),
        mHost(),
        mAlreadyDoneFlagIdx(-1),
        mYOffset(100.0f),
        mMoveTime(60),
        mUpVec(0.0f, 1.0f, 0.0f),
        mOrgPos()
    {

    }

    void HipDropNail::init(const JMapInfoIter &rIter) {
        MR::processInitFunction(this, rIter, false);

        s32 flagSub = -1;
        bool flagSet = false;

        MR::getJMapInfoArg0NoInit(rIter, &mYOffset);
        MR::getJMapInfoArg1NoInit(rIter, &mMoveTime);
        MR::getJMapInfoArg2NoInit(rIter, &flagSub);

        mOrgPos = mTranslation;
        MR::calcUpVec(&mUpVec, this);

        if (flagSub >= 0) {
            char flagName[32];
            snprintf(flagName, sizeof(flagName), "HipDropNailMoved%03d", flagSub);

            flagSet = MR::setupAlreadyDoneFlag(&mAlreadyDoneFlagIdx, flagName, rIter, true);
        }

        if (flagSet) {
            displacePosition(mYOffset);
            initNerve(NrvHipDropNailDone, 0);
        }
        else {
            initNerve(NrvHipDropNailWait, 0);
        }

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
        return isNerve(NrvHipDropNailDone);
    }

    void HipDropNail::exeWait() {
        
    }

    void HipDropNail::exeMove() {
        f32 yOffset = MR::calcNerveValue(this, mMoveTime, 0.0f, mYOffset);
        displacePosition(yOffset);

        if (MR::isGreaterEqualStep(this, mMoveTime)) {
            setNerve(NrvHipDropNailDone);
        }
    }

    void HipDropNail::exeDone() {
        if (MR::isFirstStep(this)) {
            MR::invalidateCollisionParts(this);

            if (MR::isValidSwitchA(this)) {
                MR::onSwitchA(this);
            }
            
            if (mAlreadyDoneFlagIdx >= 0) {
                MR::onAlreadyDoneFlag(mAlreadyDoneFlagIdx);
            }

            if (mHost != nullptr) {
                mHost->setSpawnPos(mTranslation);
            }
        }
    }

    void HipDropNail::displacePosition(f32 yOffset) {
        TVec3f newPosition = mOrgPos + mUpVec * -yOffset;
        MR::setPosition(this, newPosition);
    }

    // -----------------------------------------------------------------------------------------------------------------

    HipDropNailWatcher::HipDropNailWatcher(const char *pName)
        : LiveActor(pName),
        mNails(),
        mNailsCount(),
        mScenarioId(-1),
        mSpawnPos(),
        mAlreadyDoneFlagIdx(-1),
        mPlayFanfare()
    {

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
        
        bool flagUse = false;
        bool flagSet = false;
        MR::getJMapInfoArg2NoInit(rIter, &flagUse);

        if (flagUse) {
            flagSet = MR::setupAlreadyDoneFlag(&mAlreadyDoneFlagIdx, "HipDropNailComplete", rIter, true);
        }

        if (mPlayFanfare) {
            initSound(2, "ExterminationGeneralChecker", nullptr, TVec3f(0.0f, 0.0f, 0.0f));
        }

        if (flagSet) {
            if (MR::isValidSwitchA(this)) {
                MR::onSwitchA(this);
            }

            initNerve(NrvHipDropNailWatcherDone, 0);
        }
        else {
            initNerve(NrvHipDropNailWatcherWatch, 0);
        }

        if (mScenarioId != -1 && !flagSet) {
            MR::declarePowerStar(this, mScenarioId);
        }

        makeActorAppeared();
    }

    void HipDropNailWatcher::setSpawnPos(const TVec3f &rSpawnPos) {
        mSpawnPos.set(rSpawnPos);
    }

    void HipDropNailWatcher::exeWatch() {
        bool complete = true;

        for (s32 i = 0; i < mNailsCount; i++) {
            if (!mNails[i]->isMoved()) {
                complete = false;
                break;
            }
        }

        if (complete) {
            setNerve(NrvHipDropNailWatcherComplete);
        }
    }

    void HipDropNailWatcher::exeComplete() {
        if (MR::isFirstStep(this)) {
            if (MR::isValidSwitchA(this)) {
                MR::onSwitchA(this);
            }

            if (mAlreadyDoneFlagIdx >= 0) {
                MR::onAlreadyDoneFlag(mAlreadyDoneFlagIdx);
            }

            if (mPlayFanfare) {
                MR::startActionSound(this, "CompleteInstant", -1, -1, -1);
            }
        }

        if (!mPlayFanfare || MR::isGreaterEqualStep(this, 105)) {
            if (mScenarioId != -1) {
                MR::requestAppearPowerStar(this, mScenarioId, mSpawnPos);
            }

            setNerve(NrvHipDropNailWatcherDone);
        }
    }

    void HipDropNailWatcher::exeDone() {

    }
}
