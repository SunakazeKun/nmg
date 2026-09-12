#pragma once

#include "syati.h"

namespace neo {
    enum GameProgress {
        GameProgress_NormalStars,
        GameProgress_GreenStars,
        GameProgress_AllStars,
        GameProgress_AllComplete
    };

    GameProgress getGameProgressType(const UserFile *pUserFile);
    void calcUserFileCollectibles(const UserFile *pUserFile, s32 *pMedals, s32 *pStamps);

    bool isUnlockComets();
    bool isUnlockGreenStars();
};
