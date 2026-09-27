#include "revolution/types.h"
#include "kamek/hooks.h"

namespace {
    // ==================================================================================================================
    // ChangeRoom is always usable
    // ==================================================================================================================

    kmWrite32(0x8028141C, PPC_LI(3, 1));
    kmWrite32(0x802813B4, PPC_LI(3, 1));


    // ==================================================================================================================
    // Always retain player character
    // ==================================================================================================================

#if defined(SB4_JEP)
    kmWrite32(0x804B4EBC, PPC_LI(3, 1));
    kmWrite32(0x804B559C, PPC_LI(3, 1));
    kmWrite32(0x804D5DFC, PPC_LI(3, 1));
    kmWrite32(0x804D6274, PPC_LI(3, 1));
    kmWrite32(0x804D62EC, PPC_LI(3, 1));
    kmWrite32(0x804D6488, PPC_LI(3, 1));
    kmWrite32(0x804D66AC, PPC_LI(3, 1));
    kmWrite32(0x804D66FC, PPC_LI(3, 1));
    kmWrite32(0x804D6730, PPC_LI(3, 1));
    kmWrite32(0x804D7F4C, PPC_LI(3, 1));
#elif defined(SB4_WK)
    kmWrite32(0x804B4F2C, PPC_LI(3, 1));
    kmWrite32(0x804B560C, PPC_LI(3, 1));
    kmWrite32(0x804D5E8C, PPC_LI(3, 1));
    kmWrite32(0x804D6304, PPC_LI(3, 1));
    kmWrite32(0x804D637C, PPC_LI(3, 1));
    kmWrite32(0x804D6518, PPC_LI(3, 1));
    kmWrite32(0x804D673C, PPC_LI(3, 1));
    kmWrite32(0x804D678C, PPC_LI(3, 1));
    kmWrite32(0x804D67C0, PPC_LI(3, 1));
    kmWrite32(0x804D7FDC, PPC_LI(3, 1));
#endif
}
