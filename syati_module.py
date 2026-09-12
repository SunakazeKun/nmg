from config import MWCCEPPC_PATH, MWASMEPPC_PATH, KAMEK_PATH, GAME_PATH
from pathlib import Path

import logging
import subprocess

__all__ = ["build_syati_module"]


# ======================================================================================================================

MODULE_CWD = Path("syati")
SOURCE_SUBPATH = Path("source")
BUILD_SUBPATH = Path("build")

assert MWCCEPPC_PATH.is_file(), "Missing CodeWarrior compiler!"
assert MWASMEPPC_PATH.is_file(), "Missing CodeWarrior assembler!"
assert KAMEK_PATH.is_file(), "Missing Kamek linker!"


# ======================================================================================================================

MODULE_REGIONS = [
    "SB4P",
    "SB4E",
    "SB4J",
    "SB4W",
    "SB4K",
]
MODULE_TARGETS = [
    "RELEASE",
]
SOURCE_FILES = [
    "init.cpp",
    "UseResourceHelper.cpp",

    "neo/AreaObj/AstroWarpArea.cpp",
    "neo/Effect/EffectObjLine.cpp",
    "neo/Enemy/Chokomera.cpp",
    "neo/Enemy/SkeletalFish.cpp",
    "neo/Extras/AudioChanges.cpp",
    "neo/Extras/CometChanges.cpp",
    "neo/Extras/FileSelectChanges.cpp",
    "neo/Extras/GameEventFlagChanges.cpp",
    "neo/Extras/IntroHubChanges.cpp",
    "neo/Extras/LanguageChanges.cpp",
    "neo/Extras/MiscExtensions.cpp",
    "neo/Extras/PlayerLeftRemoval.cpp",
    "neo/Extras/PlayerLeftReplacements.cpp",
    "neo/Extras/PowerStarChanges.cpp",
    "neo/Extras/ScoreAttackChanges.cpp",
    "neo/Extras/StarPieceColors.cpp",
    "neo/Extras/StationedFileInfoOverwrite.cpp",
    "neo/Extras/TalkChanges.cpp",
    "neo/Extras/UnlockLuigi.cpp",
    "neo/Factories/ExtNameObjFactory.cpp",
    "neo/Factories/ExtProjectMapCreator.cpp",
    "neo/Factories/ExtSceneObjHolder.cpp",
    "neo/Map/ClippingDummyTarget.cpp",
    "neo/MapObj/AstroCore.cpp",
    "neo/MapObj/AstroLibrary.cpp",
    "neo/MapObj/HipDropNail.cpp",
    "neo/MapObj/IllustItem.cpp",
    "neo/MapObj/KinokoCollect.cpp",
    "neo/MapObj/MarblePlanet.cpp",
    "neo/MapObj/PhantomShipBridge.cpp",
    "neo/MapObj/SpiderCocoon.cpp",
    "neo/MapObj/StringSpiderAttachPoint.cpp",
    "neo/NPC/Butler.cpp",
    "neo/NPC/HelperWitch.cpp",
    "neo/NPC/LibraryBook.cpp",
    "neo/NPC/TicoManiac.cpp",
    "neo/Screen/ExtInformationObserver.cpp",
    "neo/Screen/ExtStageResultInformer.cpp",
    "neo/Screen/IllustItemCounter.cpp",
    "neo/System/DaredevilModeAccess.cpp",
    "neo/System/DaredevilModeExtensions.cpp",
    "neo/System/GameProgress.cpp",
    "neo/System/IllustItemAccess.cpp",
    "neo/System/IllustItemExtensions.cpp",
    "neo/System/InStageSave.cpp",

    "pt/Enemy/Anagon.cpp",
    "pt/Enemy/BallBeamer.cpp",
    "pt/Extras/ExtDummyDisplayModel.cpp",
    "pt/Extras/ExtPowerStarColors.cpp",
    "pt/Extras/ExtShellfishItems.cpp",
    "pt/Extras/ExtSuperSpinDriverGreen.cpp",
    "pt/Extras/MiscExtensions.cpp",
    "pt/Extras/VanillaBugfixes.cpp",
    "pt/MapObj/BlueChip.cpp",
    "pt/MapObj/NewMorphItemNeo.cpp",
    "pt/MapObj/PowerStarSpawner.cpp",
    "pt/MapObj/TransparentWall.cpp",
    "pt/Util/ActorUtil.cpp",
    "pt/Util/PlayerUtil.cpp",
    "pt/Util/TalkUtil.cpp",
]


# ======================================================================================================================

