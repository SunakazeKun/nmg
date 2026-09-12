#include "neo/System/DaredevilModeAccess.h"
#include "neo/core.h"

namespace neo {
	/*********************************************************************************************************************/
	/* Checker functions for Daredevil mode and comet                                                                    */
	/*********************************************************************************************************************/
	bool isDaredevilMode() {
		return GameDataFunction::isOnGameEventFlag(GAME_EVENT_FLAG_DAREDEVIL_MODE_ON);
	}

	bool isDaredevilModeOrComet() {
		return isDaredevilMode() || MR::isGalaxyDarkCometAppearInCurrentStage();
	}

	bool isNotDaredevilModeAndComet() {
		return !isDaredevilMode() && MR::isGalaxyDarkCometAppearInCurrentStage();
	}

	bool isUserFileDaredevilMode(const UserFile *pUserFile) {
		return pUserFile->mGameDataHolder->isOnGameEventFlag(GAME_EVENT_FLAG_DAREDEVIL_MODE_ON);
	}


	/*********************************************************************************************************************/
	/* Mode activation & deactivation                                                                                    */
	/*********************************************************************************************************************/
	bool tryOnDaredevilMode() {
		if (isDaredevilMode()) {
			return false;
		}
		else {
			GameDataFunction::onGameEventFlag(GAME_EVENT_FLAG_DAREDEVIL_MODE_ON);
			return true;
		}
	}

	bool tryOffDaredevilMode() {
		if (!isDaredevilMode()) {
			return false;
		}
		else {
			GameDataFunction::offGameEventFlag(GAME_EVENT_FLAG_DAREDEVIL_MODE_ON);
			return true;
		}
	}


	/*********************************************************************************************************************/
	/* Helper functions                                                                                                  */
	/*********************************************************************************************************************/
	void changeMarioItemStatusIfNotDaredevil(s32 status) {
		if (!neo::isDaredevilModeOrComet()) {
			MarioAccess::changeItemStatus(status);
		}

		MR::incPlayerOxygen(8);
	}
}
