#include "Game/Scene/SceneObjHolder.h"
#include "pt/MapObj/BlueChip.h"
#include "kamek/hooks.h"

namespace {
    struct CreateSceneObjEntry {
        s32 mSlotId;
        NameObj* (*mCreationFunc)(void);
    };


    // =================================================================================================================
    // Entry table
    // =================================================================================================================

    // There is a bunch of free SceneObj slots that we can still use as desired.
    // Unused slots: 0x3E, 0x47, 0x48, 0x49, 0x54, 0x5A, 0x68, 0x69, 0x7B

    const CreateSceneObjEntry cNewCreateSceneObjTable[] = {
        { SCENE_OBJ_BLUE_CHIP_HOLDER, pt::createBlueChipHolder }
    };

    const s32 cNewCreateSceneObjTableCount = sizeof(cNewCreateSceneObjTable) / sizeof(CreateSceneObjEntry);


    // =================================================================================================================
    // SceneObjHolder extension
    // =================================================================================================================

    NameObj* createSceneObj(SceneObjHolder *pHolder, int type) {
        NameObj* sceneObj = pHolder->newEachObj(type);

        if (!sceneObj) {
            for (s32 i = 0; i < cNewCreateSceneObjTableCount; i++) {
                const CreateSceneObjEntry entry = cNewCreateSceneObjTable[i];

                if (entry.mSlotId == type) {
                    sceneObj = entry.mCreationFunc();
                    break;
                }
            }
        }

        return sceneObj;
    }

    kmCall(0x804599D0, createSceneObj);
}
