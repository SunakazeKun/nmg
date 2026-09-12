#pragma once

#include "syati.h"

namespace neo {
    class AstroCore : public LiveActor {
    public:
        AstroCore(const char *pName);

        virtual void init(const JMapInfoIter &rIter);
        virtual void attackSensor(HitSensor *pSender, HitSensor *pReceiver);

        void appearPowerStar();

        bool mStarSpawned;
    };
};
