from pyjkernel import JKRCompression, write_archive_file, create_new_archive
from pyjmap import SuperMarioGalaxyHashTable, from_csv, pack_buffer
from config import GAME_PATH
from pathlib import Path

import re
import shutil
import logging

__all__ = ["pack_all_stage_data", "pack_stage_data"]


########################################################################################################################

DUMMY_STAGE_ARCS = {
    "IslandFleetGalaxy":    ["Design", "Sound"],
    "YosshiHomeGalaxy":     ["Design", "Sound"],
    "DigMineGalaxy":        ["Design", "Sound", "ZoneInfo"],
    "MokumokuValleyGalaxy": ["Design", "Sound"],
    "AbekobeGalaxy":        ["Design", "Sound"],
    "RedBlueExGalaxy":      ["Design"],
    "VsKoopaJrLv1Galaxy":   ["Design", "Sound"],
}

SCENARIO_STEMS = {
    "ScenarioData",
    "ZoneList",
    "GalaxyInfo",
}
ZONE_INFO_STEMS = {
    "InStageFlagNameTable",
    "ShadowControllerParam",
    "WaterCameraFilterParam",
}

SRC_STAGE_DATA_PATH = Path("data/StageData")
HASH_TABLE = SuperMarioGalaxyHashTable()


def pack_all_stage_data():
    for stage_path in filter(lambda p: p.is_dir(), SRC_STAGE_DATA_PATH.iterdir()):
        pack_stage_data(stage_path)


def pack_stage_data(src_stage_path: Path):
    stage_name = src_stage_path.name
    dst_stage_path = GAME_PATH / f'files/StageData/{stage_name}'

    __create_dummy_archives(stage_name, dst_stage_path)
    __create_scenario_archive(stage_name, src_stage_path, dst_stage_path)
    __create_ghost_archive(stage_name, src_stage_path, dst_stage_path)
    __create_light_archive(stage_name, src_stage_path, dst_stage_path)
    __create_demo_archive(stage_name, src_stage_path, dst_stage_path)
    __create_zone_info_archive(stage_name, src_stage_path, dst_stage_path)

    src_map_path = src_stage_path / f'{stage_name}Map.arc'
    dst_map_path = dst_stage_path / f'{stage_name}Map.arc'

    if not src_map_path.is_file():
        return

    logging.info("Copying %s", src_map_path.name)
    shutil.copyfile(src_map_path, dst_map_path)


# ======================================================================================================================

def __create_scenario_archive(stage_name: str, src_stage_path: Path, dst_stage_path: Path):
    arc_path = dst_stage_path / f'{stage_name}Scenario.arc'
    csv_paths = list(filter(lambda p: p.is_file() and p.stem in SCENARIO_STEMS, src_stage_path.iterdir()))

    if len(csv_paths) == 0:
        return

    logging.info("Packing %s", arc_path.name)

    arc = create_new_archive(f'{stage_name}Scenario')

    for csv_path in csv_paths:
        bcsv = arc.create_file(f'{stage_name}Scenario/{csv_path.stem}.bcsv')

        jmap = from_csv(HASH_TABLE, str(csv_path))
        bcsv.data = pack_buffer(jmap)

    write_archive_file(arc, str(arc_path), compression=JKRCompression.SZS)


def __create_ghost_archive(stage_name: str, src_stage_path: Path, dst_stage_path: Path):
    arc_path = dst_stage_path / f'{stage_name}Ghost.arc'
    gst_paths = list(filter(lambda p: p.is_file() and p.suffix == ".gst", src_stage_path.iterdir()))
    pad_paths = list(filter(lambda p: p.is_file() and p.suffix == ".pad", src_stage_path.iterdir()))

    if len(gst_paths) == 0 and len(pad_paths) == 0:
        return

    logging.info("Packing %s", arc_path.name)

    arc = create_empty_ghost_arc()

    for gst_path in gst_paths:
        arc.create_file(f'Stage/gst/{gst_path.name}', gst_path.read_bytes())

    for pad_path in pad_paths:
        arc.create_file(f'Stage/pad/{pad_path.name}', pad_path.read_bytes())

    write_archive_file(arc, str(arc_path), compression=JKRCompression.SZS)


