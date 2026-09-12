#pragma once

#include "syati.h"

namespace neo {
    class Butler : public NPCActor {
    public:
        Butler(const char *pName);

        virtual void init(const JMapInfoIter &rIter);
        virtual void kill();
        virtual void control();
    };
};
