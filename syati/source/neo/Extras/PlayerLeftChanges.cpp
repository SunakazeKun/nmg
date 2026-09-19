#include "neo/System/DaredevilModeAccess.h"
#include "Game/MapObj/BenefitItemLifeUp.h"
#include "Game/Util/PlayerUtil.h"
#include "Game/Util/LayoutUtil.h"
#include "kamek/hooks.h"

namespace {
    // =================================================================================================================
    // Helper functions
    // =================================================================================================================

    void hidePaneRecursive(LayoutActor* pLayout, const char* pPaneName) {
        MR::hidePaneRecursive(pLayout, pPaneName);
    }

    void changePlayerItemLifeUp() {
        MR::changePlayerItemLifeUp();
    }

    BenefitItemLifeUp* createBenefitItemLifeUp() {
        return new BenefitItemLifeUp("スーパーキノコ");
    }


    // =================================================================================================================
    // Hide 1-Up counters
    // =================================================================================================================

    // ScenarioSelectLayout
#if defined(SB4P) || defined(SB4E) || defined(SB4J)
    kmCall(0x8048EB18, hidePaneRecursive);
    kmCall(0x8048EB2C, hidePaneRecursive);
#elif defined(SB4W) || defined(SB4K)
    kmCall(0x8048EB28, hidePaneRecursive);
    kmCall(0x8048EB3C, hidePaneRecursive);
#endif

    // PauseMenu
#if defined(SB4P) || defined(SB4E) || defined(SB4J)
    kmCall(0x80487154, hidePaneRecursive);
#elif defined(SB4W) || defined(SB4K)
    kmCall(0x80487164, hidePaneRecursive);
#endif


    // =================================================================================================================
    // Disable PlayerLeft layout
    // =================================================================================================================

#if defined(SB4P) || defined(SB4E) || defined(SB4J)
    kmWrite32(0x804A8E20, PPC_B(0x20));
    kmWrite32(0x804A8E78, PPC_NOP);
    kmWrite32(0x804A8F04, PPC_B(0x14));
    kmWrite32(0x804A8F68, PPC_NOP);
    kmWrite32(0x804A8FDC, PPC_B(0x14));
    kmWrite32(0x804A9060, PPC_B(0x24));
    kmWrite32(0x804A9214, PPC_B(0x3C));
#elif defined(SB4W) || defined(SB4K)
    kmWrite32(0x804A8E90, PPC_B(0x20));
    kmWrite32(0x804A8EE8, PPC_NOP);
    kmWrite32(0x804A8F74, PPC_B(0x14));
    kmWrite32(0x804A8FD8, PPC_NOP);
    kmWrite32(0x804A904C, PPC_B(0x14));
    kmWrite32(0x804A90D0, PPC_B(0x24));
    kmWrite32(0x804A9284, PPC_B(0x3C));
#endif


    // =================================================================================================================
    // Disable PlayerLeftMiss layout
    // =================================================================================================================

#if defined(SB4P) || defined(SB4E) || defined(SB4J)
    kmWrite32(0x804D9200, PPC_NOP);
    kmWrite32(0x804B325C, PPC_NOP);
    kmWrite32(0x804B43A4, PPC_B(0x58));
#elif defined(SB4W) || defined(SB4K)
    kmWrite32(0x804D9290, PPC_NOP);
    kmWrite32(0x804B32CC, PPC_NOP);
    kmWrite32(0x804B4414, PPC_B(0x58));
#endif


    // =================================================================================================================
    // Acquirement changes
    // =================================================================================================================

    // GameDataPlayerStatus::addPlayerLeft stub
#if defined(SB4P) || defined(SB4E) || defined(SB4J)
    kmWrite32(0x804DE030, PPC_BLR);
#elif defined(SB4W) || defined(SB4K)
    kmWrite32(0x804DE0C0, PPC_BLR);
#endif

    // MR::addPlayerLeft stub
    kmWrite32(0x80023890, PPC_BLR);

    // Prevent SE_SY_1UP/5UP sounds
    kmWrite32(0x80059970, PPC_BLR);

    // MR::addStarPiece doesn't grant 1-Ups
    kmWrite32(0x80023A30, PPC_B(0x24));

    // MarioLauncherTarget grants a Life-Up instead
    kmCall(0x802CFC4C, changePlayerItemLifeUp);

    // YoshiFruit grants a Life-Up instead
    kmCall(0x8041793C, changePlayerItemLifeUp);

    // Disable Bank Toad 1-Up check
    kmWrite32(0x8034EC20, PPC_LI(3, 0));
    kmWrite32(0x8034EC24, PPC_B(0xAC));

    // Disable Bank Toad 1-Up reward
    kmWrite32(0x8034EE54, PPC_B(0x38));


    // =================================================================================================================
    // Actor replacements
    // =================================================================================================================

    // ItemDice creates Life Mushroom
    kmWrite32(0x802B4978, PPC_B(0x18)); // skip garbage
    kmCall(0x802B4990, createBenefitItemLifeUp);

    // TreasureBoxCracked creates Life Mushroom
    kmCall(0x8032561C, createBenefitItemLifeUp);
    kmWrite32(0x80325620, PPC_MR(31, 3)); // r31 = BenefitItemLifeUp*
    kmWrite32(0x80325624, PPC_B(0x2C)); // skip garbage
}
