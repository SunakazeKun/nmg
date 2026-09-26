"""
Syati module builder

Builds Syati modules from the module source and outputs the linked CustomCode binaries in the active game directory.
The CodeWarrior compiler and assembler are used to generate object binaries and Kamek is used to link them into a
module binary that can be loaded with Syati. Modules are built for all five versions of SMG2.
"""

from config import GAME_PATH, MWCCEPPC_PATH, MWASMEPPC_PATH, KAMEK_PATH, ORG_MAIN_DOLS_PATH
from pathlib import Path

import logging
import subprocess

__all__ = ["build_syati_binaries", "build_syati_module"]


# ======================================================================================================================

MODULE_CWD = Path("syati")
SOURCE_SUBPATH = Path("source")
BUILD_SUBPATH = Path("build")
BIN_SUBPATH = Path("bin")

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
MODULE_SOURCE_FILES = [
    "neo/AreaObj/AstroWarpArea.cpp",
    "neo/Debug/UseResourceHelper.cpp",
    "neo/Effect/EffectObjLine.cpp",
    "neo/Enemy/SkeletalFish.cpp",
    "neo/Extras/BgmChanges.cpp",
    "neo/Extras/FileSelectChanges.cpp",
    "neo/Extras/GameEventFlagChanges.cpp",
    "neo/Extras/MarioFaceShipChanges.cpp",
    "neo/Extras/MiscExtensions.cpp",
    "neo/Extras/PlayerLeftChanges.cpp",
    "neo/Extras/PowerStarChanges.cpp",
    "neo/Extras/PrologueChanges.cpp",
    "neo/Extras/ScoreAttackChanges.cpp",
    "neo/Extras/StarPieceColors.cpp",
    "neo/Extras/TalkChanges.cpp",
    "neo/Extras/UnlockLuigi.cpp",
    "neo/Extras/WorldMapChanges.cpp",
    "neo/Factories/ExtNameObjFactory.cpp",
    "neo/Factories/ExtProductMapCreator.cpp",
    "neo/Factories/ExtSceneObjHolder.cpp",
    "neo/Map/ClippingDummyTarget.cpp",
    "neo/MapObj/AstroCore.cpp",
    "neo/MapObj/AstroLibrary.cpp",
    "neo/MapObj/HipDropNail.cpp",
    "neo/MapObj/IllustItem.cpp",
    "neo/MapObj/KinokoCollect.cpp",
    "neo/MapObj/MarblePlanet.cpp",
    "neo/MapObj/PetitPorterForceEndCtrl.cpp",
    "neo/MapObj/PhantomShipBridge.cpp",
    "neo/MapObj/SpiderCocoon.cpp",
    "neo/MapObj/StringSpiderAttachPoint.cpp",
    "neo/NPC/Butler.cpp",
    "neo/NPC/HelperWitch.cpp",
    "neo/NPC/LibraryBook.cpp",
    "neo/NPC/TicoManiac.cpp",
    "neo/Screen/ExtGalaxyCometScreenFilter.cpp",
    "neo/Screen/ExtInformationObserver.cpp",
    "neo/Screen/ExtStageResultInformer.cpp",
    "neo/Screen/IllustItemCounter.cpp",
    "neo/System/DaredevilModeAccess.cpp",
    "neo/System/DaredevilModeExtensions.cpp",
    "neo/System/GameProgress.cpp",
    "neo/System/IllustItemAccess.cpp",
    "neo/System/IllustItemExtensions.cpp",
    "neo/System/HeapMemory.cpp",
    "neo/System/InStageSave.cpp",
    "neo/System/Language.cpp",
    "neo/System/RiivolutionConfig.cpp",
    "neo/System/StationedFileInfo.cpp",

    "pt/Enemy/Anagon.cpp",
    "pt/Enemy/BallBeamer.cpp",
    "pt/Extras/ExtDummyDisplayModel.cpp",
    "pt/Extras/ExtPowerStarColors.cpp",
    "pt/Extras/ExtShellfishItems.cpp",
    "pt/Extras/ExtSuperSpinDriverGreen.cpp",
    "pt/Extras/VanillaBugfixes.cpp",
    "pt/MapObj/BlueChip.cpp",
    "pt/MapObj/NewMorphItemNeo.cpp",
    "pt/MapObj/PowerStarSpawner.cpp",
    "pt/MapObj/TransparentWall.cpp",
    "pt/Util/ActorUtil.cpp",
    "pt/Util/PlayerUtil.cpp",
    "pt/Util/TalkUtil.cpp",
]
LOADER_SOURCE_FILES = [
    "loader/init.cpp",
    "loader/KamekAccess.cpp",
    "loader/KamekModule.cpp",
]


