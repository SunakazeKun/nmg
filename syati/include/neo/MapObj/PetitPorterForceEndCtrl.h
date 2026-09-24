#include "Game/LiveActor/LiveActor.h"

class PetitPorter;

namespace neo {
    class PetitPorterForceEndCtrl : public LiveActor {
    public:
        PetitPorterForceEndCtrl(const char *pName);

        virtual void init(const JMapInfoIter &rIter);

        void exeWaitForPorter();
        void exeForceEndPorter();

        PetitPorter* mPetitPorter;
        s32 mDelay;
    };

    namespace NrvPetitPorterForceEndCtrl {
        NERVE(NrvWaitForPorter);
        NERVE(NrvForceEndPorter);
    }
}

#define NrvPetitPorterForceEndCtrlWaitForPorter &neo::NrvPetitPorterForceEndCtrl::NrvWaitForPorter::sInstance
#define NrvPetitPorterForceEndCtrlForceEndPorter &neo::NrvPetitPorterForceEndCtrl::NrvForceEndPorter::sInstance
