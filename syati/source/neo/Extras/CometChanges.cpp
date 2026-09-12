#include "syati.h"

namespace {
	void setCometScreenFilterColor(LayoutActor *layout, bool isGreenStar) {
		register const char* cometName;
		__asm {
			lwz cometName, 0xC(r29);
		}

		f32 frame = isGreenStar ? 1.0f : 0.0f;

		if (cometName != NULL) {
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

		MR::startAnim(layout, "Color", 0);
		MR::setAnimFrameAndStop(layout, frame, 0);
	}

	kmCall(0x8037295C, setCometScreenFilterColor);
}
