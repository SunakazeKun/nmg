#include "neo/System/DaredevilModeAccess.h"
#include "Game/MapObj/BenefitItemLifeUp.h"
#include "Game/Util/PlayerUtil.h"
#include "Game/Util/LayoutUtil.h"
#include "kamek/hooks.h"
#include "neo/core.h"

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
    kmWrite32(0x804A8E20, 0x48000020);
    kmWrite32(0x804A8E78, NOP);
    kmWrite32(0x804A8F04, 0x48000014);
    kmWrite32(0x804A8F68, NOP);
    kmWrite32(0x804A8FDC, 0x48000014);
    kmWrite32(0x804A9060, 0x48000024);
    kmWrite32(0x804A9214, 0x4800003C);
#elif defined(SB4W) || defined(SB4K)
    kmWrite32(0x804A8E90, 0x48000020);
    kmWrite32(0x804A8EE8, NOP);
    kmWrite32(0x804A8F74, 0x48000014);
    kmWrite32(0x804A8FD8, NOP);
    kmWrite32(0x804A904C, 0x48000014);
    kmWrite32(0x804A90D0, 0x48000024);
    kmWrite32(0x804A9284, 0x4800003C);
#endif


    // =================================================================================================================
    // Disable PlayerLeftMiss layout
    // =================================================================================================================

#if defined(SB4P) || defined(SB4E) || defined(SB4J)
    kmWrite32(0x804D9200, NOP);
    kmWrite32(0x804B325C, NOP);
    kmWrite32(0x804B43A4, 0x48000058);
#elif defined(SB4W) || defined(SB4K)
    kmWrite32(0x804D9290, NOP);
    kmWrite32(0x804B32CC, NOP);
    kmWrite32(0x804B4414, 0x48000058);
#endif


    // =================================================================================================================
    // Acquirement changes
    // =================================================================================================================

    // GameDataPlayerStatus::addPlayerLeft stub
#if defined(SB4P) || defined(SB4E) || defined(SB4J)
    kmWrite32(0x804DE030, BLR); 
#elif defined(SB4W) || defined(SB4K)
    kmWrite32(0x804DE0C0, BLR);
#endif

    // MR::addPlayerLeft stub
    kmWrite32(0x80023890, BLR);

    // Prevent SE_SY_1UP/5UP sounds
    kmWrite32(0x80059970, BLR);

    // MR::addStarPiece doesn't grant 1-Ups
    kmWrite32(0x80023A30, 0x48000024);

    // MarioLauncherTarget grants a Life-Up instead
    kmCall(0x802CFC4C, changePlayerItemLifeUp);

    // YoshiFruit grants a Life-Up instead
    kmCall(0x8041793C, changePlayerItemLifeUp);

    // Disable Bank Toad 1-Up check
    kmWrite32(0x8034EC20, 0x38600000);
    kmWrite32(0x8034EC24, 0x480000AC);

    // Disable Bank Toad 1-Up reward
    kmWrite32(0x8034EE54, 0x48000038);


    // =================================================================================================================
    // Actor replacements
    // =================================================================================================================

    // ItemDice creates Life Mushroom
    kmWrite32(0x802B4978, 0x48000018); // skip garbage
    kmCall(0x802B4990, createBenefitItemLifeUp);

    // TreasureBoxCracked creates Life Mushroom
    kmCall(0x8032561C, createBenefitItemLifeUp);
    kmWrite32(0x80325620, 0x7C7F1B78); // r31 = BenefitItemLifeUp*
    kmWrite32(0x80325624, 0x4800002C); // skip garbage
}
