from config import SUPPORTED_LANGUAGES
from struct import Struct, unpack_from
from tools.dataio import read_json
from pathlib import Path

import re


########################################################################################################################

IGNORE_ARCHIVES = [
    "TestArchive"
]
IGNORE_BANKS = [
    ("LayoutMessage", "Layout_ResultWindow"),
    ("LayoutMessage", "Layout_TitleLogo"),
    ("SystemMessage", "Banner"),
    ("SystemMessage", "WiiMessage"),
]
IGNORE_LABELS = [
    # Example: (archive, bank, label)
]

MESSAGE_DATA_PATH = Path("data/MessageData")
FONT_PATH = Path("data/Font")
FONT_NAME = "MessageFont26"
TAG_PATTERNS = [
    re.compile(r'\[defcolor.*?]'),
    re.compile(r'\[color.*?]'),
    re.compile(r'\[delay.*?]'),
    re.compile(r'\[pagebreak.*?]'),
    re.compile(r'\[ycenter.*?]'),
    re.compile(r'\[xcenter.*?]'),
    re.compile(r'\[sound.*?]'),
    re.compile(r'\[icon.*?]'),
    re.compile(r'\[size.*?]'),
    re.compile(r'\[player.*?]'),
    re.compile(r'\[intvar.*?]'),
    re.compile(r'\[stringvar.*?]'),
    re.compile(r'\[race.*?]'),
    re.compile(r'\[numberfont.*?]'),
]


def print_missing_characters(language: str):
    used_chars = collect_used_characters(language)
    valid_chars = collect_valid_characters(language, FONT_NAME)
    missing_chars = used_chars - valid_chars

    print(f'* {language}')

    if not missing_chars:
        print("No missing characters found.")
    else:
        for char in sorted(missing_chars, key=ord):
            print(f'{ord(char):04x} {char}')

    print()


def collect_used_characters(language: str) -> set[str]:
    used_chars = {'\n'}

    for folder_path in filter(lambda p: p.is_dir(), MESSAGE_DATA_PATH.iterdir()):
        archive_name = folder_path.name

        if archive_name in IGNORE_ARCHIVES:
            continue

        for json_path in filter(lambda p: p.is_file() and p.name.endswith(".json"), folder_path.iterdir()):
            bank_name = json_path.stem

            if (archive_name, bank_name) in IGNORE_BANKS:
                continue

            json_data = read_json(json_path)

            for message in json_data:
                label = message["label"]

                if (archive_name, bank_name, label) in IGNORE_LABELS:
                    continue

                lines = message["languages"].get(language)

                if lines is None:
                    lines = message["languages"].get("default")
                if lines is None:
                    continue

                text = '\n'.join(lines)
                for tag_pattern in TAG_PATTERNS:
                    text = tag_pattern.sub("", text)
                used_chars |= set(text)

    return used_chars


def collect_valid_characters(language: str, font_name: str) -> set[str]:
    for font_path in [
        FONT_PATH / f'{language}_{font_name}.brfnt',
        FONT_PATH / f'{language[:2]}_{font_name}.brfnt',
        FONT_PATH / f'{font_name}.brfnt',
    ]:
        if font_path.exists():
            return extract_valid_char_codes_from_brfnt(font_path)

    return {'\n'}


########################################################################################################################

_STRUCT_RFNT_HEADER = Struct(">4sH2BI2H")
_STRUCT_FINF_INFO = Struct(">2BH4B3I4B")
_STRUCT_TGLP_INFO = Struct(">4BI6HI")
_STRUCT_CWDH_INFO = Struct(">2HI")
_STRUCT_CMAP_INFO = Struct(">4HIH")
_STRUCT_SECTION_HEADER = Struct(">4sI")


def extract_valid_char_codes_from_brfnt(brfnt_path: Path) -> set[str]:
    binary = brfnt_path.read_bytes()
    valid_char_codes = {'\n'}

    file_magic, file_bom, version_major, version_minor, file_size, off_sections, num_sections \
        = _STRUCT_RFNT_HEADER.unpack_from(binary, 0)

    if file_magic != b'RFNT':
        raise Exception("unknown magic")
    if file_bom != 0xFEFF:
        raise Exception("unsupported byte order")
    if version_major != 1 and version_minor != 4:
        raise Exception("unsupported version")

    off_section = off_sections
    for _ in range(num_sections):
        section_magic, section_size = _STRUCT_SECTION_HEADER.unpack_from(binary, off_section)

        if section_magic == b'FINF':
            _, _, _, _, _, _, encoding, _, _, _, _, _, _, _ \
                = _STRUCT_FINF_INFO.unpack_from(binary, off_section + 0x08)

            if encoding != 1:
                raise Exception("only supports UTF-16 encoding")

        elif section_magic == b'TGLP':
            pass

        elif section_magic == b'CWDH':
            pass

        elif section_magic == b'CMAP':
            first_code, last_code, mapping, _, _, _ \
                = _STRUCT_CMAP_INFO.unpack_from(binary, off_section + 0x08)
            off_mapping_data = off_section + 0x14

            if mapping == 0:
                for code in range(first_code, last_code + 1):
                    valid_char_codes.add(chr(code))

            if mapping == 1:
                for i, code in enumerate(range(first_code, last_code + 1)):
                    glyph = unpack_from(">H", binary, off_mapping_data + i * 0x02)[0]

                    if glyph != 0xFFFF:
                        valid_char_codes.add(chr(code))

            if mapping == 2:
                num_scans = unpack_from(">H", binary, off_mapping_data + 0x00)[0]

                for i in range(num_scans):
                    code, _ = unpack_from(">2H", binary, off_mapping_data + 0x02 + i * 0x04)
                    valid_char_codes.add(chr(code))

        off_section += section_size

    return valid_char_codes


########################################################################################################################

def __main():
    for language in SUPPORTED_LANGUAGES:
        print_missing_characters(language)


if __name__ == '__main__':
    __main()
