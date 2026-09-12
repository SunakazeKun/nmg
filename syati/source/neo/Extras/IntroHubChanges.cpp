#include "syati.h"
#include "neo/core.h"

namespace {
	/*****************************************************************************************************************/
	/* Intro skips                                                                                                   */
	/*****************************************************************************************************************/
	kmWrite32(0x80056BF0, 0x4800008C); // Disables StoryBook layout for PeachCastleGalaxy

	// Removes Spin Attack restriction
#if defined(PAL) || defined(USA) || defined(JPN)
	//kmWrite32(0x804B3FD8, 0x38600001);
#elif defined(TWN) || defined(KOR)
	//kmWrite32(0x804B4048, 0x38600001);
#endif


	// The following lines fix selecting the proper scenarios when creating a new file, collecting the first star, ...
#if defined(PAL) || defined(USA) || defined(JPN)
	kmWrite32(0x804D6314, 0x40820058);
	kmWrite32(0x804D66A8, 0x4182007C);
#elif defined(TWN) || defined(KOR)
	kmWrite32(0x804D63A4, 0x40820058);
	kmWrite32(0x804D6738, 0x4182007C);
#endif


	// Enables the intro cam for Galactic Garden Galaxy
	kmWrite32(0x80142C34, LI_R3_0);
	kmWrite32(0x80451804, LI_R3_0);


	// Disables the Starship introduction after collecting IslandFleetGalaxy's first Power Star
#if defined(PAL) || defined(USA) || defined(JPN)
	kmWrite32(0x804D6D10, LI_R3_0);
	kmWrite32(0x804D6D14, BLR);
#elif defined(TWN) || defined(KOR)
	kmWrite32(0x804D6DA0, LI_R3_0);
	kmWrite32(0x804D6DA4, BLR);
#endif


	/*****************************************************************************************************************/
	/* Force activation of some flags                                                                                */
	/*****************************************************************************************************************/
	const char* cForceOnGameEventFlags[] = {
		"オープニング実行", // Opening run
		"ワールド1初プレイ", // World 1 first play
	};

	const s32 cForceOnGameEventFlagsCount = sizeof(cForceOnGameEventFlags) / sizeof(const char*);

	void forceOnGameEventFlags() {
		for (int i = 0; i < cForceOnGameEventFlagsCount; i++) {
			GameDataFunction::onGameEventFlag(cForceOnGameEventFlags[i]);
		}
	}

	// Flags will be set when entering PeachCastleGalaxy for the first time
#if defined(PAL) || defined(USA) || defined(JPN)
	kmCall(0x804B3FDC, forceOnGameEventFlags);
#elif defined(TWN) || defined(KOR)
	kmCall(0x804B404C, forceOnGameEventFlags);
#endif


	/*****************************************************************************************************************/
	/* Disable appearance of Postman Toad                                                                            */
	/*****************************************************************************************************************/
	kmWrite32(0x8034F620, LI_R3_0);
	kmWrite32(0x8034FA74, LI_R3_0);
}
