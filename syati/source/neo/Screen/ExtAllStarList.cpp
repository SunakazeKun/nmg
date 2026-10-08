#include "revolution/types.h"
#include "kamek/hooks.h"

namespace {
    const char* cGalaxyPanes[] = {
        "Galaxy1",
        "Galaxy2",
        "Galaxy3",
        "Galaxy4",
        "Galaxy5",
        "Galaxy6",
        "Galaxy7",
        "Galaxy8",
    };

    __asm void loadGalaxyPanesArray() {
        nofralloc;
        lis r26, cGalaxyPanes@h;
        ori r26, r26, cGalaxyPanes@l;
        blr;
    }

    kmCall(0x8045E3C0, loadGalaxyPanesArray);
}
