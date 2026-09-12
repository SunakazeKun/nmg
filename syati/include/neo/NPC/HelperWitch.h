#pragma once

#include "syati.h"

#define HELPER_WITCH_EVENT_DAREDEVIL_MODE_ON 0
#define HELPER_WITCH_EVENT_DAREDEVIL_MODE_OFF 1
#define HELPER_WITCH_EVENT_SET_PLAYER_MARIO 2
#define HELPER_WITCH_EVENT_SET_PLAYER_LUIGI 3
#define HELPER_WITCH_EVENT_MSG_ARG_DAREDEVIL 4
#define HELPER_WITCH_EVENT_MSG_ARG_PLAYER 5
#define HELPER_WITCH_EVENT_RESET_SCENE 6

#define HELPER_WITCH_BRANCH_RESET_BEFORE 0
#define HELPER_WITCH_BRANCH_SHOULD_RESET 1
#define HELPER_WITCH_BRANCH_DAREDEVIL_MODE 2

namespace neo {
    class HelperWitch : public NPCActor {
    public:
        HelperWitch(const char *pName);

        virtual void init(const JMapInfoIter &rIter);

        bool eventFunc(u32 funcType);
        bool branchFunc(u32 funcType);

        JMapIdInfo mRestartId;
        bool mShouldReset;
    };
};
