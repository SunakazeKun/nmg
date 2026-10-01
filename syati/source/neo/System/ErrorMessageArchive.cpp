#include "neo/System/ErrorMessageArchive.h"
#include "Game/System/FileRipper.h"
#include "Game/Util/FileUtil.h"
#include "kamek/hooks.h"

namespace neo {
    const char* getErrorMessageArchivePath() {
#if defined(SB4P)
        return "/SystemData/ErrorMessageArchiveEu.arc";
#elif defined(SB4E)
        return "/SystemData/ErrorMessageArchiveUs.arc";
#elif defined(SB4J)
        return "/SystemData/ErrorMessageArchiveJp.arc";
#elif defined(SB4C)
        return "/SystemData/ErrorMessageArchiveCn.arc";
#elif defined(SB4W)
        return "/SystemData/ErrorMessageArchiveAs.arc";
#elif defined(SB4K)
        return "/SystemData/ErrorMessageArchiveKr.arc";
#endif
    }

    // -----------------------------------------------------------------------------------------------------------------

    namespace {
        void loadAndMountErrorMessageArchive(void *, JKRHeap *pHeap, const char *) {
            const char* pFilePath = neo::getErrorMessageArchivePath();
            void* pData = FileRipper::loadToMainRAM(pFilePath, nullptr, false, pHeap, FileRipper::UNK_0);
            MR::createAndAddArchive(pData, pHeap, pFilePath);
        }

#if defined(SB4_JEP)
        kmCall(0x804B8E20, loadAndMountErrorMessageArchive);
        kmCall(0x804B934C, getErrorMessageArchivePath);
        kmCall(0x804B7ECC, getErrorMessageArchivePath);
#elif defined(SB4_WK)
        kmCall(0x804B8E90, loadAndMountErrorMessageArchive);
        kmCall(0x804B93BC, getErrorMessageArchivePath);
        kmCall(0x804B7F3C, getErrorMessageArchivePath);
#endif
    }
}
