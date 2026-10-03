#include "neo/System/RiivolutionConfig.h"

namespace neo {
    namespace {
        struct RiivolutionConfig {
            u32 mLocale;
            u8 mReserved[52];
        };

        static const RiivolutionConfig* sRiivolutionConfig = (RiivolutionConfig*)0x80003FC0;
    }

    // -----------------------------------------------------------------------------------------------------------------

#define LOCALE_VALIDITY_MASK (0x80000000)
#define LOCALE_LANGUAGE_MASK (0x7FFFFFFF)

    bool getRiivolutionConfigLocale(u32 *pDest) {
        if (sRiivolutionConfig->mLocale & LOCALE_VALIDITY_MASK) {
            *pDest = sRiivolutionConfig->mLocale & LOCALE_LANGUAGE_MASK;
            return true;
        }

        return false;
    }
}
