import shutil
from pathlib import Path
from config import GAME_PATH, SUPPORTED_LANGUAGES

import logging

__all__ = ["copy_simple_tree_to_game"]


########################################################################################################################

def copy_simple_tree_to_game(iso_folder_subpath: Path, src_folder_path: Path):
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

def __main():
    copy_simple_tree_to_game(Path("LayoutData"), Path("data/LayoutData"))
    copy_simple_tree_to_game(Path("ObjectData"), Path("data/ObjectData"))


if __name__ == '__main__':
    __main()
