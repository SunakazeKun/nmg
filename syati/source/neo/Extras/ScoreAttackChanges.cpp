#include "syati.h"
#include "neo/core.h"

/*********************************************************************************************************************/
/* ScoreAttack changes                                                                                               */
/*********************************************************************************************************************/

namespace {
	// Ignores MokumokuValleyGalaxy in MR::isStageScoreAttack
	kmWrite32(0x80056954, LI_R3_0);

	// Ignores TaskEvent check for MokumokuValleyGalaxy
	kmWrite32(0x800253C8, LI_R3_0);

	// Ignores MokumokuValleyGalaxy's entry in TaskEvent table
	kmWrite32(0x8035B844, 0x38000006);

	// Changes minimum required highscore (500 -> 600) for MameMuimuiScorer
	//kmWrite32(0x801C07B8, 0x38000000 | 600);


	// Make Star Select show score for YosshiHomeGalaxy2
#if defined(PAL) || defined(USA) || defined(JPN)
	kmWritePointer(0x8064CB20, STAGE_SWEET_TREAT_GALAXY);
	kmWrite32(0x8064CB24, 3);
#elif defined(TWN) || defined(KOR)
	kmWritePointer(0x8064D0A0, STAGE_SWEET_TREAT_GALAXY);
	kmWrite32(0x8064D0A4, 3);
#endif


	// Score value for Sweet Treat replaces Fluffy Bluff's
#if defined(PAL) || defined(USA) || defined(JPN)
	kmWritePointer(0x8064E098, GAME_EVENT_VALUE_HIGHSCORE_SWEET_TREAT_LO);
	kmWritePointer(0x8064E0A0, GAME_EVENT_VALUE_HIGHSCORE_SWEET_TREAT_HI);
#elif defined(TWN) || defined(KOR)
	kmWritePointer(0x8064E638, GAME_EVENT_VALUE_HIGHSCORE_SWEET_TREAT_LO);
	kmWritePointer(0x8064E640, GAME_EVENT_VALUE_HIGHSCORE_SWEET_TREAT_HI);
#endif
}
