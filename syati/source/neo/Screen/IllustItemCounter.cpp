#include "neo/Screen/IllustItemCounter.h"
#include "neo/System/IllustItemAccess.h"
#include "Game/Screen/CounterLayoutAppearer.h"
#include "Game/Util/LayoutUtil.h"
#include "Game/Util/ObjUtil.h"
#include "Game/Util/PlayerUtil.h"
#include "Game/Util/SceneUtil.h"

// Syati's header for this class has faulty includes
class CounterLayoutController : public LayoutActor {
public:
	static bool isWaitToDisappearCounter(const LayoutActor* pLayout);
};

namespace neo {
    IllustItemCounter::IllustItemCounter()
		: LayoutActor("IllustItemCounter", true)
	{
        mAppearer = nullptr;
        mAlreadyShown = false;
        mForceAppear = false;
    }

    void IllustItemCounter::init(const JMapInfoIter &rIter) {
        initLayoutManager("IllustItemCounter", 3);
        initNerve(NrvIllustItemCounterHidden);
        MR::connectToSceneLayout(this);

        mAppearer = new CounterLayoutAppearer(this, TVec2f(-50.0f, 0.0f));

        if (MR::getCurrentStagePowerStarNum() != 0) {
            if (neo::isOnGalaxyFlagIllustItem(MR::getCurrentStageName()) || neo::isIllustItemInTempStageGet()) {
                MR::startAnimAndSetFrameAndStop(this, "Icon", 1.0f, 2);
                mAlreadyShown = true;
                return;
            }
        }

        MR::startAnimAndSetFrameAndStop(this, "Icon", 0.0f, 2);
        mAlreadyShown = false;
    }

    void IllustItemCounter::appear() {
        if (MR::getCurrentStagePowerStarNum() != 0) {
            mAppearer->reset();
            setNerve(NrvIllustItemCounterHidden);
            LayoutActor::appear();

            mForceAppear = false;
            MR::requestMovementOn(this);
        }
    }

    void IllustItemCounter::control() {
        mAppearer->updateNerve();

        if (!mAlreadyShown && neo::isIllustItemInTempStageCollected()) {
            if (!isNerve(NrvIllustItemCounterFlash) && !isNerve(NrvIllustItemCounterAppear)) {
                if (isNerve(NrvIllustItemCounterWait)) {
                    setNerve(NrvIllustItemCounterFlash);
                }
                else {
                    setNerve(NrvIllustItemCounterAppear);
                }
            }
        }
    }

    void IllustItemCounter::forceAppear() {
        if (!isNerve(NrvIllustItemCounterWait) && !isNerve(NrvIllustItemCounterFlash)) {
            appear();
            setNerve(NrvIllustItemCounterAppear);
        }

        mForceAppear = true;
    }

    void IllustItemCounter::disappear() {
        mForceAppear = false;
        setNerve(NrvIllustItemCounterDisappear);
    }

    bool IllustItemCounter::isWait() {
        return isNerve(NrvIllustItemCounterWait) || isNerve(NrvIllustItemCounterFlash);
    }

    void IllustItemCounter::exeHidden() {
        if (MR::isFirstStep(this)) {
            MR::hideLayout(this);
        }
    }

    void IllustItemCounter::exeAppear() {
        if (MR::isFirstStep(this)) {
            MR::showLayout(this);
            mAppearer->appear(TVec2f(0.0f, 0.0f));
            MR::startAnim(this, "Wait", 1);
        }

        if (mAppearer->isAppeared()) {
            setNerve(NrvIllustItemCounterWait);
        }
    }

    void IllustItemCounter::exeWait() {
        if (!mForceAppear && mAlreadyShown && CounterLayoutController::isWaitToDisappearCounter(this)) {
            setNerve(NrvIllustItemCounterDisappear);
        }
    }

    void IllustItemCounter::exeFlash() {
        if (MR::isFirstStep(this)) {
            MR::startAnimAndSetFrameAndStop(this, "Icon", 1.0f, 2);
            MR::startAnim(this, "Flash", 0);
        }

        if (MR::isAnimStopped(this, nullptr)) {
            mAlreadyShown = true;
            setNerve(NrvIllustItemCounterWait);
        }
    }

    void IllustItemCounter::exeDisappear() {
        if (MR::isFirstStep(this)) {
            mAppearer->disappear();
        }

        if (mAppearer->isDisappeared()) {
            setNerve(NrvIllustItemCounterHidden);
        }
    }

	// -----------------------------------------------------------------------------------------------------------------

    namespace NrvIllustItemCounter {
		NrvHidden(NrvHidden::sInstance);
		NrvAppear(NrvAppear::sInstance);
		NrvWait(NrvWait::sInstance);
		NrvFlash(NrvFlash::sInstance);
		NrvDisappear(NrvDisappear::sInstance);

        void NrvHidden::execute(Spine *pSpine) const {
            IllustItemCounter *pActor = (IllustItemCounter*)pSpine->mExecutor;
            pActor->exeHidden();
        }

        void NrvAppear::execute(Spine *pSpine) const {
            IllustItemCounter *pActor = (IllustItemCounter*)pSpine->mExecutor;
            pActor->exeAppear();
        }

        void NrvWait::execute(Spine *pSpine) const {
            IllustItemCounter *pActor = (IllustItemCounter*)pSpine->mExecutor;
            pActor->exeWait();
        }

        void NrvFlash::execute(Spine *pSpine) const {
            IllustItemCounter *pActor = (IllustItemCounter*)pSpine->mExecutor;
            pActor->exeFlash();
        }

        void NrvDisappear::execute(Spine *pSpine) const {
            IllustItemCounter *pActor = (IllustItemCounter*)pSpine->mExecutor;
            pActor->exeDisappear();
        }
    }
}
