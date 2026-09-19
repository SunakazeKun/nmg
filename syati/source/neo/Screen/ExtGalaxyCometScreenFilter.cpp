#include "Game/Screen/LayoutActor.h"
#include "Game/Util/StringUtil.h"
#include "Game/Util/LayoutUtil.h"
#include "kamek/hooks.h"

namespace {
    void setCometTypeExt(LayoutActor *layout, f32 frame, u32 frameCtrlIdx) {
        register const char* cometName;
        __asm {
            lwz cometName, 0xC(r29)
        }

        if (cometName != nullptr) {
            if (MR::isEqualString(cometName, "Red")) {
                frame = 2.0f;
            }
            else if (MR::isEqualString(cometName, "Horror")) {
                frame = 3.0f;
            }
            else if (MR::isEqualString(cometName, "Dark")) {
                frame = 4.0f;
            }
            else if (MR::isEqualString(cometName, "Quick")) {
                frame = 5.0f;
            }
            else if (MR::isEqualString(cometName, "Purple")) {
                frame = 6.0f;
            }
        }

        MR::setAnimFrameAndStop(layout, frame, frameCtrlIdx);
    }

    // GalaxyCometScreenFilter::setCometType
    kmCall(0x8046F554, setCometTypeExt);
}
