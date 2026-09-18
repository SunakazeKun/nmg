from pyjkernel import JKRCompression, create_new_archive, write_archive_file
from pyjmap import SuperMarioGalaxyHashTable, JMapInfo, JMapFieldType, pack_buffer, from_csv
from config import GAME_PATH, SUPPORTED_LANGUAGES
from tools.dataio import read_bin, read_lines
from pathlib import Path

import logging
import re

__all__ = ["pack_other_archives"]


########################################################################################################################

SRC_AUDIO_RES_PATH     = Path("data/AudioRes")
SRC_FONT_PATH          = Path("data/Font")
SRC_OBJECT_DATA_PATH   = Path("data/ObjectData")
SRC_PARTICLE_DATA_PATH = Path("data/ParticleData")
SRC_SYSTEM_DATA_PATH   = Path("data/SystemData")

WORLD_MAP_HEAP_RESOURCE_APPEND_PATH = Path("data/ObjectData/WorldMapHeapResource_append.txt")
MARIO_FACE_SHIP_GALAXY_RESOURCE_PATH = Path("data/UseResourceLogs/MarioFaceShipGalaxy_Scenario2.txt")

BcsvFileInfo = tuple[Path, str, str]
SimpleBcsvArcInfo = tuple[str, str, list[BcsvFileInfo]]

HASH_TABLE = SuperMarioGalaxyHashTable()
SIMPLE_BCSV_ARC_INFOS: list[SimpleBcsvArcInfo] = [
    ("AudioRes/Info/ActionSound.arc", "ActionSound", [
        (SRC_AUDIO_RES_PATH, "ActionSound", ".bcsv"),
    ]),
    ("AudioRes/Info/BgmParam.arc", "BgmParam", [
        (SRC_AUDIO_RES_PATH, "BgmParam", ".bcsv"),
    ]),
    ("AudioRes/Info/MultiBgmInfo.arc", "MultiBgmInfo", [
        (SRC_AUDIO_RES_PATH, "MultiBgmInfo", ".bcsv"),
    ]),
    ("AudioRes/Info/StageBgmInfo.arc", "StageBgmInfo", [
        (SRC_AUDIO_RES_PATH, "ScenarioBgmInfo", ".bcsv"),
        (SRC_AUDIO_RES_PATH, "StageBgmInfo", ".bcsv"),
    ]),
    ("ObjectData/PlanetMapDataTable.arc", "PlanetMapDataTable", [
        (SRC_OBJECT_DATA_PATH, "PlanetMapDataTable", ".bcsv"),
    ]),
    ("ObjectData/ProductMapObjDataTable.arc", "ProductMapObjDataTable", [
        (SRC_OBJECT_DATA_PATH, "ProductMapObjDataTable", ".bcsv"),
    ]),
    ("SystemData/ObjNameTable.arc", "ObjNameTable", [
        (SRC_SYSTEM_DATA_PATH, "ObjNameTable", ".tbl"),
    ]),
]


def pack_other_archives():
    for arc_subpath, root_name, bcsv_file_infos in SIMPLE_BCSV_ARC_INFOS:
        arc_path = GAME_PATH / "files" / arc_subpath
        __pack_bcsv_arc(arc_path, root_name, bcsv_file_infos)

    pack_system_data_table_arc()
    pack_effect_arc()
    pack_font_arc()


# ======================================================================================================================
# SystemDataTable.arc
# ======================================================================================================================

def pack_system_data_table_arc():
    csv_file_stems = [
        "GalaxyDataTable",
        "GalaxyWorldOrderList",
        "HeapSizeExcept",
        "MarioFaceShipEventCastTable",
        "MarioFaceShipEventDataTable",
        "WorldMapHeapGalaxy",
    ]

    arc_path = GAME_PATH / "files/ObjectData/SystemDataTable.arc"
    logging.info("Packing %s", arc_path.name)

    archive = create_new_archive("SystemDataTable", sync_file_ids=True)

    for csv_file_stem in csv_file_stems:
        archive.create_file(
            f'{archive.root_name}/{csv_file_stem}.bcsv',
            data=__read_pack_csv(SRC_OBJECT_DATA_PATH / f'{csv_file_stem}.csv'),
        )

    archive.create_file(
        f'{archive.root_name}/WorldMapHeapResource.bcsv',
        data=__pack_world_map_heap_resource_bcsv(),
    )

    write_archive_file(archive, str(arc_path), big_endian=True, compression=JKRCompression.SZS)


