from dataclasses import dataclass
from dotenv import load_dotenv
from pathlib import Path
from typing import Iterable

import os
import logging

load_dotenv()

logging.basicConfig(
    level=logging.INFO,
    format="%(levelname)s : %(message)s"
)


########################################################################################################################

PROJECT_TITLE = "NMG_2026"
PROJECT_SHORT = "nmg"
PROJECT_VERSION = "1.6.1"

GAME_ID = os.getenv("GAME_ID")
GAME_PATH = Path(os.getenv("GAME_PATH")).absolute()

MWCCEPPC_PATH = Path(os.getenv("MWCCEPPC_PATH")).absolute()
MWASMEPPC_PATH = Path(os.getenv("MWASMEPPC_PATH")).absolute()
KAMEK_PATH = Path(os.getenv("KAMEK_PATH")).absolute()

NEW_AUDIO_RES_PATH = Path(os.getenv("NEW_AUDIO_RES_PATH")).absolute()
ORG_MAIN_DOLS_PATH = Path(os.getenv("ORG_MAIN_DOLS_PATH")).absolute()

assert GAME_ID in {"SB4P", "SB4E", "SB4J", "SB4W", "SB4K"}, f'{GAME_ID} is not a valid game ID!'
assert GAME_PATH.is_dir(), "Active game dump is missing!"
assert MWCCEPPC_PATH.is_file(), "CodeWarrior PPC EABI C/C++ Compiler is missing!"
assert MWASMEPPC_PATH.is_file(), "CodeWarrior PPC EABI Assembler is missing!"
assert KAMEK_PATH.is_file(), "Kamek linker is missing!"
assert NEW_AUDIO_RES_PATH.is_dir(), "Modified AudioRes folder is missing!"
assert ORG_MAIN_DOLS_PATH.is_dir(), "Original main.dol folder is missing!"


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
STAGE_NAME_STARSHIP_MARIO         = "MarioFaceShipGalaxy"
STAGE_NAME_WORLD_1                = "WorldMap01Galaxy"
STAGE_NAME_FILE_SELECT            = "FileSelect"

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
    (STAGE_NAME_ROSALINA_INTRO,                 3, False),
    (STAGE_NAME_STARSHIP_MARIO,                 4, False),
    (STAGE_NAME_WORLD_1,                        1, False),
    (STAGE_NAME_FILE_SELECT,                    1, False),
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


def get_language_region(language: str) -> str:
    return language[:2]


# ======================================================================================================================
# Deployment
# ======================================================================================================================

@dataclass(init=True, kw_only=True, frozen=True)
class RiivolutionTarget:
    id: str
    region: str
    error_msg_arc_addr: int
    error_msg_arc_size: int


RIIVO_TARGETS = [
    RiivolutionTarget(id="SB4", region="P", error_msg_arc_addr=0x8064E220, error_msg_arc_size=0xB780),
    RiivolutionTarget(id="SB4", region="E", error_msg_arc_addr=0x8064E220, error_msg_arc_size=0x6080),
    RiivolutionTarget(id="SB4", region="J", error_msg_arc_addr=0x8064E220, error_msg_arc_size=0x5860),
    RiivolutionTarget(id="SB4", region="W", error_msg_arc_addr=0x8064E7C0, error_msg_arc_size=0x5D80),
    RiivolutionTarget(id="SB4", region="K", error_msg_arc_addr=0x8064E7C0, error_msg_arc_size=0x4920),
]

RIIVO_CONFIG_LOCALE_ADDR = 0x80003FC0
RIIVO_CONFIG_LOCALE_VALID = 1 << 7
RIIVO_CONFIG_LOCALE_INFO = [
    (0x10 | RIIVO_CONFIG_LOCALE_VALID, "JpJapanese",    "Japanese"),
    (0x21 | RIIVO_CONFIG_LOCALE_VALID, "UsEnglish",     "English (US)"),
    (0x01 | RIIVO_CONFIG_LOCALE_VALID, "EuEnglish",     "English (UK)"),
    (0x02 | RIIVO_CONFIG_LOCALE_VALID, "EuGerman",      "German"),
    (0x03 | RIIVO_CONFIG_LOCALE_VALID, "EuFrench",      "French (FR)"),
    (0x23 | RIIVO_CONFIG_LOCALE_VALID, "UsFrench",      "French (QC)"),
    (0x04 | RIIVO_CONFIG_LOCALE_VALID, "EuSpanish",     "Spanish (ES)"),
    (0x24 | RIIVO_CONFIG_LOCALE_VALID, "UsSpanish",     "Spanish (MX)"),
    (0x05 | RIIVO_CONFIG_LOCALE_VALID, "EuItalian",     "Italian"),
    (0x06 | RIIVO_CONFIG_LOCALE_VALID, "EuDutch",       "Dutch"),
#   (0x0A | RIIVO_CONFIG_LOCALE_VALID, "EuPortuguese",  "Portuguese (PT)"),
#   (0x2A | RIIVO_CONFIG_LOCALE_VALID, "UsPortuguese",  "Portuguese (BR)"),
#   (0x0B | RIIVO_CONFIG_LOCALE_VALID, "EuRussian",     "Russian"),
#   (0x37 | RIIVO_CONFIG_LOCALE_VALID, "CnSimpChinese", "Simp. Chinese"),
    (0x58 | RIIVO_CONFIG_LOCALE_VALID, "AsTradChinese", "Trad. Chinese"),
    (0x49 | RIIVO_CONFIG_LOCALE_VALID, "KrKorean",      "Korean"),
]

IGNORE_FILES = [
    "/galaxies.json",
    "/objectdb.json",

    "/CustomCode/CustomCode_SB4E.map",
    "/CustomCode/CustomCode_SB4J.map",
    "/CustomCode/CustomCode_SB4K.map",
    "/CustomCode/CustomCode_SB4P.map",
    "/CustomCode/CustomCode_SB4W.map",

    "/LayoutData/DestroyCounter.arc",
    "/LayoutData/GameSet.arc",
    "/LayoutData/PrologueText.arc",

    "/ObjectData/LifeUpHeart.arc",
    "/ObjectData/SoundSyncGhostStepD.arc",
    "/ObjectData/SoundSyncGhostStepG.arc",
    "/ObjectData/SoundSyncGhostStepJ.arc",
    "/ObjectData/TestGalaxyPlanet.arc"
]

IGNORE_FOLDERS = [
    "/UseResourceLogs",
    "/StageData/TestGalaxy"
]
