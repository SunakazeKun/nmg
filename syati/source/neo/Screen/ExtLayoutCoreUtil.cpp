#include "revolution/types.h"
#include "kamek/hooks.h"

namespace {
#if defined(SB4_JEP)
    __asm void adjustTextBoxWidthTolerance() {
        nofralloc
        stwu r1, -0x10(r1)
        mflr r0
        stw r0, 0x14(r1)

        // r2 = 0.05f
        lis r0, 0x3D4C
        ori r0, r0, 0xCCCD
        stw r0, 0x8(r1)
        lfs f2, 0x8(r1)
        
        // Adjustment
        fsubs f0, f0, f2
        fcmpo cr0, f0, f1

        lwz r0, 0x14(r1)
        mtlr r0
        addi r1, r1, 0x10
        blr
    }

    kmCall(0x8047B0C0, adjustTextBoxWidthTolerance);
#endif
}
