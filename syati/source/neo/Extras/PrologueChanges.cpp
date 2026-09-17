#include "Game/System/AllData/GameDataFunction.h"
#include "kamek/hooks.h"
#include "neo/core.h"

namespace {
    // =================================================================================================================
    // Progress-based scenario selection
    // =================================================================================================================

    // No Power Stars collected -> Starship Mario
#if defined(SB4P) || defined(SB4E) || defined(SB4J)
    kmWrite32(0x804D6314, PPC_COND_BRANCH(PPC_COND_NE, 0x58));
#elif defined(SB4W) || defined(SB4K)
    kmWrite32(0x804D63A4, PPC_COND_BRANCH(PPC_COND_NE, 0x58));
#endif

    // VsKoopaLv3Galaxy cleared -> Starship Mario
#if defined(SB4P) || defined(SB4E) || defined(SB4J)
    kmWrite32(0x804D66A8, PPC_COND_BRANCH(PPC_COND_EQ, 0x7C));
#elif defined(SB4W) || defined(SB4K)
    kmWrite32(0x804D6738, PPC_COND_BRANCH(PPC_COND_EQ, 0x7C));
#endif


    // =================================================================================================================
    // Disables StoryBook layout for PeachCastleGalaxy
    // =================================================================================================================

    kmWrite32(0x80056BF0, PPC_B(0x8C));


    // =================================================================================================================
    // Force activation of some game event flags
    // =================================================================================================================

    const char* cForceOnGameEventFlags[] = {
        "オープニング実行", // Prologue played
        "ワールド1初プレイ", // World 1 first play
    };

    const s32 cForceOnGameEventFlagsCount = sizeof(cForceOnGameEventFlags) / sizeof(const char *);

    void forceOnGameEventFlags() {
        for (int i = 0; i < cForceOnGameEventFlagsCount; i++) {
            GameDataFunction::onGameEventFlag(cForceOnGameEventFlags[i]);
        }
    }

    // GameSequenceInGame::exeOpening
#if defined(SB4P) || defined(SB4E) || defined(SB4J)
    kmCall(0x804B3FDC, forceOnGameEventFlags);
#elif defined(SB4W) || defined(SB4K)
    kmCall(0x804B404C, forceOnGameEventFlags);
#endif
}
