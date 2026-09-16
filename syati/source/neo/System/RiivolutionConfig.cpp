#include "neo/System/RiivolutionConfig.h"

namespace neo {
    namespace {
        struct RiivolutionConfig {
            bool mLocaleValid : 1;
            u32 mLocaleCode : 31;
            u8 mReserved[36];
        };

        static const RiivolutionConfig* sRiivolutionConfig = (RiivolutionConfig *)0x80003FC0;
    };

    // -----------------------------------------------------------------------------------------------------------------

    bool getRiivolutionConfigLocale(u32 *pDest) {
        if (sRiivolutionConfig->mLocaleValid) {
            *pDest = sRiivolutionConfig->mLocaleCode;
            return true;
        }

        return false;
    }
}
