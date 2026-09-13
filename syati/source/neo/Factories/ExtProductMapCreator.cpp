#include "Game/Map/ProductMapCreator.h"
#include "Game/Util/StringUtil.h"
#include "kamek/hooks.h"
#include "neo/Effect/EffectObjLine.h"
#include "neo/MapObj/PhantomShipBridge.h"
#include "pt/MapObj/TransparentWall.h"

class AirFar100m;
class AssemblyBlock;
class BallOpener;
class BeeJumpBall;
class BlueStarCupsulePlanet;
class GoroRockLaneParts;
class GravityFallBlock;
class InvisiblePolygonObj;
class JumpHole;
class ManholeCover;
class RailMoveTimerObj;
class RepeatTimerSwitchingBlock;
class Shellfish;
class SimpleEnvironmentObj;
class SimpleTextureSwitchChangeObj;
class WoodBox;

namespace {
    template<typename T>
    NameObj* createExtActor(const char* pName) {
        return new T(pName);
    }


    // =================================================================================================================
    // Creator table extension
    // =================================================================================================================

    const CreateActorEntry cNewCreateMapObjTable[] = {
        { "AirFar100m", NameObjFactory::createNameObj<AirFar100m> },
        { "AssemblyBlock", NameObjFactory::createNameObj<AssemblyBlock> },
        { "BallOpener", NameObjFactory::createNameObj<BallOpener> },
        { "BeeJumpBall", NameObjFactory::createNameObj<BeeJumpBall> },
        // { "ChooChooTrain", createExtActor<ChooChooTrain> },
        { "EffectObjLine", createExtActor<neo::EffectObjLine> },
        { "GoroRockLaneParts", NameObjFactory::createNameObj<GoroRockLaneParts> },
        { "GravityFallBlock", NameObjFactory::createNameObj<GravityFallBlock> },
        { "InvisiblePolygonObj", NameObjFactory::createNameObj<InvisiblePolygonObj> },
        { "JumpHole", NameObjFactory::createNameObj<JumpHole> },
        { "ManholeCover", NameObjFactory::createNameObj<ManholeCover> },
        { "PhantomShipBridge", createExtActor<neo::PhantomShipBridge> },
        { "RailMoveTimerObj", NameObjFactory::createNameObj<RailMoveTimerObj> },
        { "RepeatTimerSwitchingBlock", NameObjFactory::createNameObj<RepeatTimerSwitchingBlock> },
        // { "ScaleMapObj", createExtActor<LavaBallRisingPlanetLava> },
        { "SimpleEnvironmentObj", NameObjFactory::createNameObj<SimpleEnvironmentObj> },
        { "SimpleTextureSwitchChangeObj", NameObjFactory::createNameObj<SimpleTextureSwitchChangeObj> },
        { "TransparentWall", createExtActor<pt::TransparentWall> },
        { "WoodBox", NameObjFactory::createNameObj<WoodBox> }
    };

    const s32 cNewCreateMapObjTableCount = sizeof(cNewCreateMapObjTable) / sizeof(CreateActorEntry);


    // =================================================================================================================
    // ProductMapCreator extension
    // =================================================================================================================

    CreateActorFunc* getProductMapCreator(ProductMapCreator *pFactory, const char *pName) {
        CreateActorFunc* creator = pFactory->getProductMapCreator(pName);

        if (!creator) {
            for (s32 i = 0; i < cNewCreateMapObjTableCount; i++) {
                const CreateActorEntry entry = cNewCreateMapObjTable[i];

                if (MR::isEqualString(entry.pActorName, pName)) {
                    creator = entry.mCreationFunc;
                    break;
                }
            }
        }

        return creator;
    }

    kmCall(0x8026305C, getProductMapCreator);
}
