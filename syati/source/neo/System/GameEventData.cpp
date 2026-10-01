#include "neo/System/GameEventData.h"
#include "kamek/hooks.h"

namespace {
#if defined(SB4_JEP)
#define GAME_EVENT_FLAG_TABLE_START  (0x8064DF48)
#define GAME_EVENT_VALUE_TABLE_START (0x8064E078)
#elif defined(SB4_WK)
#define GAME_EVENT_FLAG_TABLE_START  (0x8064E4F8)
#define GAME_EVENT_VALUE_TABLE_START (0x8064E628)
#endif

#define REGISTER_FLAG(index, name) kmWritePointer(GAME_EVENT_FLAG_TABLE_START + index * 0x04, name);
#define REGISTER_VALUE(index, name, defval) kmWritePointer(GAME_EVENT_VALUE_TABLE_START + index * 0x08 + 0x00, name);\
kmWrite16(GAME_EVENT_VALUE_TABLE_START + index * 0x08 + 0x04, defval);

    // -----------------------------------------------------------------------------------------------------------------

    REGISTER_FLAG(36, "フライングマリオ初変身");
    REGISTER_FLAG(37, GAME_EVENT_FLAG_DAREDEVIL_MODE_ON);
    REGISTER_FLAG(38, GAME_EVENT_FLAG_EXPLAIN_ILLUST_ITEM_AT_FIRST);
    REGISTER_FLAG(39, "Dummy39");
    REGISTER_FLAG(40, "Dummy40");
    REGISTER_FLAG(41, "Dummy41");
    REGISTER_FLAG(42, "Dummy42");

    // -----------------------------------------------------------------------------------------------------------------

    REGISTER_VALUE(4, GAME_EVENT_VALUE_HIGHSCORE_SWEET_TREAT_LO, 0);
    REGISTER_VALUE(5, GAME_EVENT_VALUE_HIGHSCORE_SWEET_TREAT_HI, 0);
    REGISTER_VALUE(6, "Dummy6", 0);
    REGISTER_VALUE(7, "Dummy7", 0);
    REGISTER_VALUE(8, "Dummy8", 0);
    REGISTER_VALUE(9, "Dummy9", 0);
    REGISTER_VALUE(10, "Dummy10", 0);
    REGISTER_VALUE(11, "Dummy11", 0);
    REGISTER_VALUE(12, "Dummy12", 0);
    REGISTER_VALUE(13, "Dummy13", 0);
    REGISTER_VALUE(14, "Dummy14", 0);
    REGISTER_VALUE(15, "Dummy15", 0);
}
