#pragma once

#include "syati.h"

namespace neo {
    class SpiderCocoon : public LiveActor {
    public:
        SpiderCocoon(const char *pName);

        virtual void init(const JMapInfoIter &rIter);
        virtual void attackSensor(HitSensor *pSender, HitSensor *pReceiver);
        virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver);

        void openCocoon();

        void exeShake();
        void exeOpen();
    };

    namespace NrvSpiderCocoon {
        NERVE(NrvWait);
        NERVE(NrvShake);
        NERVE(NrvOpen);
    };
};

#define NrvSpiderCocoonWait &neo::NrvSpiderCocoon::NrvWait::sInstance
#define NrvSpiderCocoonShake &neo::NrvSpiderCocoon::NrvShake::sInstance
#define NrvSpiderCocoonOpen &neo::NrvSpiderCocoon::NrvOpen::sInstance
