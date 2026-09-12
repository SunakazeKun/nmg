#pragma once

#include "syati.h"

namespace neo {
    class PhantomShipBridge : public LiveActor {
    public:
        PhantomShipBridge(const char *pName);

        virtual void init(const JMapInfoIter &rIter);
        virtual void calcAnim();

        void startMoveA();
        void startMoveB();
        void setStateMoveA();
        void exeMoveA();
        void exeMoveB();

        CollisionParts* mMoveCollision;
    };

    namespace NrvPhantomShipBridge {
        NERVE(NrvWait);
        NERVE(NrvMoveA);
        NERVE(NrvMoveB);
    };
};

#define NrvPhantomShipBridgeWait &neo::NrvPhantomShipBridge::NrvWait::sInstance
#define NrvPhantomShipBridgeMoveA &neo::NrvPhantomShipBridge::NrvMoveA::sInstance
#define NrvPhantomShipBridgeMoveB &neo::NrvPhantomShipBridge::NrvMoveB::sInstance
