#include "Game/System/FileRipper.h"
#include "Game/System/Language.h"
#include "Game/Util/FileUtil.h"
#include "kamek/hooks.h"

// =====================================================================================================================
// ErrorMessageArchive holds important assets for displaying error messages when the game fails to read files from 
// the game's DVD. It is usually embedded within the game's main executable and only contains the assets for the
// regional release's supported languages. The following patch makes the game retrieve ErrorMessageArchive from
// an external file in the LocalizeData directory.
// =====================================================================================================================

namespace {
    const char* cErrorMsgArcPathFormat = "/LocalizeData/%s/SystemData/ErrorMessageArchive.arc";

    void loadAndMountErrorMessageArchive(void *, JKRHeap *pHeap, const char *pFilePath) {
        char fsFilePath[256];
        snprintf(fsFilePath, sizeof(fsFilePath), cErrorMsgArcPathFormat, MR::getCurrentLanguagePrefix());

        void* pData = FileRipper::loadToMainRAM(fsFilePath, nullptr, false, pHeap, FileRipper::UNK_0);
        MR::createAndAddArchive(pData, pHeap, pFilePath);
    }

#if defined(SB4_JEP)
    kmCall(0x804B8E20, loadAndMountErrorMessageArchive);
#elif defined(SB4_WK)
    kmCall(0x804B8E90, loadAndMountErrorMessageArchive);
#endif
}
