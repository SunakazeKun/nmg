#include "neo/System/RiivolutionConfig.h"

#define RVL_CFG_LOCALE_MASK_VALIDITY (1 << 31)
#define RVL_CFG_LOCALE_MASK_LANGUAGE (~RVL_CFG_LOCALE_MASK_VALIDITY)

namespace neo {
    namespace {
        struct RiivolutionConfig {
            u32 mLocale;
            u8 mReserved[60];
        };

        static const RiivolutionConfig* sRiivolutionConfig = (RiivolutionConfig *)0x80003FC0;
    };

    // -----------------------------------------------------------------------------------------------------------------

    bool getRiivolutionConfigLocale(u32 *pDest) {
        if (sRiivolutionConfig->mLocale & RVL_CFG_LOCALE_MASK_VALIDITY) {
            *pDest = sRiivolutionConfig->mLocale & RVL_CFG_LOCALE_MASK_LANGUAGE;
            return true;
        }

        return false;
    }
}
