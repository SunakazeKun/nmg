#include "neo/MapObj/PetitPorterForceEndCtrl.h"
#include "Game/MapObj/PetitPorter.h"
#include "Game/Util/ActorSwitchUtil.h"
#include "Game/Util/JMapUtil.h"
#include "Game/Util/LiveActorUtil.h"
#include "Game/Util/ObjUtil.h"
#include "Game/Util/SceneUtil.h"


namespace neo {
    PetitPorterForceEndCtrl::PetitPorterForceEndCtrl(const char *pName)
        : LiveActor(pName)
    {
        mPetitPorter = nullptr;
        mDelay = 0;
    }

    void PetitPorterForceEndCtrl::init(const JMapInfoIter &rIter) {
        MR::connectToSceneMapObjMovement(this);
        MR::invalidateClipping(this);

        mPetitPorter = new PetitPorter("プチポーター");
        MR::initChildObj(mPetitPorter, rIter, 0);

        MR::getJMapInfoArg0NoInit(rIter, &mDelay);
        MR::needStageSwitchReadA(this, rIter);

        initNerve(NrvPetitPorterForceEndCtrlWaitForPorter, 0);
        makeActorAppeared();
    }

    void PetitPorterForceEndCtrl::exeWaitForPorter() {
        if (!mPetitPorter->isNerve(&NrvPetitPorter::PetitPorterNrvMinigame::sInstance)) {
            return;
        }
        
        if (MR::isOnSwitchA(this)) {
            setNerve(NrvPetitPorterForceEndCtrlForceEndPorter);
        }
    }

    void PetitPorterForceEndCtrl::exeForceEndPorter() {
        if (!mPetitPorter->isNerve(&NrvPetitPorter::PetitPorterNrvMinigame::sInstance)) {
            setNerve(NrvPetitPorterForceEndCtrlWaitForPorter);
            return;
        }

        if (MR::isGreaterEqualStep(this, mDelay)) {
            mPetitPorter->mSpine->mStep = mPetitPorter->mTimeLimit;
            MR::offSwitchA(this);
        }
    }

    // -----------------------------------------------------------------------------------------------------------------

    namespace NrvPetitPorterForceEndCtrl {
        NrvWaitForPorter(NrvWaitForPorter::sInstance);
        NrvForceEndPorter(NrvForceEndPorter::sInstance);

        void NrvWaitForPorter::execute(Spine *pSpine) const {
            PetitPorterForceEndCtrl *pActor = (PetitPorterForceEndCtrl*)pSpine->mExecutor;
            pActor->exeWaitForPorter();
        }

        void NrvForceEndPorter::execute(Spine *pSpine) const {
            PetitPorterForceEndCtrl *pActor = (PetitPorterForceEndCtrl*)pSpine->mExecutor;
            pActor->exeForceEndPorter();
        }
    }
}
