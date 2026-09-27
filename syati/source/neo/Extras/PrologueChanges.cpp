#include "Game/System/AllData/GameDataFunction.h"
#include "kamek/hooks.h"

namespace {
    // =================================================================================================================
    // Disables StoryBook layout for PeachCastleGalaxy
    // =================================================================================================================

    kmWrite32(0x80056BF0, PPC_B(0x8C));

    
    // =================================================================================================================
    // Don't disable IslandFleetGalaxy's intro camera
    // =================================================================================================================

    kmWrite32(0x80142C34, PPC_LI(3, 0));
    kmWrite32(0x80451804, PPC_LI(3, 0));


    // =================================================================================================================
    // Force activation of some game event flags
    // =================================================================================================================

    void forceOnGameEventFlags() {
        GameDataFunction::onGameEventFlag("オープニング実行"); // Prologue played
        GameDataFunction::onGameEventFlag("ワールド1初プレイ"); // World 1 first play
    }

    // GameSequenceInGame::exeOpening
#if defined(SB4_JEP)
    kmCall(0x804B3FDC, forceOnGameEventFlags);
#elif defined(SB4_WK)
    kmCall(0x804B404C, forceOnGameEventFlags);
#endif
}
