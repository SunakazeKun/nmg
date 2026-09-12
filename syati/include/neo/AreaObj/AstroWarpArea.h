#pragma once

#include "syati.h"

namespace neo {
    class AstroWarpArea : public AreaObj {
    public:
        AstroWarpArea(const char *pName);

        virtual void init(const JMapInfoIter &rIter);
        virtual void movement();
        virtual const char* getManagerName() const;

        void exeWait();
        void exeStartWarp();
        void exeEndWarp();

        Spine* mSpine;
    };

    namespace NrvAstroWarpArea {
        NERVE(NrvWait);
        NERVE(NrvStartWarp);
        NERVE(NrvEndWarp);
    };
}

#define NrvAstroWarpAreaWait &neo::NrvAstroWarpArea::NrvWait::sInstance
#define NrvAstroWarpAreaStartWarp &neo::NrvAstroWarpArea::NrvStartWarp::sInstance
#define NrvAstroWarpAreaEndWarp &neo::NrvAstroWarpArea::NrvEndWarp::sInstance
