from dataclasses import dataclass
from dotenv import load_dotenv
from pathlib import Path

import os
import logging

load_dotenv()

logging.basicConfig(
    level=logging.INFO,
    format="%(levelname)s : %(message)s"
)


########################################################################################################################

PROJECT_TITLE = "NMG_2026"
PROJECT_VERSION = "1.0.0"

GAME_PATH = Path(os.getenv("GAME_PATH")).absolute()

MWCCEPPC_PATH = Path(os.getenv("MWCCEPPC_PATH")).absolute()
MWASMEPPC_PATH = Path(os.getenv("MWASMEPPC_PATH")).absolute()
KAMEK_PATH = Path(os.getenv("KAMEK_PATH")).absolute()

ORG_LOCALIZE_DATA_PATH = Path(os.getenv("ORG_LOCALIZE_DATA_PATH"))


# ======================================================================================================================
# Stages
# ======================================================================================================================

STAGE_NAME_GALACTIC_GARDEN_GALAXY = "IslandFleetGalaxy"
STAGE_NAME_SWEET_TREAT_GALAXY     = "YosshiHomeGalaxy"
STAGE_NAME_GLOOMY_GALLEON_GALAXY  = "DigMineGalaxy"
STAGE_NAME_BATTLE_BLAST_GALAXY    = "MokumokuValleyGalaxy"
STAGE_NAME_EEP_BEEP_GALAXY        = "AbekobeGalaxy"
STAGE_NAME_TURNLOG_GALAXY         = "RedBlueExGalaxy"
STAGE_NAME_KRAKEN_KING_GALAXY     = "VsKoopaJrLv1Galaxy"
STAGE_NAME_COMET_OBSERVATORY      = "AstroGalaxy"
STAGE_NAME_ROSALINA_INTRO         = "PeachCastleGalaxy"

USE_RESOURCE_STAGE_INFOS = [
    # Stage name                        Scenarios  Use common?
    (STAGE_NAME_GALACTIC_GARDEN_GALAXY,         3, True),
    (STAGE_NAME_SWEET_TREAT_GALAXY,             2, True),
    (STAGE_NAME_GLOOMY_GALLEON_GALAXY,          2, True),
    (STAGE_NAME_BATTLE_BLAST_GALAXY,            2, True),
    (STAGE_NAME_EEP_BEEP_GALAXY,                2, True),
    (STAGE_NAME_TURNLOG_GALAXY,                 2, True),
    (STAGE_NAME_KRAKEN_KING_GALAXY,             2, True),
    (STAGE_NAME_COMET_OBSERVATORY,              2, True),
    (STAGE_NAME_ROSALINA_INTRO,                 3, False)
]


# ======================================================================================================================
# Localization
# ======================================================================================================================

SUPPORTED_LANGUAGES = [
    "EuEnglish",
    "EuGerman",
    "EuItalian",
    "EuSpanish",
    "EuFrench",
    "EuDutch",
#   "EuPortuguese",   # Custom
#   "EuRussian",      # Custom
    "UsEnglish",
    "UsSpanish",
    "UsFrench",
#   "UsPortuguese",   # Switch only
    "JpJapanese",
    "KrKorean",
#   "CnSimpChinese",  # Switch only
#   "CnTradChinese",  # Unused
    "AsTradChinese",
]

PARTIAL_REPLACEMENTS = [
    "SystemMessage",
    "LayoutMessage",
    "SceneCommonMessage",
]


# ======================================================================================================================
# Deployment
# ======================================================================================================================

@dataclass(init=True, kw_only=True, frozen=True)
class RiivolutionTarget:
    id: str
    region: str
    error_message_archive_addr: int
    language_options: list[str]


RIIVO_TARGETS = [
    RiivolutionTarget(id="SB4", region="P", error_message_archive_addr=0x8064E220, language_options=[
        "EuEnglish",
        "EuGerman",
        "EuItalian",
        "EuSpanish",
        "EuFrench",
        "EuDutch",
    ]),
    RiivolutionTarget(id="SB4", region="E", error_message_archive_addr=0x8064E220, language_options=[
        "UsEnglish",
        "UsSpanish",
        "UsFrench",
    ]),
    RiivolutionTarget(id="SB4", region="J", error_message_archive_addr=0x8064E220, language_options=[]),
    RiivolutionTarget(id="SB4", region="W", error_message_archive_addr=0x8064E7C0, language_options=[]),
    RiivolutionTarget(id="SB4", region="K", error_message_archive_addr=0x8064E7C0, language_options=[]),
]

RIIVO_CONFIG_LOCALE_ADDR = 0x80003FC0
RIIVO_CONFIG_LOCALE_VALID = 1 << 31
RIIVO_CONFIG_LOCALE_INFO = {
    "JpJapanese":    (0x10 | RIIVO_CONFIG_LOCALE_VALID, "Japanese"),
    "UsEnglish":     (0x21 | RIIVO_CONFIG_LOCALE_VALID, "English"),
    "UsSpanish":     (0x24 | RIIVO_CONFIG_LOCALE_VALID, "Spanish"),
    "UsFrench":      (0x23 | RIIVO_CONFIG_LOCALE_VALID, "French"),
    "UsPortuguese":  (0x2A | RIIVO_CONFIG_LOCALE_VALID, "Portuguese"),
    "EuEnglish":     (0x01 | RIIVO_CONFIG_LOCALE_VALID, "English"),
    "EuSpanish":     (0x04 | RIIVO_CONFIG_LOCALE_VALID, "Spanish"),
    "EuFrench":      (0x03 | RIIVO_CONFIG_LOCALE_VALID, "French"),
    "EuGerman":      (0x02 | RIIVO_CONFIG_LOCALE_VALID, "German"),
    "EuItalian":     (0x05 | RIIVO_CONFIG_LOCALE_VALID, "Italian"),
    "EuDutch":       (0x06 | RIIVO_CONFIG_LOCALE_VALID, "Dutch"),
    "EuPortuguese":  (0x0A | RIIVO_CONFIG_LOCALE_VALID, "Portuguese"),
    "EuRussian":     (0x0B | RIIVO_CONFIG_LOCALE_VALID, "Russian"),
    "CnSimpChinese": (0x37 | RIIVO_CONFIG_LOCALE_VALID, "Simp. Chinese"),
    "CnTradChinese": (0x38 | RIIVO_CONFIG_LOCALE_VALID, "Trad. Chinese"),
    "KrKorean":      (0x49 | RIIVO_CONFIG_LOCALE_VALID, "Korean"),
    "AsTradChinese": (0x58 | RIIVO_CONFIG_LOCALE_VALID, "Trad. Chinese"),
}

IGNORE_FILES = [
    "/galaxies.json",
    "/objectdb.json",

    "/CustomCode/CustomCode_JPN.map",
    "/CustomCode/CustomCode_PAL.map",
    "/CustomCode/CustomCode_USA.map",
    "/CustomCode/CustomCode_KOR.map",
    "/CustomCode/CustomCode_TWN.map",

    "/LayoutData/DestroyCounter.arc",
    "/LayoutData/GameSet.arc",
    "/LayoutData/PrologueText.arc",

    "/ObjectData/LifeUpHeart.arc",
    "/ObjectData/SoundSyncGhostStepD.arc",
    "/ObjectData/SoundSyncGhostStepG.arc",
    "/ObjectData/SoundSyncGhostStepI.arc",
    "/ObjectData/SoundSyncGhostStepJ.arc",
    "/ObjectData/TestGalaxyPlanet.arc"
]

IGNORE_FOLDERS = [
    "/UseResourceLogs",
    "/StageData/TestGalaxy"
]
