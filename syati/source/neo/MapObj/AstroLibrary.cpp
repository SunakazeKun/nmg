#include "neo/MapObj/AstroLibrary.h"
#include "Game/Util/ActorAnimUtil.h"
#include "Game/Util/ActorInitUtil.h"
#include "Game/Util/ActorSensorUtil.h"
#include "Game/Util/ActorSwitchUtil.h"
#include "Game/Util/JMapUtil.h"
#include "Game/Util/LiveActorUtil.h"
#include "Game/Util/ModelUtil.h"
#include "Game/Util/ObjUtil.h"

namespace neo {
    AstroLibrary::AstroLibrary(const char *pName)
        : LiveActor(pName)
    {

    }

    void AstroLibrary::init(const JMapInfoIter &rIter) {
        MR::processInitFunction(this, rIter, false);
        MR::initDefaultPos(this, rIter);
        MR::connectToSceneMapObj(this);
        MR::setClippingTypeSphereContainsModelBoundingBox(this, 400.0f);

        initHitSensor(1);
        HitSensor *bodySensor = MR::addBodyMessageSensorMapObj(this);

        bool isOpen = false;
        MR::getJMapInfoArg0NoInit(rIter, &isOpen);

        if (isOpen) {
            MR::initCollisionPartsAutoEqualScale(this, "Open", bodySensor, nullptr);
            MR::hideMaterial(this, "DoorDummyMat_v");
        }
        else {
            MR::initCollisionPartsAutoEqualScale(this, "Close", bodySensor, nullptr);
            MR::startBtk(this, "Close");
        }

        MR::useStageSwitchAwake(this, rIter);
        makeActorAppeared();
    }
}
