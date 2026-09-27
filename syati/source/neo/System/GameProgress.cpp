#include "neo/System/GameProgress.h"
#include "neo/System/IllustItemAccess.h"
#include "Game/System/GameData/UserFile.h"
#include "Game/System/AllData/GameDataFunction.h"
#include "kamek/hooks.h"
#include "neo/core.h"

// ---------------------------------------------------------------------------------------------------------------------

#define TOTAL_NORMAL_STARS (21)
#define TOTAL_GREEN_STARS  (21)
#define TOTAL_POWER_STARS  (TOTAL_NORMAL_STARS + TOTAL_GREEN_STARS)
#define TOTAL_COMET_MEDALS (8)
#define TOTAL_STAMPS       (8)

#define PRANKSTER_COMETS_CHECK_POWER_STARS_WORLD (1)
#define PRANKSTER_COMETS_CHECK_POWER_STARS_COUNT (5)

#define FINAL_GALAXY_CHECK_STAMPS_COUNT (TOTAL_STAMPS - 1)

#define GREEN_STARS_CHECK_POWER_STARS_COUNT  (TOTAL_NORMAL_STARS)
#define GREEN_STARS_CHECK_COMET_MEDALS_COUNT (TOTAL_COMET_MEDALS)

// ---------------------------------------------------------------------------------------------------------------------

namespace neo {
    GameProgress getGameProgressType(const UserFile *pUserFile) {
        s32 normalStars = pUserFile->calcNormalPowerStarNum();
        s32 totalStars = pUserFile->getPowerStarNum();
        s32 medals, stamps;

        calcUserFileCollectibles(pUserFile, &medals, &stamps);

        if (totalStars >= TOTAL_POWER_STARS) {
            if (medals >= TOTAL_COMET_MEDALS && stamps >= TOTAL_STAMPS) {
                return GameProgress_AllComplete;
            }
            else {
                return GameProgress_AllStars;
            }
        }

        if (pUserFile->mGameDataHolder->isOnGameEventFlag("スター120個エンディング実行")) {
            return GameProgress_GreenStars;
        }

        return GameProgress_NormalStars;
    }

    void calcUserFileCollectibles(const UserFile *pUserFile, s32 *pMedals, s32 *pStamps) {
        GameDataAllGalaxyStorage* allGalaxyStorage = pUserFile->mGameDataHolder->mAllGalaxyStorage;
        s32 medals = 0;
        s32 stamps = 0;

        for (int i = 0; i < allGalaxyStorage->mNumGalaxies; i++) {
            u8 flags = allGalaxyStorage->mGalaxyStorages[i]->mFlag;

            if (flags & SOME_GALAXY_STORAGE_FLAG_TICO_COIN) {
                medals++;
            }

            if (flags & SOME_GALAXY_STORAGE_FLAG_ILLUST_ITEM) {
                stamps++;
            }
        }

        *pMedals = medals;
        *pStamps = stamps;
    }

    bool checkUserFileAllowRosettaMiiIcon(const UserFile *pUserFile) {
        GameDataAllGalaxyStorage* allGalaxyStorage = pUserFile->mGameDataHolder->mAllGalaxyStorage;
        return allGalaxyStorage->makeAccessor(STAGE_COMET_OBSERVATORY)->getScenarioAccessor(2)->mFlag & 1;
    }

    
    namespace {
        // =============================================================================================================
        // Stage selected after game restart
        // =============================================================================================================

        // No Power Stars collected -> Starship Mario
    #if defined(SB4_JEP)
        kmWrite32(0x804D6314, PPC_COND_BRANCH(PPC_COND_NE, 0x58));
    #elif defined(SB4_WK)
        kmWrite32(0x804D63A4, PPC_COND_BRANCH(PPC_COND_NE, 0x58));
    #endif

        // VsKoopaLv3Galaxy cleared -> Starship Mario
    #if defined(SB4_JEP)
        kmWrite32(0x804D66A8, PPC_COND_BRANCH(PPC_COND_EQ, 0x7C));
    #elif defined(SB4_WK)
        kmWrite32(0x804D6738, PPC_COND_BRANCH(PPC_COND_EQ, 0x7C));
    #endif

    
        // =============================================================================================================
        // Skippable intro after collecting a Power Star
        // =============================================================================================================

        // GameDataFunction::setPowerStar
#if defined(SB4_JEP)
        kmWrite32(0x804D254C, 0x60000005);
#elif defined(SB4_WK)
        kmWrite32(0x804D25DC, 0x60000005);
#endif


        // =============================================================================================================
        // Prankster Comet unlock criteria
        // =============================================================================================================

        // Required Power Stars in specific World
#if defined(SB4_JEP)
        kmWrite16(0x804D8952, PRANKSTER_COMETS_CHECK_POWER_STARS_WORLD);
        kmWrite16(0x804D895A, PRANKSTER_COMETS_CHECK_POWER_STARS_COUNT);
#elif defined(SB4_WK)
        kmWrite16(0x804D89E2, PRANKSTER_COMETS_CHECK_POWER_STARS_WORLD);
        kmWrite16(0x804D89EA, PRANKSTER_COMETS_CHECK_POWER_STARS_COUNT);
#endif

        // Disable hardcoded check for "Mimic" comets
#if defined(SB4_JEP)
        kmWrite32(0x804D8B80, PPC_LI(3, 1));
#elif defined(SB4_WK)
        kmWrite32(0x804D8C10, PPC_LI(3, 1));
#endif

        // Disable hardcoded check for "Purple" comets
#if defined(SB4_JEP)
        kmWrite32(0x804D8C44, PPC_LI(3, 1));
#elif defined(SB4_WK)
        kmWrite32(0x804D8CD4, PPC_LI(3, 1));
#endif


        // =============================================================================================================
        // Final Galaxy unlock criteria
        // =============================================================================================================

        bool checkUnlockFinalGalaxy() {
            return neo::calcIllustItemNum() >= FINAL_GALAXY_CHECK_STAMPS_COUNT;
        }

#if defined(SB4_JEP)
        kmCall(0x804D8878, checkUnlockFinalGalaxy);
#elif defined(SB4_WK)
        kmCall(0x804D8908, checkUnlockFinalGalaxy);
#endif


        // =============================================================================================================
        // Green Stars unlock critera
        // =============================================================================================================

        void tryUnlockGreenStars() {
            if (GameDataFunction::calcCurrentPowerStarNum() < GREEN_STARS_CHECK_POWER_STARS_COUNT) {
                return;
            }
            if (GameDataFunction::calcTicoCoinNum() < GREEN_STARS_CHECK_COMET_MEDALS_COUNT) {
                return;
            }

            GameDataFunction::onGameEventFlag("スター120個エンディング実行");
        }

#if defined(SB4_JEP)
        kmBranch(0x804D88E8, tryUnlockGreenStars);
#elif defined(SB4_WK)
        kmBranch(0x804D8978, tryUnlockGreenStars);
#endif
    }
}
