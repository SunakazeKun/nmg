from dotenv import load_dotenv
from pathlib import Path

import os

load_dotenv()


########################################################################################################################

GAME_PATH = Path(os.getenv("GAME_PATH"))

ORG_LOCALIZE_DATA_PATH = Path(os.getenv("ORG_LOCALIZE_DATA_PATH"))


# ======================================================================================================================
# Localization
# ======================================================================================================================

SUPPORTED_LANGUAGES = [
    "EuEnglish",
    "EuGerman",
    "EuItalian",
#   "EuSpanish",
#   "EuFrench",
    "EuDutch",
    "UsEnglish",
#   "UsSpanish",
#   "UsFrench",
#   "UsPortuguese",   # Switch only
    "JpJapanese",
#   "KrKorean",
#   "CnSimpChinese",  # Switch only
#   "CnTradChinese",  # Unused
#   "AsTradChinese",
]

PARTIAL_REPLACEMENTS = [
    "SystemMessage",
    "LayoutMessage",
    "SceneCommonMessage",
]
