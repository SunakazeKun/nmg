#include "Game/NameObj/NameObjFactory.h"
#include "Game/Util/StringUtil.h"
#include "neo/System/DaredevilModeAccess.h"
#include "kamek/hooks.h"

#include "Game/MapObj/JetTurtle.h"
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
#include "neo/MapObj/PetitPorterForceEndCtrl.h"
#include "neo/MapObj/SpiderCocoon.h"
#include "neo/MapObj/StringSpiderAttachPoint.h"
#include "neo/NPC/Butler.h"
#include "neo/NPC/HelperWitch.h"
#include "neo/NPC/LibraryBook.h"
#include "neo/NPC/TicoManiac.h"

#ifdef DEBUG
#include "Game/Util/GamePadUtil.h"
#include "gst/GhostLuigiRecordHelper.h"
#endif

class TakoSho;
class MorphItemCollection;
class Shellfish;

namespace {
    template<typename T>
    NameObj* createExtActor(const char* pName) {
        return new T(pName);
    }

    template<>
    NameObj* createExtActor<pt::MorphItemNeoFoo>(const char *pName) {
        return new pt::MorphItemNeoFoo("飛行オブジェ");
    }

    template<>
    NameObj* createExtActor<pt::MorphItemNeoIce>(const char *pName) {
        return new pt::MorphItemNeoIce("アイスオブジェ");
    }

    template<>
    NameObj* createExtActor<pt::PowerStarSpawner>(const char *pName) {
        return new pt::PowerStarSpawner("PowerStarSpawner");
    }


    // =================================================================================================================
    // Creator table extension
    // =================================================================================================================

    const CreateActorEntry cNewCreateNameObjTable[] = {
        // -------------------------------------------------
        // PROJECT TEMPLATE
        // -------------------------------------------------
        
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


        // -------------------------------------------------
        // NEO MARIO GALAXY
        // -------------------------------------------------
        
        // AreaObj
        { "AstroWarpArea", createExtActor<neo::AstroWarpArea> },

        // Enemy
        { "SkeletalFish", createExtActor<neo::SkeletalFish> },
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
        { "PetitPorterForceEndCtrl", createExtActor<neo::PetitPorterForceEndCtrl> },
        { "IllustItem", createExtActor<neo::IllustItem> },
        { "KinokoStar", neo::createKinokoStar },
        { "KinokoTreasure", neo::createKinokoTreasure },
        { "LifeUpHeart", neo::createLifeUpHeart },

        // NPC
        { "Butler", createExtActor<neo::Butler> },
        { "HelperWitch", createExtActor<neo::HelperWitch> },
        { "LibraryBook", createExtActor<neo::LibraryBook> },
        { "TicoManiac", createExtActor<neo::TicoManiac> },
    };


    // =================================================================================================================
    // Prevent creation in Daredevil Mode
    // 
    // Make sure that these are never used in ChildObj setups!
    // =================================================================================================================

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


    // =================================================================================================================
    // NameObjFactory extension
    // =================================================================================================================

    CreateActorFunc* getNameObjCreator(const char *pName) {
        CreateActorFunc* creator = NameObjFactory::getCreator(pName);

#if DEBUG
        if (MR::isEqualString(pName, "GhostAttackGhost") && MR::testCorePadButtonPlus(0)) {
            return createExtActor<gst::GhostLuigiRecordHelper>;
        }
#endif

        if (!creator) {
            for (s32 i = 0; i < ARRAY_SIZE(cNewCreateNameObjTable); i++) {
                const CreateActorEntry entry = cNewCreateNameObjTable[i];

                if (MR::isEqualString(entry.pActorName, pName)) {
                    creator = entry.mCreationFunc;
                    break;
                }
            }
        }

        if (neo::isDaredevilMode()) {
            for (s32 i = 0; i < ARRAY_SIZE(cDaredevilIgnoreObjs); i++) {
                if (MR::isEqualString(cDaredevilIgnoreObjs[i], pName)) {
                    return nullptr;
                }
            }
        }

        return creator;
    }

    kmCall(0x803394C0, getNameObjCreator);
    kmCall(0x803430C8, getNameObjCreator);
    kmCall(0x804564D4, getNameObjCreator);
}
