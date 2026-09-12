#pragma once

#include "syati.h"

namespace neo {
    class SkeletalFish : public LiveActor {
    public:
        SkeletalFish(const char *pName);

        virtual void init(const JMapInfoIter &rIter);
        virtual void appear();
        virtual void control();
        virtual void calcAndSetBaseMtx();
        virtual void attackSensor(HitSensor *pSender, HitSensor *pReceiver);
        virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver);

        void exeSwim();
        void exeNotice();
        void exeLaunch();
        void exeExplode();
        void exeBreak();

        void startFreeze();
        void exeFreeze();
        void endFreeze();

        void rotateToPlayer();
        void startAlarmSound();
        bool tryAppearStarPiece();
        bool trySupportTicoFreeze();

        const Nerve* mPushedNerve;
        AnimScaleController* mAnimScaleCtrl;
        TVec3f mRailClippingCenter;
        TVec3f mFrontVec;
        TVec3f mUpVec;
    };

    namespace NrvSkeletalFish {
        NERVE(NrvSwim);
        NERVE(NrvNotice);
        NERVE(NrvLaunch);
        NERVE(NrvExplode);
        NERVE(NrvBreak);
        ENDABLE_NERVE(NrvFreeze);
    };
};

#define NrvSkeletalFishSwim &neo::NrvSkeletalFish::NrvSwim::sInstance
#define NrvSkeletalFishNotice &neo::NrvSkeletalFish::NrvNotice::sInstance
#define NrvSkeletalFishLaunch &neo::NrvSkeletalFish::NrvLaunch::sInstance
#define NrvSkeletalFishExplode &neo::NrvSkeletalFish::NrvExplode::sInstance
#define NrvSkeletalFishBreak &neo::NrvSkeletalFish::NrvBreak::sInstance
#define NrvSkeletalFishFreeze &neo::NrvSkeletalFish::NrvFreeze::sInstance
