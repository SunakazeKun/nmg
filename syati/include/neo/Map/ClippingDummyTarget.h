#pragma once

#include "syati.h"

namespace neo {
    class ClippingDummyTarget : public LiveActor {
    public:
        ClippingDummyTarget(const char *pName);

        virtual void init(const JMapInfoIter &rIter);
    };
};
