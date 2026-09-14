"""
Distributable ZIP packaging

Packs the ZIP file to be distributed containing only the new and changed files as well as the Riivolution configurations
necessary for loading and playing Neo Mario Galaxy. Edited and new files are efficiently identified by comparing the
active game files against known file names and hashes from the unmodified SMG2 games.
"""

from config import PROJECT_TITLE, PROJECT_VERSION, GAME_PATH, RIIVOLUTION_TARGETS, IGNORE_FILES, IGNORE_FOLDERS
from xml.etree import ElementTree as ET
from tools.dataio import read_json
from pathlib import Path
from datetime import datetime
from hashlib import md5

import os
import shutil
import getpass
import time
import logging

__all__ = ["pack_zip_archive"]


########################################################################################################################

SYATI_LOADER_PATH = Path("syati/Syati/loader")
VANILLA_FILE_HASHES_PATH = Path("data/vanilla_file_hashes.json")

COPY_DIST_PATH = Path("data/dist")
BUILD_DIST_PATH = Path("dist")

DIST_NMG_PATH = BUILD_DIST_PATH / "nmg"
DIST_RIIVO_PATH = BUILD_DIST_PATH / "riivolution"

DIST_GAME_PATH = DIST_NMG_PATH / "game"
DIST_SAVE_PATH = DIST_NMG_PATH / "save"
DIST_MEMORY_PATH = DIST_NMG_PATH / "memory"


def pack_zip_archive():
    if BUILD_DIST_PATH.exists():
        shutil.rmtree(BUILD_DIST_PATH)

    logging.info("Copying base dist folder ...")
    shutil.copytree(COPY_DIST_PATH, BUILD_DIST_PATH)

    DIST_NMG_PATH.mkdir(parents=True, exist_ok=True)
    DIST_RIIVO_PATH.mkdir(parents=True, exist_ok=True)

    DIST_GAME_PATH.mkdir(parents=True, exist_ok=True)
    DIST_SAVE_PATH.mkdir(parents=True, exist_ok=True)
    DIST_MEMORY_PATH.mkdir(parents=True, exist_ok=True)

    # ------------------------------------------------------

    logging.info("Collecting dist files ...")

    vanilla_file_hashes = read_json(VANILLA_FILE_HASHES_PATH)
    __traverse_game_folder(GAME_PATH / "files", DIST_GAME_PATH, "", vanilla_file_hashes)

    # ------------------------------------------------------

    for game_id, game_region, error_message_archive_addr in RIIVOLUTION_TARGETS:
        __make_riivolution_xml(game_id, game_region, error_message_archive_addr,
                               SYATI_LOADER_PATH / f'riivo_{game_id}{game_region}.xml',
                               DIST_RIIVO_PATH / f'nmg-{game_id}{game_region}.xml')

    # ------------------------------------------------------

    logging.info("Packaging dist archive ...")

    build_time = datetime.now()
    utime = time.mktime(build_time.timetuple())

    with open(DIST_NMG_PATH / "build-info.txt", "w") as f:
        f.write(f'Project-Name: {PROJECT_TITLE}\n'
                f'Project-Version: v{PROJECT_VERSION}\n'
                f'Build-Date: {build_time.strftime("%Y-%m-%d %H:%M:%S")}\n'
                f'Host: {getpass.getuser()}\n')

    for root, folders, files in os.walk(BUILD_DIST_PATH):
        for child in folders + files:
            file_path = os.path.join(root, child)
            os.utime(file_path, (utime, utime))

    shutil.make_archive(f'{PROJECT_TITLE}_v{PROJECT_VERSION}', "zip", BUILD_DIST_PATH)


# ----------------------------------------------------------------------------------------------------------------------

def __traverse_game_folder(src_folder_path: Path, dst_folder_path: Path, iso_folder_path: str,
                           vanilla_file_hashes: dict[str, list[str]]):
    for src_child_path in src_folder_path.iterdir():
        child_name = src_child_path.name
        dst_child_path = dst_folder_path / child_name
        iso_child_path = iso_folder_path + '/' + child_name

        if src_child_path.is_dir() and iso_child_path not in IGNORE_FOLDERS:
            __traverse_game_folder(src_child_path, dst_child_path, iso_child_path, vanilla_file_hashes)

        elif src_child_path.is_file() and iso_child_path not in IGNORE_FILES:
            if not __is_file_to_include(src_child_path, iso_child_path, vanilla_file_hashes):
                continue

            dst_child_path.parent.mkdir(parents=True, exist_ok=True)

            logging.info("Copying %s ...", iso_child_path)
            shutil.copy(src_child_path, dst_child_path)


