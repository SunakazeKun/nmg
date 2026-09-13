#include "neo/Map/ClippingDummyTarget.h"
#include "Game/Util/ActorInitUtil.h"
#include "Game/Util/ActorSwitchUtil.h"
#include "Game/Util/JMapUtil.h"
#include "Game/Util/LiveActorUtil.h"
#include "Game/Util/ObjUtil.h"

namespace neo {
    ClippingDummyTarget::ClippingDummyTarget(const char *pName)
        : LiveActor(pName)
    {

    }

    void ClippingDummyTarget::init(const JMapInfoIter &rIter) {
        MR::initDefaultPos(this, rIter);
        MR::connectToSceneMapObjMovement(this);

        f32 farClipDistance = -1.0f;
        f32 clippingRadius = 1000.0f;
        s32 clippingGroupSize = 0;

        MR::getJMapInfoArg0NoInit(rIter, &farClipDistance);
        MR::getJMapInfoArg1NoInit(rIter, &clippingRadius);
        MR::getJMapInfoArg2NoInit(rIter, &clippingGroupSize);

        MR::setClippingTypeSphere(this, clippingRadius);
        MR::setClippingFar(this, farClipDistance);

        if (clippingGroupSize > 0) {
            MR::setGroupClipping(this, rIter, clippingGroupSize);
        }

        MR::useStageSwitchAwake(this, rIter);
        MR::useStageSwitchSyncAppear(this, rIter);
    }
}
