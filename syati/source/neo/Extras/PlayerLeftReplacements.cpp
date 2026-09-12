#include "neo/System/DaredevilModeAccess.h"

/*****************************************************************************************************************/
/* 1-Up replacements                                                                                             */
/*****************************************************************************************************************/

namespace {
	void changePlayerItemLifeUp() {
		MR::changePlayerItemLifeUp();
	}

	BenefitItemLifeUp* createBenefitItemLifeUp() {
		return new BenefitItemLifeUp("スーパーキノコ");
	}

	// MarioLauncherTarget grants a Life-Up instead
	kmCall(0x802CFC4C, changePlayerItemLifeUp);


	// ItemDice creates Life Mushroom
	kmWrite32(0x802B4978, 0x48000018); // skip garbage
	kmCall(0x802B4990, createBenefitItemLifeUp);


	// TreasureBoxCracked creates Life Mushroom
	kmCall(0x8032561C, createBenefitItemLifeUp);
	kmWrite32(0x80325620, 0x7C7F1B78); // r31 = BenefitItemLifeUp*
	kmWrite32(0x80325624, 0x4800002C); // skip garbage


	// YoshiFruit grants a Life-Up instead
	kmCall(0x8041793C, changePlayerItemLifeUp);
}
