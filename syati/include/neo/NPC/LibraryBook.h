#pragma once

#include "syati.h"

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
