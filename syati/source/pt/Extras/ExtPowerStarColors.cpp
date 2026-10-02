#include "pt/Extras/ExtPowerStarColors.h"
#include "Game/LiveActor/LiveActor.h"
#include "Game/System/AllData/GalaxyStatusAccessor.h"
#include "Game/System/ScenarioDataParser.h"
#include "Game/Util/SceneUtil.h"
#include "Game/Util/StringUtil.h"
#include "kamek/hooks.h"

/*
* Authors: Aurum
* 
* This changes all code that apply Power Star colors to support new and custom colors as well. To accomplish this, the
* PowerStarType in the scenario file is changed to easily configure the color for a star. The new types are Bronze, Red,
* Blue and LegacyGreen. As the Green type from the base game has to be unlocked first, the LegacyGreen type was added in
* to ignore these conditions effectively.
*/

namespace pt {
    struct PowerStarColorInfo {
        const char* mType;
        s32 mFrame;
        int mCode;
    };

    const PowerStarColorInfo cPowerStarColorInfos[] = {
        { "Green",       2, 0x4F },
        { "LegacyGreen", 2, 0x82 },
        { "Red",         3, 0x80 },
        { "Blue",        5, 0x83 },
    };

    void getPowerStarColorInfo(const char *pStageName, s32 scenarioId, s32 *pFrame, int *pCode) {
        const char* pType;
        GalaxyStatusAccessor gsa(MR::makeGalaxyStatusAccessor(pStageName));
        gsa.mScenarioData->getScenarioString("PowerStarType", scenarioId, &pType);

        for (int i = 0; i < ARRAY_SIZE(cPowerStarColorInfos); i++) {
            const PowerStarColorInfo& info = cPowerStarColorInfos[i];

            if (MR::isEqualString(info.mType, pType)) {
                if (pFrame) {
                    *pFrame = info.mFrame;
                }
                if (pCode) {
                    *pCode = info.mCode;
                }
                return;
            }
        }

        if (pFrame) {
            *pFrame = 0;
        }
        if (pCode) {
            *pCode = 0x37;
        }
    }

    // -----------------------------------------------------------------------------------------------------------------

    namespace {
        s32 getPowerStarColor(const char *pStageName, s32 scenarioId) {
            s32 frame;
            getPowerStarColorInfo(pStageName, scenarioId, &frame, nullptr);
            return frame;
        }

        s32 getPowerStarColorCurrentStage(s32 scenarioId) {
            s32 frame;
            getPowerStarColorInfo(MR::getCurrentStageName(), scenarioId, &frame, nullptr);
            return frame;
        }

        // PowerStar::initMapToolInfo
        kmWrite32(0x802E01B8, PPC_MR(3, 0));
        kmCall(0x802E01BC, getPowerStarColorCurrentStage);
        kmWrite32(0x802E01C0, PPC_STW(3, 0x130, 30));
        kmWrite32(0x802E01F0, PPC_LWZ(3, 0x130, 30));
        
        // PowerStar::getBtpFrameCurrentStage
        kmCall(0x802DFC14, getPowerStarColorCurrentStage);
        kmWrite32(0x802DFC20, PPC_NOP);
        
        // Star Return cutscene
        kmCall(0x802DF0FC, getPowerStarColor);
        kmWrite32(0x802DF100, PPC_MR(28, 3));
        kmWrite32(0x802DF104, PPC_CMPWI(28, 0));
        kmWrite32(0x802DF108, PPC_COND_BRANCH(PPC_COND_EQ, 8));
        
        // Cutscene/dummy models
        kmWrite32(0x802DF01C, PPC_MR(3, 27));
        kmCall(0x802DF020, getPowerStarColorCurrentStage);
        kmWrite32(0x802DF024, PPC_MR(29, 3));
        kmWrite32(0x802DF028, PPC_B(0x14));

        // -------------------------------------------------------------------------------------------------------------

        void getPowerStarIcon(wchar_t* pStr, u32 fontCode) {
            register const char* stageName;
            register s32 scenarioId;

            __asm {
                mr stageName, r27;
                mr scenarioId, r31;
            }

            int code;
            getPowerStarColorInfo(stageName, scenarioId, nullptr, &code);
            MR::addPictureFontCode(pStr, code);
        }

        kmCall(0x80041E30, getPowerStarIcon);
    }
}
