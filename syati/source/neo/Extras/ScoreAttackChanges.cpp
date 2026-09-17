#include "kamek/hooks.h"
#include "neo/core.h"

namespace {
    // Ignore MokumokuValleyGalaxy in MR::isStageScoreAttack
    kmWrite32(0x80056954, PPC_LI(3, 0));

    // Ignore TaskEvent check for MokumokuValleyGalaxy
    kmWrite32(0x800253C8, PPC_LI(3, 0));

    // Ignore MokumokuValleyGalaxy's entry in TaskEvent table
    kmWrite32(0x8035B844, PPC_LI(0, 6));


    // Make Star Select show score for Sweet Treat Galaxy 3
#if defined(SB4P) || defined(SB4E) || defined(SB4J)
    kmWritePointer(0x8064CB20, STAGE_SWEET_TREAT_GALAXY);
    kmWrite32(0x8064CB24, 3);
#elif defined(SB4W) || defined(SB4K)
    kmWritePointer(0x8064D0A0, STAGE_SWEET_TREAT_GALAXY);
    kmWrite32(0x8064D0A4, 3);
#endif


    // Score value for Sweet Treat replaces Fluffy Bluff's
#if defined(SB4P) || defined(SB4E) || defined(SB4J)
    kmWritePointer(0x8064E098, GAME_EVENT_VALUE_HIGHSCORE_SWEET_TREAT_LO);
    kmWritePointer(0x8064E0A0, GAME_EVENT_VALUE_HIGHSCORE_SWEET_TREAT_HI);
#elif defined(SB4W) || defined(SB4K)
    kmWritePointer(0x8064E648, GAME_EVENT_VALUE_HIGHSCORE_SWEET_TREAT_LO);
    kmWritePointer(0x8064E650, GAME_EVENT_VALUE_HIGHSCORE_SWEET_TREAT_HI);
#endif
}