def __create_light_archive(stage_name: str, src_stage_path: Path, dst_stage_path: Path):
    arc_path = dst_stage_path / f'{stage_name}Light.arc'
    csv_path = src_stage_path / "Light.csv"

    if not csv_path.is_file():
        return

    logging.info("Packing %s", arc_path.name)

    arc = create_empty_csv_arc()
    bcsv = arc.create_file(f'Stage/csv/{stage_name}.bcsv')

    jmap = from_csv(HASH_TABLE, str(csv_path))
    bcsv.data = pack_buffer(jmap)

    write_archive_file(arc, str(arc_path), compression=JKRCompression.SZS)


def __create_demo_archive(stage_name: str, src_stage_path: Path, dst_stage_path: Path):
    arc_path = dst_stage_path / f'{stage_name}Demo.arc'

    csv_paths = list(filter(lambda p: p.is_file() and re.match(r'Demo.+\.csv', p.name), src_stage_path.iterdir()))

    if len(csv_paths) == 0:
        return

    logging.info("Packing %s", arc_path.name)

    arc = create_empty_csv_arc()

    for csv_path in csv_paths:
        bcsv = arc.create_file(f'Stage/csv/{csv_path.stem}.bcsv')

        jmap = from_csv(HASH_TABLE, str(csv_path))
        bcsv.data = pack_buffer(jmap)

    write_archive_file(arc, str(arc_path), compression=JKRCompression.SZS)


def __create_zone_info_archive(stage_name: str, src_stage_path: Path, dst_stage_path: Path):
    arc_path = dst_stage_path / f'{stage_name}ZoneInfo.arc'
    csv_paths = list(filter(lambda p: p.is_file() and p.stem in ZONE_INFO_STEMS, src_stage_path.iterdir()))

    if len(csv_paths) == 0:
        return

    logging.info("Packing %s", arc_path.name)

    arc = create_empty_csv_arc()

    for csv_path in csv_paths:
        bcsv = arc.create_file(f'Stage/csv/{csv_path.stem}.bcsv')

        jmap = from_csv(HASH_TABLE, str(csv_path))
        bcsv.data = pack_buffer(jmap)

    write_archive_file(arc, str(arc_path), compression=JKRCompression.SZS)


# ======================================================================================================================

def create_empty_jmp_arc():
    arc = create_new_archive("Stage")
    arc.create_folder("Stage/jmp")
    return arc


def create_empty_ghost_arc():
    arc = create_new_archive("Stage")
    arc.create_folder("Stage/gst")
    arc.create_folder("Stage/pad")
    return arc


def create_empty_csv_arc():
    arc = create_new_archive("Stage")
    arc.create_folder("Stage/csv")
    return arc


# ======================================================================================================================

__DUMMY_ARCHIVE_CREATORS = [
    ("Design",   create_empty_jmp_arc),
    ("Sound",    create_empty_jmp_arc),
    ("Ghost",    create_empty_ghost_arc),
    ("Light",    create_empty_csv_arc),
    ("Demo",     create_empty_csv_arc),
    ("ZoneInfo", create_empty_csv_arc),
]


def __create_dummy_archives(stage_name: str, dst_stage_path: Path):
    if stage_name not in DUMMY_STAGE_ARCS:
        return

    for arc_name, arc_creator in __DUMMY_ARCHIVE_CREATORS:
        if arc_name not in DUMMY_STAGE_ARCS[stage_name]:
            continue

        arc_path = dst_stage_path / f'{stage_name}{arc_name}.arc'

        logging.info("Creating dummy for %s", arc_path.name)

        arc = arc_creator()
        write_archive_file(arc, str(arc_path), compression=JKRCompression.SZS)


########################################################################################################################

if __name__ == '__main__':
    pack_all_stage_data()
