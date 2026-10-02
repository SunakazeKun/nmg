#pragma once

#include "Game/NPC/NPCActor.h"

namespace neo {
    class TicoComet : public NPCActor {
    public:
        TicoComet(const char *pName);

        virtual void init(const JMapInfoIter &rIter);
        virtual void control();
    };
}
