#include "syati.h"
#include "neo/core.h"

namespace {
	/*****************************************************************************************************************/
	/* Power Stars that don't play star chance music                                                                 */
	/*****************************************************************************************************************/
	struct StarChanceExceptInfo {
		const char* mStageName;
		int mPowerStarId;
	};

	const StarChanceExceptInfo cStarChanceExceptInfos[] = {
		{ STAGE_GLOOMY_GALLEON_GALAXY, 0 },
		{ STAGE_TURNLOG_GALAXY, 3 },
		{ STAGE_COMET_OBSERVATORY, 0 }
	};

	const s32 cStarChanceExceptInfosCount = sizeof(cStarChanceExceptInfos) / sizeof(StarChanceExceptInfo);

	bool checkStarChanceStage() {
		register s32 powerStarId;
		__asm {
			mr powerStarId, r30
		}

		for (s32 i = 0; i < cStarChanceExceptInfosCount; i++) {
			StarChanceExceptInfo exceptInfo = cStarChanceExceptInfos[i];

			if (MR::isEqualStageName(exceptInfo.mStageName)) {
				if (exceptInfo.mPowerStarId == powerStarId || exceptInfo.mPowerStarId <= 0) {
					return false;
				}
			}
		}

		return true;
	}

	kmCall(0x80056E90, checkStarChanceStage);


	/*****************************************************************************************************************/
	/* Prevent BGM_BOSSRUSH02 after defeating SMG1 bosses                                                            */
	/*****************************************************************************************************************/
	kmWrite32(0x8001BB20, BLR);


	/*****************************************************************************************************************/
	/* Starship Mario music sequence                                                                                 */
	/*****************************************************************************************************************/
	bool newStartMarioFaceShipBGM(int bgmType) {
		s32 numStars = MR::getPowerStarNum();

		if (numStars >= 30) {
			bgmType = 3;
		}
		else if (numStars >= 15) {
			bgmType = 2;
		}
		else {
			bgmType = 1;
		}

		switch (bgmType) {
		case 1:
			return MR::startStageBGMIfNotPlaying("MBGM_SMG2_MARIO_SHIP01", false);
		case 2:
			return MR::startStageBGMIfNotPlaying("MBGM_SMG2_MARIO_SHIP02", false);
		case 3:
			return MR::startStageBGMIfNotPlaying("MBGM_SMG2_MARIO_SHIP03", false);
		}

		return false;
	}

#if defined(PAL) || defined(USA) || defined(JPN)
	kmCall(0x804B49B0, newStartMarioFaceShipBGM);
	kmCall(0x804B5378, newStartMarioFaceShipBGM);
	kmCall(0x804B5BAC, newStartMarioFaceShipBGM);
#elif defined(TWN) || defined(KOR)
	kmCall(0x804B4A20, newStartMarioFaceShipBGM);
	kmCall(0x804B53E8, newStartMarioFaceShipBGM);
	kmCall(0x804B5C1C, newStartMarioFaceShipBGM);
#endif
}
