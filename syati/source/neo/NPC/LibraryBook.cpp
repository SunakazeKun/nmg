#include "neo/NPC/LibraryBook.h"
#include "Game/Util/TalkUtil.h"

namespace neo {
    LibraryBook::LibraryBook(const char *pName)
        : NPCActor(pName)
    {

    }

    void LibraryBook::init(const JMapInfoIter &rIter) {
        NPCActorCaps caps("LibraryBook");
        
        caps.setDefault();
        caps.mWaitNerve = NrvLibraryBookWait;
        caps.mBodySensorRadius = 20.f;
        caps.mInitSphereShadow = false;
        caps.mInitYoshiLockOnTarget = false;
        caps.mInitStarPointerTarget = false;

        initialize(rIter, caps, nullptr, nullptr, false);
    }

    // -----------------------------------------------------------------------------------------------------------------

    namespace NrvLibraryBook {
        NrvWait(NrvWait::sInstance);

        void NrvWait::execute(Spine *pSpine) const {
            LibraryBook *pActor = (LibraryBook*)pSpine->mExecutor;
            MR::tryTalkNearPlayer(pActor->mTalkCtrl);
        }
    }
}
