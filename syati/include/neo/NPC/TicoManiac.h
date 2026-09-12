#pragma once

#include "syati.h"

namespace neo {
    class TicoManiac : public NPCActor {
    public:
        TicoManiac(const char *pName);

        virtual void init(const JMapInfoIter &rIter);
        virtual void calcAnim();
        virtual void control();

        void exeMeta();
        bool killFunc(u32 funcType);
    };

    namespace NrvTicoManiac {
        NERVE(NrvMeta);
    };
}

#define NrvTicoManiacMeta &neo::NrvTicoManiac::NrvMeta::sInstance