def __pack_world_map_heap_resource_bcsv() -> bytes:
    arc_pattern = re.compile(r'^.*ARCHIVE\s+')

    jmap_data = JMapInfo(HASH_TABLE)
    jmap_data.create_field("ResourceName", JMapFieldType.STRING_OFFSET, "")

    for raw_line in read_lines(MARIO_FACE_SHIP_GALAXY_RESOURCE_PATH):
        if arc_pattern.match(raw_line):
            resource = arc_pattern.sub("", raw_line).strip()
            jmap_data.create_entry()["ResourceName"] = resource

    for resource in read_lines(WORLD_MAP_HEAP_RESOURCE_APPEND_PATH):
        jmap_data.create_entry()["ResourceName"] = resource

    return pack_buffer(jmap_data)


# ======================================================================================================================
# Effect.arc
# ======================================================================================================================

def pack_effect_arc():
    csv_file_stems = [
        "AutoEffectList",
        "ParticleNames",
    ]

    arc_path = GAME_PATH / "files/ParticleData/Effect.arc"
    logging.info("Packing %s", arc_path.name)

    archive = create_new_archive("Effect", sync_file_ids=True)

    for csv_file_stem in csv_file_stems:
        archive.create_file(
            f'{archive.root_name}/{csv_file_stem}.bcsv',
            data=__read_pack_csv(SRC_PARTICLE_DATA_PATH / f'{csv_file_stem}.csv'),
        )

    archive.create_file(
        f'{archive.root_name}/Particles.jpc',
        data=read_bin(SRC_PARTICLE_DATA_PATH / "Particles.jpc"),
    )

    write_archive_file(archive, str(arc_path), big_endian=True, compression=JKRCompression.SZS)


# ======================================================================================================================
# Font.arc
# ======================================================================================================================

def pack_font_arc():
    brfnt_stems = [
        "MenuFont64",
        "MenuFont96",
        "MessageFont26",
        "NumberFont",
        "PictureFont",
        "StaffFont18",
    ]

    for language in SUPPORTED_LANGUAGES:
        arc_path = GAME_PATH / f'files/LocalizeData/{language}/LayoutData/Font.arc'
        logging.info("Packing %s for %s", arc_path.name, language)

        archive = create_new_archive("Font")

        for brfnt_stem in brfnt_stems:
            archive.create_file(
                f'{archive.root_name}/{brfnt_stem}.brfnt',
                data=__read_font_data(language, brfnt_stem),
            )

        arc_path.parent.mkdir(parents=True, exist_ok=True)
        write_archive_file(archive, arc_path, big_endian=True, compression=JKRCompression.SZS)


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

def __pack_bcsv_arc(arc_path: Path, root_name: str, bcsv_file_infos: list[BcsvFileInfo]):
    logging.info("Packing %s", arc_path.name)

    archive = create_new_archive(root_name)

    for csv_folder_path, csv_file_stem, csv_file_suffix in bcsv_file_infos:
        archive.create_file(
            f'{root_name}/{csv_file_stem}{csv_file_suffix}',
            data=__read_pack_csv(csv_folder_path / f'{csv_file_stem}.csv')
        )

    write_archive_file(archive, str(arc_path), big_endian=True, compression=JKRCompression.SZS)


def __read_pack_csv(csv_path: Path) -> bytes:
    jmap_data = from_csv(HASH_TABLE, str(csv_path))
    return pack_buffer(jmap_data, big_endian=True, encoding="shift_jisx0213")


########################################################################################################################

if __name__ == '__main__':
    pack_other_archives()
