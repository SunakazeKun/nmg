#pragma once

#include "revolution.h"

class UserFile;

namespace neo {
    bool isDaredevilMode();
    bool isDaredevilModeOrComet();
    bool isNotDaredevilModeAndComet();
    bool isUserFileDaredevilMode(const UserFile *pUserFile);

    bool tryOnDaredevilMode();
    bool tryOffDaredevilMode();
};
