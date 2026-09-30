from pathlib import Path
from config import GAME_PATH, SUPPORTED_LANGUAGES

import shutil
import logging

__all__ = ["copy_static_files_to_game", "copy_simple_tree_to_game", "copy_localized_tree_to_game"]


########################################################################################################################

def copy_static_files_to_game():
    copy_localized_tree_to_game(Path("SystemData"), Path("data/arcs/SystemData"))
    copy_localized_tree_to_game(Path("LayoutData"), Path("data/arcs/LayoutData"))
    copy_localized_tree_to_game(Path("ObjectData"), Path("data/arcs/ObjectData/SMG1"))
    copy_localized_tree_to_game(Path("ObjectData"), Path("data/arcs/ObjectData/Edited"))
    copy_localized_tree_to_game(Path("ObjectData"), Path("data/arcs/ObjectData/Custom"))


def copy_simple_tree_to_game(iso_folder_subpath: Path, src_folder_path: Path):
    logging.info("Copying into game %s from %s", iso_folder_subpath, src_folder_path)

    dst_folder_path = GAME_PATH / "files" / iso_folder_subpath
    shutil.copytree(src_folder_path, dst_folder_path, dirs_exist_ok=True)


def copy_localized_tree_to_game(iso_folder_subpath: Path, src_folder_path: Path):
    logging.info("Copying into game %s from %s", iso_folder_subpath, src_folder_path)

    for src_file_path in filter(lambda p: p.is_file(), src_folder_path.iterdir()):
        dst_file_path = GAME_PATH / "files" / iso_folder_subpath / src_file_path.name
        shutil.copy(src_file_path, dst_file_path)

    for language in SUPPORTED_LANGUAGES:
        src_lang_folder_path = src_folder_path / language

        if not src_lang_folder_path.is_dir():
            continue

        for src_file_path in filter(lambda p: p.is_file(), src_lang_folder_path.iterdir()):
            dst_file_path = GAME_PATH / "files" / "LocalizeData" / language / iso_folder_subpath / src_file_path.name
            shutil.copy(src_file_path, dst_file_path)


########################################################################################################################

if __name__ == '__main__':
    copy_static_files_to_game()
