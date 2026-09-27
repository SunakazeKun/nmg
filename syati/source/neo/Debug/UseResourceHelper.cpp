#include "kamek/hooks.h"
#include "Game/Util/FileUtil.h"
#include "JSystem/JGadget/hashcode.h"

#ifdef DEBUG
namespace {
    bool printArchive(const char* pName) {
        OSReport("ARCHIVE %s\n", pName);
        return MR::isFileExist(pName, false);
    }

#if defined(SB4_JEP)
    kmCall(0x804B1FE0, printArchive);
#elif defined(SB4_WK)
    kmCall(0x804B2050, printArchive);
#endif

    // -----------------------------------------------------------------------------------------------------------------

    u32 printSound(const char* pName) {
        OSReport("SOUND %s\n", pName);
        return JGadget::getHashCode(pName);
    }

    kmCall(0x80086544, printSound);
}
#endif // DEBUG
