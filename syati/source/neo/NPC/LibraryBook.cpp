#include "neo/NPC/LibraryBook.h"
#include "Game/Util/TalkUtil.h"

namespace neo {
    LibraryBook::LibraryBook(const char *pName)
		: NPCActor(pName)
	{

    }

    void LibraryBook::init(const JMapInfoIter &rIter) {
        NPCActorCaps capabilities("LibraryBook");
        capabilities.setDefault();
        capabilities.mWaitNerve = NrvLibraryBookWait;
        capabilities.mBodySensorRadius = 20.f;
        capabilities.mInitSphereShadow = false;
        capabilities.mInitYoshiLockOnTarget = false;

        initialize(rIter, capabilities, nullptr, nullptr, false);
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
