#pragma once

#include "pt/Enemy/Anagon.h"
#include "pt/Enemy/BallBeamer.h"
#include "pt/Extras/ExtSuperSpinDriverGreen.h"
#include "pt/MapObj/BlueChip.h"
#include "pt/MapObj/NewMorphItemNeo.h"
#include "pt/MapObj/PowerStarSpawner.h"
#include "neo/AreaObj/AstroWarpArea.h"
#include "neo/Enemy/SkeletalFish.h"
#include "neo/Map/ClippingDummyTarget.h"
#include "neo/MapObj/AstroCore.h"
#include "neo/MapObj/AstroLibrary.h"
#include "neo/MapObj/HipDropNail.h"
#include "neo/MapObj/IllustItem.h"
#include "neo/MapObj/KinokoCollect.h"
#include "neo/MapObj/MarblePlanet.h"
#include "neo/MapObj/SpiderCocoon.h"
#include "neo/MapObj/StringSpiderAttachPoint.h"
#include "neo/NPC/Butler.h"
#include "neo/NPC/HelperWitch.h"
#include "neo/NPC/LibraryBook.h"
#include "neo/NPC/TicoManiac.h"

class Meramera;
class TakoSho;

namespace {
    template<typename T>
    NameObj* createExtActor(const char *pName) {
        return new T(pName);
    }

    template<>
    NameObj* createExtActor<pt::MorphItemNeoFoo>(const char *pName);

    template<>
    NameObj* createExtActor<pt::MorphItemNeoIce>(const char *pName);

    template<>
    NameObj* createExtActor<pt::PowerStarSpawner>(const char *pName);

    // ----------------------------------------------------------------------------------------------------------------

    const CreateActorEntry cNewCreateNameObjTable[] = {
        // PROJECT TEMPLATE
        // AreaObj
        { "ExtraWallCheckCylinder", NameObjFactory::createBaseOriginCylinder<AreaObj> },
        { "ForbidJumpArea", NameObjFactory::createNameObj<AreaObj> },
        { "ForbidWaterSearchArea", NameObjFactory::createNameObj<AreaObj> },
        { "PipeModeCube", NameObjFactory::createBaseOriginCube<AreaObj> },
        { "PlaneCircularModeCube", NameObjFactory::createBaseOriginCube<AreaObj> },
        
        // Enemy
        { "Anagon", createExtActor<pt::Anagon> },
        { "BallBeamer", createExtActor<pt::BallBeamer> },

        // MapObj
        { "BlueChip", createExtActor<pt::BlueChip> },
        { "BlueChipGroup", createExtActor<pt::BlueChipGroup> },
        { "GoldenTurtle", createExtActor<GoldenTurtle> },
        { "MorphItemCollectionFoo", NameObjFactory::createNameObj<MorphItemCollection> },
        { "MorphItemCollectionIce", NameObjFactory::createNameObj<MorphItemCollection> },
        { "MorphItemNeoFoo", createExtActor<pt::MorphItemNeoFoo> },
        { "MorphItemNeoIce", createExtActor<pt::MorphItemNeoIce> },
        { "PowerStarSpawner", createExtActor<pt::PowerStarSpawner> },
        { "ShellfishBlueChip", NameObjFactory::createNameObj<Shellfish> },
        { "ShellfishPurpleCoin", NameObjFactory::createNameObj<Shellfish> },
        { "SuperSpinDriverGreen", pt::createSuperSpinDriverGreen },


        // NEO MARIO GALAXY
        // AreaObj
        { "AstroWarpArea", createExtActor<neo::AstroWarpArea> },

        // Enemy
        { "SkeletalFish", createExtActor<neo::SkeletalFish> },
        { "Chokomera", NameObjFactory::createNameObj<Meramera> },
        { "TakoKin", NameObjFactory::createNameObj<TakoSho> },

        // Map
        { "ClippingDummyTarget", createExtActor<neo::ClippingDummyTarget> },

        // MapObj
        { "AstroCore", createExtActor<neo::AstroCore> },
        { "AstroLibrary", createExtActor<neo::AstroLibrary> },
        { "MarblePlanet", createExtActor<neo::MarblePlanet> },
        { "SpiderCocoon", createExtActor<neo::SpiderCocoon> },
        { "StringSpiderAttachPoint", createExtActor<neo::StringSpiderAttachPoint> },
        { "HipDropNail", createExtActor<neo::HipDropNail> },
        { "HipDropNailWatcher", createExtActor<neo::HipDropNailWatcher> },
        { "IllustItem", createExtActor<neo::IllustItem> },
        { "KinokoStar", neo::createKinokoStar },
        { "KinokoTreasure", neo::createKinokoTreasure },
        { "LifeUpHeart", neo::createLifeUpHeart },

        // NPC
        { "Butler", createExtActor<neo::Butler> },
        { "HelperWitch", createExtActor<neo::HelperWitch> },
        { "LibraryBook", createExtActor<neo::LibraryBook> },
        { "TicoManiac", createExtActor<neo::TicoManiac> }
    };

    const s32 cNewCreateNameObjTableCount = sizeof(cNewCreateNameObjTable) / sizeof(CreateActorEntry);

    // Some objects are not used during Daredevil Mode. Make sure that these are never used in ChildObj setups!!!
    const char* cDaredevilIgnoreObjs[] = {
        "BenefitItemLifeUp",
        "LifeUpHeart",
        "RestartObj",

        "ItemDice",
        "LuckyDice",
        "PositiveDice",
        "TreasureBoxCrackedKinokoLifeUp",
        "TreasureBoxKinokoLifeUp",

        "Dreamer",
        "SuperDreamer",
        "TicoShop",
        "TicoShopDice"
    };

    const s32 cDaredevilIgnoreObjsCount = sizeof(cDaredevilIgnoreObjs) / sizeof(const char*);
};
