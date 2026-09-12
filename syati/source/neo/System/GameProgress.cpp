#include "neo/System/GameProgress.h"
#include "neo/System/IllustItemAccess.h"
#include "neo/core.h"

namespace neo {
	GameProgress getGameProgressType(const UserFile *pUserFile) {
		s32 normalStars = pUserFile->calcNormalPowerStarNum();
		s32 totalStars = pUserFile->getPowerStarNum();
		s32 medals, stamps;

		calcUserFileCollectibles(pUserFile, &medals, &stamps);

		if (totalStars >= GAME_FLOW_TOTAL_POWER_STARS) {
			if (medals >= GAME_FLOW_TOTAL_COMET_MEDALS && stamps >= GAME_FLOW_TOTAL_STAMPS) {
				return GameProgress_AllComplete;
			}
			else {
				return GameProgress_AllStars;
			}
		}

		if (normalStars >= GAME_FLOW_POWER_STARS_FOR_GREEN_STARS) {
			return GameProgress_GreenStars;
		}

		return GameProgress_NormalStars;
	}

	void calcUserFileCollectibles(const UserFile *pUserFile, s32 *pMedals, s32 *pStamps) {
		GameDataAllGalaxyStorage* allGalaxyStorage = pUserFile->mGameDataHolder->mAllGalaxyStorage;
		s32 medals = 0;
		s32 stamps = 0;

		for (int i = 0; i < allGalaxyStorage->mNumGalaxies; i++) {
			u8 flags = allGalaxyStorage->mGalaxyStorages[i]->mFlag;

			if (flags & SOME_GALAXY_STORAGE_FLAG_TICO_COIN) {
				medals++;
			}

			if (flags & SOME_GALAXY_STORAGE_FLAG_ILLUST_ITEM) {
				stamps++;
			}
		}

		*pMedals = medals;
		*pStamps = stamps;
	}

	/*
	* Unlock Prankster Comets after collecting at least 5 Stars
	*
	* This changes the complicated flag sequence that unlocks Prankster Comets. The routine has been simplified and does
	* not require specific missions to be finished anymore. The player needs to collect at least 5 Stars to unlock the
	* comets in general.
	*/
	bool isUnlockComets() {
		if (GameDataFunction::isOnGameEventFlag(GAME_EVENT_FLAG_COMET_EXPLANATION)) {
			return true;
		}
		else {
			if (MR::getPowerStarNum() >= GAME_FLOW_POWER_STARS_FOR_COMETS) {
				GameDataFunction::onGameEventFlag(GAME_EVENT_FLAG_COMET_EXPLANATION);
				return true;
			}
			else {
				return false;
			}
		}
	}

#if defined(PAL) || defined(USA) || defined(JPN)
	kmBranch(0x804D2E90, isUnlockComets); // Replace flag check by Power Star requirement
	kmWrite32(0x804D8B80, LI_R3_1); // Disable hardcoded check for Mimic comets
	kmWrite32(0x804D8C44, LI_R3_1); // Disable hardcoded check for Purple comets
#elif defined(TWN) || defined(KOR)
	kmBranch(0x804D2F20, isUnlockComets); // Replace flag check by Power Star requirement
	kmWrite32(0x804D8C10, LI_R3_1); // Disable hardcoded check for Mimic comets
	kmWrite32(0x804D8CD4, LI_R3_1); // Disable hardcoded check for Purple comets
#endif

	/*
	* Unlock Green Stars after collecting at least 21 Stars
	*
	* Originally, a special event flag declares whether Green Stars are available or not. This flag check has been replaced
	* by a routine that checks if at least 21 Power Stars and at least 8 Comet Medals have been collected.
	*/
	bool isUnlockGreenStars() {
		return GameDataFunction::calcCurrentPowerStarNum() >= GAME_FLOW_POWER_STARS_FOR_GREEN_STARS
			&& GameDataFunction::calcTicoCoinNum() >= 8;
	}

#if defined(PAL) || defined(USA) || defined(JPN)
	kmBranch(0x804D2D90, isUnlockGreenStars);
	kmCall(0x804D82A4, isUnlockGreenStars);
#elif defined(TWN) || defined(KOR)
	kmBranch(0x804D2E20, isUnlockGreenStars);
	kmCall(0x804D8334, isUnlockGreenStars);
#endif
}
