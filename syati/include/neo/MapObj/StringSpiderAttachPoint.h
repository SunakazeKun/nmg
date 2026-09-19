#pragma once

#include "Game/LiveActor/LiveActor.h"

class StringSpider;

namespace neo {
    class StringSpiderAttachPoint : public LiveActor {
    public:
        StringSpiderAttachPoint(const char *pName);

        virtual void init(const JMapInfoIter &rIter);
        virtual void control();
        virtual void attackSensor(HitSensor *pSender, HitSensor *pReceiver);
        virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver);

        void initClipping(const JMapInfoIter &rIter);

        StringSpider* mStringSpider;
        s32 mNumStarPiece;
        TVec3f mClippingCenter;
    };
}
