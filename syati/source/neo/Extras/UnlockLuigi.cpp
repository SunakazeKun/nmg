#include "syati.h"
#include "neo/core.h"

/*********************************************************************************************************************/
/* Unlock Luigi from beginning                                                                                       */
/*********************************************************************************************************************/

namespace {
	// ChangeRoom ignores flag checks
	kmWrite32(0x8028141C, LI_R3_1);
	kmWrite32(0x802813B4, LI_R3_1);


	// Prevent player type overwrite
#if defined(PAL) || defined(USA) || defined(JPN)
	kmWrite32(0x804B4EBC, LI_R3_1);
	kmWrite32(0x804B559C, LI_R3_1);
	kmWrite32(0x804D5DFC, LI_R3_1);
	kmWrite32(0x804D6274, LI_R3_1);
	kmWrite32(0x804D62EC, LI_R3_1);
	kmWrite32(0x804D6488, LI_R3_1);
	kmWrite32(0x804D66AC, LI_R3_1);
	kmWrite32(0x804D66FC, LI_R3_1);
	kmWrite32(0x804D6730, LI_R3_1);
	kmWrite32(0x804D7F4C, LI_R3_1);
#elif defined(TWN) || defined(KOR)
	kmWrite32(0x804B4F2C, LI_R3_1);
	kmWrite32(0x804B560C, LI_R3_1);
	kmWrite32(0x804D5E8C, LI_R3_1);
	kmWrite32(0x804D6304, LI_R3_1);
	kmWrite32(0x804D637C, LI_R3_1);
	kmWrite32(0x804D6518, LI_R3_1);
	kmWrite32(0x804D673C, LI_R3_1);
	kmWrite32(0x804D678C, LI_R3_1);
	kmWrite32(0x804D67C0, LI_R3_1);
	kmWrite32(0x804D7FDC, LI_R3_1);
#endif
}
