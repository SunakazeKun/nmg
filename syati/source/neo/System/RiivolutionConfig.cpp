#include "neo/System/RiivolutionConfig.h"

namespace {
    struct RiivolutionConfig {
        /* 0x00 */ u8 mLocale;
        /* 0x01 */ u8 mReserved[55];
    };

    static const RiivolutionConfig* sRiivolutionConfig = (RiivolutionConfig*)0x80003FC0;
}

#define LOCALE_VALIDITY_MASK (0x80)
#define LOCALE_LANGUAGE_MASK (0x7F)

namespace neo {
    bool getRiivolutionConfigLocale(u32 *pDest) {
        if (sRiivolutionConfig->mLocale & LOCALE_VALIDITY_MASK) {
            *pDest = sRiivolutionConfig->mLocale & LOCALE_LANGUAGE_MASK;
            return true;
        }

        return false;
    }
}
