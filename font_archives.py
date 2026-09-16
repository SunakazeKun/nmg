"""
Font archive packaging

Generates Font archives for all supported languages based on the BRFNT sheets in the Font folder.
"""

from pyjkernel import JKRCompression, create_new_archive, write_archive_file
from config import GAME_PATH, SUPPORTED_LANGUAGES
from pathlib import Path

__all__ = ["pack_font_archives"]


########################################################################################################################

SRC_FONT_PATH = Path("data/Font")

FONT_NAMES = [
    "MenuFont64",
    "MenuFont96",
    "MessageFont26",
    "NumberFont",
    "PictureFont",
    "StaffFont18",
]


def pack_font_archives():
    for language in SUPPORTED_LANGUAGES:
        archive = create_new_archive("Font")

        for font_name in FONT_NAMES:
            brfnt_file = archive.create_file(f'Font/{font_name}.brfnt')
            brfnt_file.data = __read_font_data(language, font_name)

        archive_path = GAME_PATH / f'files/LocalizeData/{language}/LayoutData/Font.arc'
        archive_path.parent.mkdir(parents=True, exist_ok=True)
        write_archive_file(archive, archive_path, big_endian=True, compression=JKRCompression.SZS)


def __read_font_data(language: str, font_name: str) -> bytes:
    for font_path in [
        SRC_FONT_PATH / f'{language}_{font_name}.brfnt',
        SRC_FONT_PATH / f'{language[:2]}_{font_name}.brfnt',
        SRC_FONT_PATH / f'{font_name}.brfnt',
    ]:
        if font_path.exists():
            return font_path.read_bytes()

    raise ValueError(f'cannot resolve {font_name} for {language}!')


########################################################################################################################

if __name__ == '__main__':
    pack_font_archives()
