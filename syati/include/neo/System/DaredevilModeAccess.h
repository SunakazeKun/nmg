#pragma once

#include "syati.h"

namespace neo {
    bool isDaredevilMode();
    bool isDaredevilModeOrComet();
    bool isNotDaredevilModeAndComet();
    bool isUserFileDaredevilMode(const UserFile *pUserFile);

    bool tryOnDaredevilMode();
    bool tryOffDaredevilMode();

    void changeMarioItemStatusIfNotDaredevil(s32 status);
};
