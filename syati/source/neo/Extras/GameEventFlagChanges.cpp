#include "kamek/hooks.h"
#include "neo/core.h"

namespace {
    // =================================================================================================================
    // Stamp collection flag replaces IsOpenScenarioGoroRockGalaxy3
    // =================================================================================================================

#if defined(SB4_JEP)
    kmWritePointer(0x8064DFD8, GAME_EVENT_FLAG_EXPLAIN_ILLUST_ITEM_AT_FIRST);
#elif defined(SB4_WK)
    kmWritePointer(0x8064E588, GAME_EVENT_FLAG_EXPLAIN_ILLUST_ITEM_AT_FIRST);
#endif


    // =================================================================================================================
    // Daredevil Mode flag replaces IsOpenScenarioJungleGliderGalaxy2
    // =================================================================================================================

#if defined(SB4_JEP)
    kmWritePointer(0x8064DFDC, GAME_EVENT_FLAG_DAREDEVIL_MODE_ON);
#elif defined(SB4_WK)
    kmWritePointer(0x8064E58C, GAME_EVENT_FLAG_DAREDEVIL_MODE_ON);
#endif


    // =================================================================================================================
    // Flying Star collection flag replaces IsOpenScenarioMokumokuValleyGalaxy2
    // =================================================================================================================

#if defined(SB4P)
    kmWrite32(0x8064DFF0, 0x80662EBC);
#elif defined(SB4E)
    kmWrite32(0x8064DFF0, 0x8065D7BC);
#elif defined(SB4J)
    kmWrite32(0x8064DFF0, 0x8065CF9C);
#elif defined(SB4W)
    kmWrite32(0x8064E5A0, 0x8065DA5C);
#elif defined(SB4K)
    kmWrite32(0x8064E5A0, 0x8065C5FC);
#endif
};
