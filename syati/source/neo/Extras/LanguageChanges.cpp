#include "syati-light.h"

namespace {
#if defined(PAL) || defined(USA) || defined(JPN)
	kmWritePointer(0x8064CF2C, "UsEnglish");
	kmWritePointer(0x8064CF34, "UsEnglish");

	kmWritePointer(0x8064CF44, "EuEnglish");
	kmWritePointer(0x8064CF4C, "EuEnglish");
	kmWritePointer(0x8064CF64, "EuEnglish");

	kmWritePointer(0x8064CF6C, "UsEnglish");
	kmWritePointer(0x8064CF74, "UsEnglish");
	kmWritePointer(0x8064CF7C, "UsEnglish");
#elif defined(TWN) || defined(KOR)
	kmWritePointer(0x8064D4C4, "UsEnglish");
	kmWritePointer(0x8064D4CC, "UsEnglish");

	kmWritePointer(0x8064D4DC, "EuEnglish");
	kmWritePointer(0x8064D4E4, "EuEnglish");
	kmWritePointer(0x8064D4FC, "EuEnglish");

	kmWritePointer(0x8064D504, "UsEnglish");
	kmWritePointer(0x8064D50C, "UsEnglish");
	kmWritePointer(0x8064D514, "UsEnglish");
	kmWritePointer(0x8064D51C, "UsEnglish");
#endif
};
