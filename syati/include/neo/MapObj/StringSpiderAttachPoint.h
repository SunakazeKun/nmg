#pragma once

#include "Game/LiveActor/LiveActor.h"

class StringSpider;

namespace neo {
    class StringSpiderAttachPoint : public LiveActor {
    public:
        StringSpiderAttachPoint(const char *pName);

        virtual void init(const JMapInfoIter &rIter);
        virtual void control();

        void initClipping(const JMapInfoIter &rIter);

        StringSpider* mStringSpider;
        TVec3f mClippingCenter;
    };
};
