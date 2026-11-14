#pragma once

#include "Game/LiveActor/LiveActor.h"

namespace neo {
    class HipDropNailWatcher;

    class HipDropNail : public LiveActor {
    public:
        HipDropNail(const char *pName);

        virtual void init(const JMapInfoIter &rIter);
        virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver);

        void setHost(HipDropNailWatcher *pHost);
        bool isMoved() const;

        void exeWait();
        void exeMove();
        void exeDone();

        void displacePosition(f32 yOffset);

        HipDropNailWatcher* mHost;
        int mAlreadyDoneFlagIdx;
        f32 mYOffset;
        s32 mMoveTime;
        TVec3f mUpVec;
        TVec3f mOrgPos;
    };

    class HipDropNailWatcher : public LiveActor {
    public:
        HipDropNailWatcher(const char *pName);

        virtual void init(const JMapInfoIter &rIter);

        void setSpawnPos(const TVec3f &rSpawnPos);

        void exeWatch();
        void exeComplete();
        void exeDone();

        HipDropNail** mNails;
        s32 mNailsCount;
        s32 mScenarioId;
        TVec3f mSpawnPos;
        int mAlreadyDoneFlagIdx;
        bool mPlayFanfare;
    };
}
