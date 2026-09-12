#include "syati.h"
#include "neo/core.h"

namespace {
	// Stamp collection flag replaces IsOpenScenarioGoroRockGalaxy3
#if defined(PAL) || defined(USA) || defined(JPN)
	kmWritePointer(0x8064DFD8, "ExplainIllustItemAtFirst");
#elif defined(TWN) || defined(KOR)
	kmWritePointer(0x8064E578, "ExplainIllustItemAtFirst");
#endif

	// Daredevil Mode flag replaces IsOpenScenarioJungleGliderGalaxy2
#if defined(PAL) || defined(USA) || defined(JPN)
	kmWritePointer(0x8064DFDC, "DaredevilModeOn");
#elif defined(TWN) || defined(KOR)
	kmWritePointer(0x8064E57C, "DaredevilModeOn");
#endif

	// Flying Star collection flag replaces IsOpenScenarioMokumokuValleyGalaxy2
#if defined(PAL)
	kmWrite32(0x8064DFF0, 0x80662EBC);
#elif defined(USA)
	kmWrite32(0x8064DFF0, 0x8065D7BC);
#elif defined(JPN)
	kmWrite32(0x8064DFF0, 0x8065CF9C);
#elif defined(TWN)
	kmWrite32(0x8064E590, 0x8065DA5C);
#elif defined(KOR)
	kmWrite32(0x8064E590, 0x8065C5FC);
#endif
};
