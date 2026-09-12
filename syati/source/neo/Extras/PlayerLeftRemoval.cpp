#include "syati.h"
#include "neo/core.h"

/*********************************************************************************************************************/
/* Remove 1-Up system                                                                                                */
/*********************************************************************************************************************/

namespace {
	void hidePaneRecursive(LayoutActor *pLayout, const char *pPaneName) {
		MR::hidePaneRecursive(pLayout, pPaneName);
	}

	// Disable 1-Up counter on ScenarioSelectLayout
#if defined(PAL) || defined(USA) || defined(JPN)
	kmCall(0x8048EB18, hidePaneRecursive);
	kmCall(0x8048EB2C, hidePaneRecursive);
#elif defined(TWN) || defined(KOR)
	kmCall(0x8048EB28, hidePaneRecursive);
	kmCall(0x8048EB3C, hidePaneRecursive);
#endif

	// Disable 1-Up counter on PauseMenu
#if defined(PAL) || defined(USA) || defined(JPN)
	kmCall(0x80487154, hidePaneRecursive);
#elif defined(TWN) || defined(KOR)
	kmCall(0x80487164, hidePaneRecursive);
#endif

	// Disable PlayerLeft layout
	// kmWrite32(0x804657F0, 0x48000020);
	// kmWrite32(0x804659D8, 0x48000014);
	// kmWrite32(0x80465B2C, NOP);
	// kmWrite32(0x80465BAC, NOP);
	// kmWrite32(0x80465C98, 0x48000050);
	// kmWrite32(0x80465EEC, 0x48000014);
	// kmWrite32(0x80465F6C, 0x48000014);
	// kmWrite32(0x80466014, 0x48000014);
#if defined(PAL) || defined(USA) || defined(JPN)
	kmWrite32(0x804A8E20, 0x48000020);
	kmWrite32(0x804A8E78, NOP);
	kmWrite32(0x804A8F04, 0x48000014);
	kmWrite32(0x804A8F68, NOP);
	kmWrite32(0x804A8FDC, 0x48000014);
	kmWrite32(0x804A9060, 0x48000024);
	kmWrite32(0x804A9214, 0x4800003C);
#elif defined(TWN) || defined(KOR)
	kmWrite32(0x804A8E90, 0x48000020);
	kmWrite32(0x804A8EE8, NOP);
	kmWrite32(0x804A8F74, 0x48000014);
	kmWrite32(0x804A8FD8, NOP);
	kmWrite32(0x804A904C, 0x48000014);
	kmWrite32(0x804A90D0, 0x48000024);
	kmWrite32(0x804A9284, 0x4800003C);
#endif

	// Disable PlayerLeftMiss layout
#if defined(PAL) || defined(USA) || defined(JPN)
	kmWrite32(0x804D9200, NOP);
	kmWrite32(0x804B325C, NOP);
	kmWrite32(0x804B43A4, 0x48000058);
#elif defined(TWN) || defined(KOR)
	kmWrite32(0x804D9290, NOP);
	kmWrite32(0x804B32CC, NOP);
	kmWrite32(0x804B4414, 0x48000058);
#endif

	// Acquirement stubs
	kmWrite32(0x80023890, BLR); // MR::addPlayerLeft stub
	kmWrite32(0x800238F8, NOP); // don't decrement lives
	kmWrite32(0x80023A30, 0x48000024); // MR::addStarPiece doesn't give 1-Up
	kmWrite32(0x80059970, BLR); // prevent SE_SY_1UP/5UP sounds
#if defined(PAL) || defined(USA) || defined(JPN)
	kmWrite32(0x804DE030, BLR); // GameDataPlayerStatus::addPlayerLeft stub
#elif defined(TWN) || defined(KOR)
	kmWrite32(0x804DE0C0, BLR); // GameDataPlayerStatus::addPlayerLeft stub
#endif

	// Skip Bank Toad sequence
	kmWrite32(0x8034EC20, 0x38600000); // Disable check for 1-Up gift
	kmWrite32(0x8034EC24, 0x480000AC);
	kmWrite32(0x8034EE54, 0x48000038); // Prevent 1-Ups from being gifted
}
