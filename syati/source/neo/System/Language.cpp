#include "neo/System/Language.h"
#include "neo/System/Language.inc"
#include "neo/System/Region.h"
#include "neo/System/RiivolutionConfig.h"
#include "kamek/hooks.h"

#define SC_LANG_COUNT (10)

namespace {
    struct Language {
        u32 mLocale;
        const char* mName;
    };

    const Language cLanguages[] = {
        { LANGUAGE_JP_JAPANESE,     LOCALE_PREFIX_JP_JAPANESE },
        { LANGUAGE_US_ENGLISH,      LOCALE_PREFIX_US_ENGLISH },
        { LANGUAGE_US_SPANISH,      LOCALE_PREFIX_US_SPANISH },
        { LANGUAGE_US_FRENCH,       LOCALE_PREFIX_US_FRENCH },
        { LANGUAGE_US_PORTUGUESE,   LOCALE_PREFIX_US_PORTUGUESE },
        { LANGUAGE_EU_ENGLISH,      LOCALE_PREFIX_EU_ENGLISH },
        { LANGUAGE_EU_SPANISH,      LOCALE_PREFIX_EU_SPANISH },
        { LANGUAGE_EU_FRENCH,       LOCALE_PREFIX_EU_FRENCH },
        { LANGUAGE_EU_GERMAN,       LOCALE_PREFIX_EU_GERMAN },
        { LANGUAGE_EU_ITALIAN,      LOCALE_PREFIX_EU_ITALIAN },
        { LANGUAGE_EU_DUTCH,        LOCALE_PREFIX_EU_DUTCH },
        { LANGUAGE_EU_PORTUGUESE,   LOCALE_PREFIX_EU_PORTUGUESE },
        { LANGUAGE_EU_RUSSIAN,      LOCALE_PREFIX_EU_RUSSIAN },
        { LANGUAGE_CN_SIMP_CHINESE, LOCALE_PREFIX_CN_SIMP_CHINESE },
        { LANGUAGE_CN_TRAD_CHINESE, LOCALE_PREFIX_CN_TRAD_CHINESE },
        { LANGUAGE_KR_KOREAN,       LOCALE_PREFIX_KR_KOREAN },
        { LANGUAGE_AS_TRAD_CHINESE, LOCALE_PREFIX_AS_TRAD_CHINESE },
    };

    // -----------------------------------------------------------------------------------------------------------------

    const u8 cSCLangToGameLangUs[SC_LANG_COUNT] = {
        LANGUAGE_JP_JAPANESE,
        LANGUAGE_US_ENGLISH,
        LANGUAGE_EU_GERMAN,
        LANGUAGE_US_FRENCH,
        LANGUAGE_US_SPANISH,
        LANGUAGE_EU_ITALIAN,
        LANGUAGE_EU_DUTCH,
        LANGUAGE_CN_SIMP_CHINESE,
        LANGUAGE_AS_TRAD_CHINESE,
        LANGUAGE_KR_KOREAN,
    };
    const u8 cSCLangToGameLangEu[SC_LANG_COUNT] = {
        LANGUAGE_JP_JAPANESE,
        LANGUAGE_EU_ENGLISH,
        LANGUAGE_EU_GERMAN,
        LANGUAGE_EU_FRENCH,
        LANGUAGE_EU_SPANISH,
        LANGUAGE_EU_ITALIAN,
        LANGUAGE_EU_DUTCH,
        LANGUAGE_CN_SIMP_CHINESE,
        LANGUAGE_AS_TRAD_CHINESE,
        LANGUAGE_KR_KOREAN,
    };

    // =================================================================================================================
    // Extension of MR::getDecidedLanguageFromIPL
    // =================================================================================================================

    u32 extGetDecidedLanguageFromIPL() {
        u32 locale;

        if (neo::getRiivolutionConfigLocale(&locale)) {
            return locale;
        }

        switch (neo::getGameRegionFromDVDDiskID()) {
        case neo::GAME_REGION_JAPAN:
            return LANGUAGE_JP_JAPANESE;
        case neo::GAME_REGION_AMERICA:
            return cSCLangToGameLangUs[SCGetLanguage()];
        case neo::GAME_REGION_EUROPE:
            return cSCLangToGameLangEu[SCGetLanguage()];
        case neo::GAME_REGION_CHINA:
            return LANGUAGE_CN_SIMP_CHINESE;
        case neo::GAME_REGION_KOREA:
            return LANGUAGE_KOREAN;
        case neo::GAME_REGION_ASIA:
            return LANGUAGE_AS_TRAD_CHINESE;
        default:
            return LANGUAGE_US_ENGLISH;
        }
    }

#if defined(SB4_JEP)
    kmBranch(0x804BD1E0, extGetDecidedLanguageFromIPL);
#elif defined(SB4_WK)
    kmBranch(0x804BD250, extGetDecidedLanguageFromIPL);
#endif


    // =================================================================================================================
    // Extension of MR::getLanguageFromIPL
    // =================================================================================================================

    u32 extGetLanguageFromIPL() {
        u32 language = GET_LANGUAGE_FROM_LOCALE(MR::getLanguage());
        return language < SC_LANG_COUNT ? language : SC_LANG_ENGLISH;
    }

#if defined(SB4_JEP)
    kmBranch(0x804BD240, extGetLanguageFromIPL);
#elif defined(SB4_WK)
    kmBranch(0x804BD2B0, extGetLanguageFromIPL);
#endif


    // =================================================================================================================
    // Extension of MR::getCurrentLanguagePrefix
    // =================================================================================================================

    const char* extGetCurrentLanguagePrefix() {
        u32 locale = MR::getLanguage();

        for (int i = 0; i < ARRAY_SIZE(cLanguages); i++) {
            if (cLanguages[i].mLocale == locale) {
                return cLanguages[i].mName;
            }
        }

        return LOCALE_PREFIX_US_ENGLISH;
    }

#if defined(SB4_JEP)
    kmBranch(0x804BD270, extGetCurrentLanguagePrefix);
#elif defined(SB4_WK)
    kmBranch(0x804BD2E0, extGetCurrentLanguagePrefix);
#endif


    // =================================================================================================================
    // Extension of MR::getCurrentRegionPrefix
    // =================================================================================================================

#if defined(SB4_JEP)
    const char* extGetCurrentRegionPrefix(const char* regionPrefix) {
        if (regionPrefix == nullptr) {
            u32 locale = MR::getLanguage();
            u32 region = GET_REGION_FROM_LOCALE(locale);

            if (region == REGION_AS) {
                return "As";
            }
        }

        return regionPrefix;
    }

    kmBranch(0x804BD38C, extGetCurrentRegionPrefix);
#endif


    // =================================================================================================================
    // Adjust count returned by MR::getLanguageNum
    // =================================================================================================================

#if defined(SB4_JEP)
    kmWrite16(0x804BD392, ARRAY_SIZE(cLanguages));
#elif defined(SB4_WK)
    kmWrite16(0x804BD422, ARRAY_SIZE(cLanguages));
#endif


    // =================================================================================================================
    // Extension of MR::getLanguagePrefixByIndex
    // =================================================================================================================

    const char* extGetLanguagePrefixByIndex(u32 index) {
        return cLanguages[index].mName;
    }

#if defined(SB4_JEP)
    kmBranch(0x804BD3A0, extGetLanguagePrefixByIndex);
#elif defined(SB4_WK)
    kmBranch(0x804BD430, extGetLanguagePrefixByIndex);
#endif
}