# ======================================================================================================================

def build_syati_binaries():
    for region in MODULE_REGIONS:
        build_syati_module(region, "RELEASE")
        build_syati_loader(region)


def build_syati_module(region: str, target: str):
    link_files = []

    for src_file_subpath in map(Path, MODULE_SOURCE_FILES):
        src_file_path = SOURCE_SUBPATH / src_file_subpath
        dst_file_path = BUILD_SUBPATH / src_file_subpath.parent / f'{src_file_subpath.stem}.{region}.{target}.o'

        parent_path = MODULE_CWD / dst_file_path.parent
        parent_path.mkdir(parents=True, exist_ok=True)

        if __build_file(src_file_path, dst_file_path, region, target):
            link_files.append(dst_file_path)
        else:
            logging.warning("Ignoring source file %s", src_file_subpath)

    # ------------------------------------------------------

    bin_path = GAME_PATH / f'files/CustomCode/CustomCode_{region}.bin'
    map_path = GAME_PATH / f'files/CustomCode/CustomCode_{region}.map'
    __link_module(KAMEK_PATH, bin_path, map_path, region, link_files)


def build_syati_loader(region: str):
    link_files = []

    for src_file_subpath in map(Path, LOADER_SOURCE_FILES):
        src_file_path = SOURCE_SUBPATH / src_file_subpath
        dst_file_path = BUILD_SUBPATH / src_file_subpath.parent / f'{src_file_subpath.stem}.{region}.o'

        parent_path = MODULE_CWD / dst_file_path.parent
        parent_path.mkdir(parents=True, exist_ok=True)

        if __build_file(src_file_path, dst_file_path, region, "RELEASE"):
            link_files.append(dst_file_path)
        else:
            logging.warning("Ignoring source file %s", src_file_subpath)

    # ------------------------------------------------------

    xml_path = BIN_SUBPATH / f'riivo_{region}.xml'
    dol_path = BIN_SUBPATH / f'main_{region}.dol'
    __link_loader(KAMEK_PATH, xml_path, dol_path, region, link_files)


# ======================================================================================================================

DEFINES = [
    # Example: ("FOO", 2)
]

INCLUDES = [
    "Syati/include",
    "loader",
    "include",
    "source",
]

COMPILER_OPTIONS = [
    "-proc gekko",
    "-enc SJIS",
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
    "-enc SJIS",
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

    for def_name, def_val in defines:
        if def_val is None:
            build_cmd += f' -D{def_name}'
        else:
            build_cmd += f' -D{def_name}={str(def_val)}'

    build_cmd += f' "{source_file_path}" -o "{build_file_path}"'

    return build_cmd


def __build_file(src_file_path: Path, dst_file_path: Path, region: str, target: str) -> bool:
    match src_file_path.suffix:
        case ".cpp":
            compile_cpp(src_file_path, dst_file_path, region, target)
            return True
        case ".c":
            compile_c(src_file_path, dst_file_path, region, target)
            return True
        case ".s":
            assemble_s(src_file_path, dst_file_path, region, target)
            return True
        case _:
            return False


def __link_module(kamek_path: Path, bin_path: Path, map_path: Path, region: str, link_files: list[Path]):
    link_cmd = (f'"{kamek_path}" {" ".join(map(str, link_files))} '
                f'-externals=Syati/symbols/{region}.txt '
                f'-output-kamek="{bin_path}" '
                f'-output-map="{map_path}"')

    logging.info(link_cmd)

    try:
        subprocess.run(link_cmd, cwd=MODULE_CWD, check=True, text=True)
    except subprocess.CalledProcessError as e:
        logging.error(f"Failed to link {region}")
        logging.error(f"{e.stderr.strip()}\n")
        exit(1)


def __link_loader(kamek_path: Path, xml_path: Path, dol_path: Path, region: str, link_files: list[Path]):
    input_dol = ORG_MAIN_DOLS_PATH / f'{region}.dol'

    link_cmd = (f'"{kamek_path}" {" ".join(map(str, link_files))} '
                f'-static=0x80002000 '
                f'-externals=Syati/symbols/{region}.txt '
                f'-output-riiv="{xml_path}" '
                f'-input-dol="{input_dol}" '
                f'-output-dol="{dol_path}"')

    logging.info(link_cmd)

    try:
        subprocess.run(link_cmd, cwd=MODULE_CWD, check=True, text=True)
    except subprocess.CalledProcessError as e:
        logging.error(f"Failed to link {region}")
        logging.error(f"{e.stderr.strip()}\n")
        exit(1)


########################################################################################################################

if __name__ == '__main__':
    build_syati_binaries()
