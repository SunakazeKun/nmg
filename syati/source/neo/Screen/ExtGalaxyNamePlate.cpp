#include "revolution/types.h"
#include "kamek/hooks.h"

// =====================================================================================================================
// For the Asian and Korean releases of the game, two GalaxyNamePlate panes were renamed to shorten them a bit.
// This change was necessary to allow for language-specific pane configurations. The following patches target the
// Japanese, American, and European releases of the game. Here, the panes TxtGalaxyName and TxtGalaxyNameU are
// renamed TxtGalaxyNa and TxtGalaxyNaU, respectively.
// =====================================================================================================================

namespace {
#if defined(SB4J)
    kmWrite32(0x806F32E0, 'yNa\0');
    kmWrite32(0x806F32E4, '\0\0\0\0');
    
    kmWrite32(0x806F32F0, 'yNaU');
    kmWrite32(0x806F32F4, '\0\0\0\0');
#elif defined(SB4E)
    kmWrite32(0x806F3B04, 'yNa\0');
    kmWrite32(0x806F3B08, '\0\0\0\0');
    
    kmWrite32(0x806F3B14, 'yNaU');
    kmWrite32(0x806F3B18, '\0\0\0\0');
#elif defined(SB4P)
    kmWrite32(0x806F9204, 'yNa\0');
    kmWrite32(0x806F9208, '\0\0\0\0');
    
    kmWrite32(0x806F9214, 'yNaU');
    kmWrite32(0x806F9218, '\0\0\0\0');
#endif
}