def build_syati_module(region: str, target: str):
    link_files = []

    for src_file_subpath in map(Path, SOURCE_FILES):
        src_file_path = SOURCE_SUBPATH / src_file_subpath
        dst_file_path = BUILD_SUBPATH / src_file_subpath.parent / f'{src_file_subpath.stem}.{region}.{target}.o'

        parent_path = MODULE_CWD / dst_file_path.parent
        parent_path.mkdir(parents=True, exist_ok=True)

        match src_file_subpath.suffix:
            case ".cpp":
                compile_cpp(src_file_path, dst_file_path, region, target)
                link_files.append(dst_file_path)
            case ".c":
                compile_c(src_file_path, dst_file_path, region, target)
                link_files.append(dst_file_path)
            case ".s":
                assemble_s(src_file_path, dst_file_path, region, target)
                link_files.append(dst_file_path)
            case _:
                logging.warning("Ignoring source file %s", src_file_subpath)

    # ------------------------------------------------------

    module_path = GAME_PATH / f'files/CustomCode/CustomCode_{region}.bin'
    link_cmd = (f'"{KAMEK_PATH}" {" ".join(map(str, link_files))} '
                f'-externals=Syati/symbols/{region}.txt '
                f'-output-kamek={module_path}')

    logging.info(link_cmd)

    try:
        subprocess.run(link_cmd, cwd=MODULE_CWD, check=True, text=True)
    except subprocess.CalledProcessError as e:
        logging.error(f"Failed to link {region} {target}")
        logging.error(f"{e.stderr.strip()}\n")
        exit(1)


# ======================================================================================================================

DEFINES = [
    # Example: ("FOO", 2)
]

INCLUDES = [
    "Syati/include",
    "include",
]

COMPILER_OPTIONS = [
    "-proc gekko",
    "-nodefaults",
    "-O4,s",
    "-inline on",
    "-rtti off",
    "-enum int",
    "-fp hard",
    "-func_align 4",
    "-sdata 0",
    "-sdata2 0",
    "-DGEKKO",
    "-DMTX_USE_PS",
]

ASSEMBLER_OPTIONS = [
    "-proc gekko",
    "-DGEKKO",
    "-DMTX_USE_PS",
]


def compile_cpp(source_file_path: Path, build_file_path: Path, region: str, target: str):
    build_cmd = __make_build_cmd(MWCCEPPC_PATH, "-c -lang=c++ -Cpp_exceptions=off",
                                 COMPILER_OPTIONS, INCLUDES, DEFINES, region, target,
                                 source_file_path, build_file_path)

    logging.info(build_cmd)

    try:
        subprocess.run(build_cmd, cwd=MODULE_CWD, check=True, text=True)
    except subprocess.CalledProcessError as e:
        logging.error(f"Failed to compile {source_file_path.name}")
        logging.error(f"{e.stderr.strip()}\n")
        exit(1)


def compile_c(source_file_path: Path, build_file_path: Path, region: str, target: str):
    build_cmd = __make_build_cmd(MWCCEPPC_PATH, "-c -lang=c",
                                 COMPILER_OPTIONS, INCLUDES, DEFINES, region, target,
                                 source_file_path, build_file_path)

    logging.info(build_cmd)

    try:
        subprocess.run(build_cmd, cwd=MODULE_CWD, check=True, text=True)
    except subprocess.CalledProcessError as e:
        logging.error(f"Failed to compile {source_file_path.name}")
        logging.error(f"{e.stderr.strip()}\n")
        exit(1)


def assemble_s(source_file_path: Path, build_file_path: Path, region: str, target: str):
    build_cmd = __make_build_cmd(MWCCEPPC_PATH, "-c",
                                 ASSEMBLER_OPTIONS, INCLUDES, DEFINES, region, target,
                                 source_file_path, build_file_path)

    logging.info(build_cmd)

    try:
        subprocess.run(build_cmd, cwd=MODULE_CWD, check=True, text=True)
    except subprocess.CalledProcessError as e:
        logging.error(f"Failed to assemble {source_file_path.name}")
        logging.error(f"{e.stderr.strip()}\n")
        exit(1)


def __make_build_cmd(mw_tool_path: Path, command: str, options: list[str], includes: list[str],
                     defines: list[tuple[str, int]], region: str, target: str,
                     source_file_path: Path, build_file_path: Path):
    build_cmd = (f'"{mw_tool_path}" {command} '
                 f'{" ".join(options)} ')
    build_cmd += " -I- ".join(map(lambda i: f'-i "{i}"', includes))
    build_cmd += f' -D{region} -D{target}'

    # TODO: Remove this once deprecated define names are fully replaced.
    match region:
        case "SB4P":
            build_cmd += " -DPAL"
        case "SB4E":
            build_cmd += " -DUSA"
        case "SB4J":
            build_cmd += " -DJPN"
        case "SB4W":
            build_cmd += " -DTWN"
        case "SB4K":
            build_cmd += " -DKOR"

    for def_name, def_val in defines:
        if def_val is None:
            build_cmd += f' -D{def_name}'
        else:
            build_cmd += f' -D{def_name}={str(def_val)}'

    build_cmd += f' "{source_file_path}" -o "{build_file_path}"'

    return build_cmd


########################################################################################################################

def __main():
    for region in MODULE_REGIONS:
        for target in MODULE_TARGETS:
            build_syati_module(region, target)


if __name__ == '__main__':
    __main()
