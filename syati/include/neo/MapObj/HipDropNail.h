#pragma once

#include "syati.h"

namespace neo {
    class HipDropNailWatcher;

    class HipDropNail : public LiveActor {
    public:
        HipDropNail(const char *pName);

        virtual void init(const JMapInfoIter &rIter);
        virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver);

        void setHost(HipDropNailWatcher *pHost);
        bool isMoved() const;

        void exeMove();
        void exeMoved();

        HipDropNailWatcher* mHost;
        f32 mYOffset;
        s32 mMoveTime;
        TVec3f mUpVec;
        TVec3f mOrgPos;
    };

    namespace NrvHipDropNail {
        NERVE(NrvWait);
        NERVE(NrvMove);
        NERVE(NrvMoved);
    }

    class HipDropNailWatcher : public LiveActor {
    public:
        HipDropNailWatcher(const char *pName);

        virtual void init(const JMapInfoIter &rIter);

        void setSpawnPos(const TVec3f &rSpawnPos);

        void exeWatch();
        void exeComplete();

        HipDropNail** mNails;
        s32 mNailsCount;
        s32 mScenarioId;
        TVec3f mSpawnPos;
        bool mPlayFanfare;
    };

    namespace NrvHipDropNailWatcher {
        NERVE(NrvWatch);
        NERVE(NrvComplete);
    };
};

#define NrvHipDropNailWait &neo::NrvHipDropNail::NrvWait::sInstance
#define NrvHipDropNailMove &neo::NrvHipDropNail::NrvMove::sInstance
#define NrvHipDropNailMoved &neo::NrvHipDropNail::NrvMoved::sInstance

#define NrvHipDropNailWatcherWatch &neo::NrvHipDropNailWatcher::NrvWatch::sInstance
#define NrvHipDropNailWatcherComplete &neo::NrvHipDropNailWatcher::NrvComplete::sInstance
