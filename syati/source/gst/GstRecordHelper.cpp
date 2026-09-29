#include "gst/GstRecordHelper.h"
#include "gst/GstRecordSystem.h"
#include "Game/Screen/IconAButton.h"
#include "Game/Util/ActorInitUtil.h"
#include "Game/Util/ActorSensorUtil.h"
#include "Game/Util/GamePadUtil.h"
#include "Game/Util/LiveActorUtil.h"
#include "Game/Util/ObjUtil.h"
#include "Game/Util/PlayerUtil.h"
#include "Game/Util/SceneUtil.h"
#include "Game/System/AllData/GameSequenceFunction.h"

namespace gst {
    GstRecordHelper::GstRecordHelper(const char *pName) : LiveActor(pName) {
        mIconAButton = NULL;
        mShowIconAButton = false;
    }

    void GstRecordHelper::init(const JMapInfoIter &rIter) {
        MR::initDefaultPos(this, rIter);
        initModelAndScene(rIter);

        initHitSensor(1);
        MR::addHitSensorMapObjSimple(this, "Body", 8, 150.0f, TVec3f(0.0f, 0.0f, 0.0f));

        mIconAButton = new IconAButton(true, false);
        mIconAButton->initWithoutIter();

        initNerve(NrvGstRecordHelperWait, getStateCount());
        makeActorAppeared();
    }

    void GstRecordHelper::initModelAndScene(const JMapInfoIter &rIter) {
        MR::connectToSceneNpcMovement(this);
    }

    s32 GstRecordHelper::getStateCount() const {
        return 0;
    }

    void GstRecordHelper::makeActorAppeared() {
        if (isEnableDebugGstRecorder() && !GameSequenceFunction::isPadMode()) {
            LiveActor::makeActorAppeared();
        }
        else {
            LiveActor::makeActorDead();
        }
    }

    void GstRecordHelper::control() {
        mShowIconAButton = false;
    }

    void GstRecordHelper::attackSensor(HitSensor *pSender, HitSensor *pReceiver) {
        if (isNerve(NrvGstRecordHelperWait)) {
            if (MR::isSensorPlayer(pReceiver) && MR::isOnGroundPlayer()) {
                mShowIconAButton = true;
            }
        }
    }

    bool GstRecordHelper::receiveOtherMsg(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
        if (MR::isMsgStartPowerStarGet(msg)) {
            forceStop();
            incGstFrame();
            return true;
        }

        return false;
    }

    void GstRecordHelper::exeWait() {
        if (mShowIconAButton) {
            if (!mIconAButton->isOpen()) {
                mIconAButton->openWithRead();
            }
        }
        else if (mIconAButton->isOpen()) {
            mIconAButton->term();
        }

        if (mShowIconAButton && mIconAButton->isOpen() && MR::testCorePadTriggerA(0)) {
            setNerve(NrvGstRecordHelperPreparing);
        }
    }

    void GstRecordHelper::exePreparing() {
        if (MR::isFirstStep(this)) {
            mIconAButton->term();

            MR::invalidateClipping(this);
            MR::offPlayerControl();
            handlePrepare();

            prepareGstRecorder(MR::getCurrentStageName(), MR::getCurrentSelectedScenarioNo(), getGhostType());
        }

        if (MR::isStep(this, 60)) {
            setNerve(NrvGstRecordHelperRecording);
        }
    }

    void GstRecordHelper::exeRecording() {
        if (MR::isFirstStep(this)) {
            MR::onPlayerControl(true);
            startGstRecorder();
        }

        supplyGstFrame();

        if (MR::testCorePadTrigger2(0)) {
            forceStop();
        }
    }

    void GstRecordHelper::forceStop() {
        stopGstRecorder();
        handleStopped();
        makeActorDead();
    }

    void GstRecordHelper::handlePrepare() {

    }

    void GstRecordHelper::handleStopped() {

    }

    void GstRecordHelper::supplyGstFrame() const {
        supplyGstFrameSyncWithActor(this, true);
    }

    namespace NrvGstRecordHelper {
        void NrvWait::execute(Spine *pSpine) const {
            GstRecordHelper* pHost = (GstRecordHelper*)pSpine->mExecutor;
            pHost->exeWait();
        }

        void NrvPreparing::execute(Spine *pSpine) const {
            GstRecordHelper* pHost = (GstRecordHelper*)pSpine->mExecutor;
            pHost->exePreparing();
        }

        void NrvRecording::execute(Spine *pSpine) const {
            GstRecordHelper* pHost = (GstRecordHelper*)pSpine->mExecutor;
            pHost->exeRecording();
        }

        NrvWait(NrvWait::sInstance);
        NrvPreparing(NrvPreparing::sInstance);
        NrvRecording(NrvRecording::sInstance);
    }
}
