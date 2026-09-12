#pragma once

#include "syati.h"

namespace neo {
    class AstroLibrary : public LiveActor {
    public:
        AstroLibrary(const char *pName);

        virtual void init(const JMapInfoIter &rIter);
    };
}
