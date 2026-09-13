#include "neo/System/DaredevilModeAccess.h"
#include "Game/System/AllData/GameDataFunction.h"
#include "Game/System/GameData/UserFile.h"
#include "Game/Util/EventUtil.h"
#include "neo/core.h"

namespace neo {
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
}
