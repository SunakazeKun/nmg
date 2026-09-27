#include "Game/Util/LayoutUtil.h"
#include "kamek/hooks.h"

namespace {
    // =================================================================================================================
    // Don't display World names
    // =================================================================================================================

    void setTextBoxMessageAndHidePaneRecursive(LayoutActor *pLayout, const char *pPaneName, const wchar_t *pMessage) {
        MR::setTextBoxMessageRecursive(pLayout, pPaneName, pMessage);
        MR::hidePaneRecursive(pLayout, pPaneName);
    }

    kmCall(0x8045E318, setTextBoxMessageAndHidePaneRecursive);
#if defined(SB4_JEP)
    kmWrite32(0x80486FE0, PPC_NOP);
    kmWrite32(0x804A9C00, PPC_BLR);
    kmBranch(0x804A9CAC, setTextBoxMessageAndHidePaneRecursive);
    kmCall(0x804EEEF0, setTextBoxMessageAndHidePaneRecursive);
#elif defined(SB4_WK)
    kmWrite32(0x80486FF0, PPC_NOP);
    kmWrite32(0x804A9C70, PPC_BLR);
    kmBranch(0x804A9D1C, setTextBoxMessageAndHidePaneRecursive);
    kmCall(0x804EEF80, setTextBoxMessageAndHidePaneRecursive);
#endif


    // =================================================================================================================
    // Move Power Star count
    // =================================================================================================================

#if defined(SB4P)
    kmWrite32(0x807E6E60, 0x43AC0000);
#elif defined(SB4E)
    kmWrite32(0x807E1760, 0x43AC0000);
#elif defined(SB4J)
    kmWrite32(0x807E0F20, 0x43AC0000);
#elif defined(SB4W)
    kmWrite32(0x8073AD80, 0x43AC0000);
#elif defined(SB4K)
    kmWrite32(0x80739920, 0x43AC0000);
#endif


    // =================================================================================================================
    // Skip Grand World Map portion of Green Star appearance
    // =================================================================================================================

#if defined(SB4_JEP)
    kmWrite32(0x804F9D58, PPC_B(0xFC));
#elif defined(SB4_WK)
    kmWrite32(0x804F9DE8, PPC_B(0xFC));
#endif
}
