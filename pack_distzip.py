"""
Distributable ZIP packaging

Packs the ZIP file to be distributed containing only the new and changed files as well as the Riivolution configuration
necessary for loading and playing Neo Mario Galaxy. Edited and new files are efficiently identified by comparing the
active game files against known file names and hashes from the unmodified SMG2 games.
"""

from config import PROJECT_TITLE, PROJECT_VERSION, IGNORE_FILES, IGNORE_FOLDERS, GAME_PATH
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

RIIVOLUTION_CONFIG_PATH = Path("data/nmg.xml")
VANILLA_FILE_HASHES_PATH = Path("data/vanilla_file_hashes.json")


def pack_zip_archive(game_path: Path, dist_path: Path):
    if dist_path.exists():
        shutil.rmtree(dist_path)
    dist_path.mkdir(parents=True, exist_ok=True)

    nmg_path = dist_path / "nmg"
    nmg_path.mkdir(parents=True)
    riivolution_path = dist_path / "riivolution"
    riivolution_path.mkdir(parents=True)

    # ------------------------------------------------------

    logging.info("Collecting dist files ...")

    vanilla_file_hashes = read_json(VANILLA_FILE_HASHES_PATH)

    shutil.copy(RIIVOLUTION_CONFIG_PATH, riivolution_path / "nmg.xml")
    __traverse_game_folder(game_path / "files", nmg_path, "", vanilla_file_hashes)

    # ------------------------------------------------------

    logging.info("Packaging dist archive ...")

    build_time = datetime.now()
    utime = time.mktime(build_time.timetuple())

    with open(dist_path / "build-info.txt", "w") as f:
        f.write(f'Project-Name: {PROJECT_TITLE}\n'
                f'Project-Version: v{PROJECT_VERSION}\n'
                f'Build-Date: {build_time.strftime("%Y-%m-%d %H:%M:%S")}\n'
                f'Host: {getpass.getuser()}\n')

    for root, folders, files in os.walk(dist_path):
        for child in folders + files:
            file_path = os.path.join(root, child)
            os.utime(file_path, (utime, utime))

    shutil.make_archive(f'{PROJECT_TITLE}_v{PROJECT_VERSION}', "zip", dist_path)


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


########################################################################################################################

if __name__ == '__main__':
    pack_zip_archive(GAME_PATH, Path("dist"))
