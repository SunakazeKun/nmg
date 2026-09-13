#include "Game/MapObj/DummyDisplayModel.h"
#include "Game/Util/ActorAnimUtil.h"
#include "Game/Util/JMapUtil.h"
#include "Kamek/hooks.h"

/*
* Author: Aurum
*
* New dummy items
*/

namespace pt {
    /*
    * New variants can be easily added and created here. This extends the existing set of dummy models. This replaces
    * all calls to MR::tryCreateDummyModel, but uses the original function to initialize the base game's items.
    */
    DummyDisplayModelInfo cNewDummyDisplayModels[] = {
        /* 15 */ { "PowerUpFire",       nullptr,    { 0.0f,  70.0f, 0.0f }, 16, nullptr, false },
        /* 16 */ { "PowerUpIce",        nullptr,    { 0.0f,  70.0f, 0.0f }, 16, nullptr, false },
        /* 17 */ { "PowerUpBee",        nullptr,    { 0.0f,  70.0f, 0.0f }, 16, nullptr, false },
        /* 18 */ { "PowerUpHopper",     nullptr,    { 0.0f,  70.0f, 0.0f }, 16, nullptr, false },
        /* 19 */ { "PowerUpTeresa",     nullptr,    { 0.0f,  70.0f, 0.0f }, 16, nullptr, false },
        /* 20 */ { "PowerUpFoo",        nullptr,    { 0.0f,  70.0f, 0.0f }, 16, nullptr, false },
        /* 21 */ { "PowerUpCloud",      nullptr,    { 0.0f,  70.0f, 0.0f }, 16, nullptr, false },
        /* 22 */ { "PowerUpRock",       nullptr,    { 0.0f,  70.0f, 0.0f }, 16, nullptr, false },
        /* 23 */ { "PowerUpInvincible", nullptr,    { 0.0f,  70.0f, 0.0f }, 16, nullptr, false },
        /* 24 */ { "EarthenPipe",       nullptr,    { 0.0f, 120.0f, 0.0f }, 16, nullptr, false },
        /* 25 */ { "LifeUpHeart",       "Dummy",    { 0.0f,  70.0f, 0.0f }, 16, nullptr, false },
    };
    const s32 cNewDummyDisplayModelsCount = sizeof(cNewDummyDisplayModels) / sizeof(DummyDisplayModelInfo);

    DummyDisplayModel* tryCreateNewDummyModel(LiveActor *pHost, const JMapInfoIter &rIter, s32 defaultId, int v4) {
        s32 modelId = MR::getDummyDisplayModelId(rIter, defaultId);

        if (modelId < 0 || modelId > 14 + cNewDummyDisplayModelsCount) {
            return nullptr;
        }

        if (modelId <= 14) {
            return MR::tryCreateDummyModel(pHost, rIter, modelId, v4);
        }

        s32 colorId = 0;

        if (MR::isValidInfo(rIter)) {
            MR::getJMapInfoArg6NoInit(rIter, &colorId);
        }

        DummyDisplayModelInfo *pInfo = &cNewDummyDisplayModels[modelId - 15];
        DummyDisplayModel *pModel = new DummyDisplayModel(pHost, pInfo, v4, modelId, colorId);
        pModel->initWithoutIter();

        // EarthenPipe doesn't use ColorChange.brk
        if (modelId == 24) {
            MR::startBrkAndSetFrameAndStop(pModel, "EarthenPipe", colorId);
        }

        return pModel;
    }

    // Hooks
    kmCall(0x80295BB0, tryCreateNewDummyModel);
    kmCall(0x80295C28, tryCreateNewDummyModel);
    kmCall(0x80295CAC, tryCreateNewDummyModel);
    kmCall(0x80295D04, tryCreateNewDummyModel);
    kmCall(0x80295D88, tryCreateNewDummyModel);

    // Skip repeated reading of Obj_arg7 field
    kmWrite32(0x80295464, 0x7CA32B78);
    kmWrite32(0x80295468, 0x60000000);
}
