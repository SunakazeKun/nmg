#pragma once

#include "revolution/types.h"

class UserFile;

namespace neo {
    enum GameProgress {
        GameProgress_NormalStars,
        GameProgress_GreenStars,
        GameProgress_AllStars,
        GameProgress_AllComplete
    };

    GameProgress getGameProgressType(const UserFile *pUserFile);
    void calcUserFileCollectibles(const UserFile *pUserFile, s32 *pMedals, s32 *pStamps);
    bool checkUserFileAllowRosettaMiiIcon(const UserFile *pUserFile);
}
