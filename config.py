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

GAME_PATH = Path(os.getenv("GAME_PATH"))

MWCCEPPC_PATH = Path(os.getenv("MWCCEPPC_PATH"))
KAMEK_PATH = Path(os.getenv("KAMEK_PATH"))

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
