#include "loader/KamekAccess.h"

static KamekModule** sModulePtr = (KamekModule**)0x80003FFC;

KamekModule* getKamekModule() {
    return *sModulePtr;
}

void setKamekModule(KamekModule *pModule) {
    *sModulePtr = pModule;
}
