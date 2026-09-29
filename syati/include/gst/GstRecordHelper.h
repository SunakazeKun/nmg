#pragma once

#include "Game/LiveActor/LiveActor.h"
#include "gst/GhostType.h"

class IconAButton;

namespace gst {
    class GstRecordHelper : public LiveActor {
    public:
        GstRecordHelper(const char *pName);

        virtual void init(const JMapInfoIter &rIter);
        virtual void makeActorAppeared();
        virtual void control();
        virtual void attackSensor(HitSensor *pSender, HitSensor *pReceiver);
        virtual bool receiveOtherMsg(u32 msg, HitSensor *pSender, HitSensor *pReceiver);

        virtual GhostType getGhostType() const = 0;
        virtual s32 getStateCount() const;
        virtual void initModelAndScene(const JMapInfoIter &rIter);
        virtual void handlePrepare();
        virtual void handleStopped();
        virtual void supplyGstFrame() const;

        void forceStop();

        void exeWait();
        void exePreparing();
        void exeRecording();

        IconAButton* mIconAButton;
        bool mShowIconAButton;
    };

    namespace NrvGstRecordHelper {
        NERVE(NrvWait);
        NERVE(NrvPreparing);
        NERVE(NrvRecording);
    }
}

#define NrvGstRecordHelperWait &gst::NrvGstRecordHelper::NrvWait::sInstance
#define NrvGstRecordHelperPreparing &gst::NrvGstRecordHelper::NrvPreparing::sInstance
#define NrvGstRecordHelperRecording &gst::NrvGstRecordHelper::NrvRecording::sInstance