def __is_file_to_include(src_file_path: Path, iso_file_path: str, vanilla_file_hashes: dict[str, list[str]]) -> bool:
    if iso_file_path not in vanilla_file_hashes:
        return True

    md5_hash = md5()

    with open(src_file_path, "rb") as f:
        while True:
            data = f.read(4096)
            if not data:
                break
            md5_hash.update(data)

    md5_hexdigest = md5_hash.hexdigest()
    return md5_hexdigest not in vanilla_file_hashes[iso_file_path]


# ----------------------------------------------------------------------------------------------------------------------

def __make_riivolution_xml(game_id: str, game_region: str, error_message_archive_addr: int | None,
                           syati_loader_xml_path: Path, riivolution_xml_path: Path):
    logging.info("Generating Riivolution XML for %s%s", game_id, game_region)

    # Load Syati memory patches
    if not syati_loader_xml_path.is_file():
        logging.error(f'%s was not build, cannot create Riivolution XML!', syati_loader_xml_path.name)
        exit(1)

    with open(syati_loader_xml_path, "r") as f:
        syati_loader_xml_root = ET.fromstring("<root>" + f.read() + "</root>")

    # Create main Riivolution tree
    wiidisc_node = ET.Element("wiidisc", {"version": "1"})

    id_node = ET.SubElement(wiidisc_node, "id", {"game": game_id})
    ET.SubElement(id_node, "region", {"type": game_region})

    # Create options branch
    options_node = ET.SubElement(wiidisc_node, "options")

    section_node = ET.SubElement(options_node, "section", {"name": f"NMG ({game_id}{game_region})"})
    option_node = ET.SubElement(section_node, "option", {"name": "Neo Mario Galaxy"})
    choice_node = ET.SubElement(option_node, "choice", {"name": "Enabled"})
    ET.SubElement(choice_node, "patch", {"id": "nmg_core"})

    # Create patch branch
    patch_node = ET.SubElement(wiidisc_node, "patch", {"id": "nmg_core", "root": "/nmg"})

    ET.SubElement(patch_node, "savegame", {"external": f'save/{game_id}{game_region}', "clone": "false"})

    ET.SubElement(patch_node, "folder", {"disc": "/AudioRes",     "external": "game/AudioRes",     "create": "true"})
    ET.SubElement(patch_node, "folder", {"disc": "/CustomCode",   "external": "game/CustomCode",   "create": "true"})
    ET.SubElement(patch_node, "folder", {"disc": "/LayoutData",   "external": "game/LayoutData",   "create": "true"})
    ET.SubElement(patch_node, "folder", {"disc": "/LocalizeData", "external": "game/LocalizeData", "create": "true"})
    ET.SubElement(patch_node, "folder", {"disc": "/ObjectData",   "external": "game/ObjectData",   "create": "true"})
    ET.SubElement(patch_node, "folder", {"disc": "/ParticleData", "external": "game/ParticleData", "create": "true"})
    ET.SubElement(patch_node, "folder", {"disc": "/StageData",    "external": "game/StageData",    "create": "true"})
    ET.SubElement(patch_node, "folder", {"disc": "/SystemData",   "external": "game/SystemData",   "create": "true"})

    if error_message_archive_addr is not None:
        ET.SubElement(patch_node, "memory", {"offset": f'0x{error_message_archive_addr:08X}', "original": "52415243",
                                             "valuefile": f'memory/ErrorMessageArchive{game_id}{game_region}.arc'})

    for syati_xml_node in iter(syati_loader_xml_root):
        offset = syati_xml_node.attrib["offset"]
        value = syati_xml_node.attrib["value"]

        ET.SubElement(patch_node, "memory", {"offset": offset, "value": value})

    # Write Riivolution XML
    tree = ET.ElementTree(wiidisc_node)
    ET.indent(tree, space="\t", level=0)
    tree.write(riivolution_xml_path, encoding="utf-8")


########################################################################################################################

if __name__ == '__main__':
    pack_zip_archive()
