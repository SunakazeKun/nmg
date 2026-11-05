#include "neo/System/ErrorMessageArchive.h"
#include "neo/System/Region.h"
#include "Game/System/FileRipper.h"
#include "Game/Util/FileUtil.h"
#include "kamek/hooks.h"
#include "revolution/os.h"

namespace neo {
    const char* getErrorMessageArchivePath() {
        switch (getGameRegionFromDVDDiskID()) {
        case GAME_REGION_JAPAN:
            return "/SystemData/ErrorMessageArchiveJp.arc";
        case GAME_REGION_EUROPE:
            return "/SystemData/ErrorMessageArchiveEu.arc";
        case GAME_REGION_AMERICA:
            return "/SystemData/ErrorMessageArchiveUs.arc";
        case GAME_REGION_CHINA:
            return "/SystemData/ErrorMessageArchiveCn.arc";
        case GAME_REGION_KOREA:
            return "/SystemData/ErrorMessageArchiveKr.arc";
        case GAME_REGION_ASIA:
            return "/SystemData/ErrorMessageArchiveAs.arc";
        default:
            GXColor bg = { 0, 0, 0, 255 };
            GXColor fg = { 255, 255, 255, 255 };
            OSFatal(fg, bg, "Unknown game region code detected!");
            return nullptr;
        }
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
