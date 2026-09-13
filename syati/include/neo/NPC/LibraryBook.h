#pragma once

#include "Game/NPC/NPCActor.h"

namespace neo {
    class LibraryBook : public NPCActor {
    public:
        LibraryBook(const char *pName);

        virtual void init(const JMapInfoIter &rIter);
    };

    namespace NrvLibraryBook {
        NERVE(NrvWait);
    };
};

#define NrvLibraryBookWait &neo::NrvLibraryBook::NrvWait::sInstance
