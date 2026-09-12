#include "neo/NPC/LibraryBook.h"

namespace neo {
	LibraryBook::LibraryBook(const char *pName) : NPCActor(pName) {

	}

	void LibraryBook::init(const JMapInfoIter &rIter) {
		NPCActorCaps capabilities("LibraryBook");
		capabilities.setDefault();
		capabilities.mWaitNerve = NrvLibraryBookWait;
		capabilities.mBodySensorRadius = 20.f;
		capabilities.mInitSphereShadow = false;
		capabilities.mInitYoshiLockOnTarget = false;

		initialize(rIter, capabilities, NULL, NULL, false);
	}

	namespace NrvLibraryBook {
		void NrvWait::execute(Spine *pSpine) const {
			LibraryBook *pActor = (LibraryBook*)pSpine->mExecutor;
			MR::tryTalkNearPlayer(pActor->mTalkCtrl);
		}

		NrvWait(NrvWait::sInstance);
	}
}
