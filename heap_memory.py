from pyjkernel import from_archive_file, write_archive_file, JKRCompression, JKRArchive
from pyjmap import SuperMarioGalaxyHashTable, from_buffer, pack_buffer, from_csv

from config import GAME_PATH
from tools.dataio import read_lines
from pathlib import Path

import re


########################################################################################################################

SYSTEM_DATA_TABLE_PATH = GAME_PATH / "files/ObjectData/SystemDataTable.arc"

HEAP_SIZE_EXCEPT_PATH = Path("data/HeapMemory/HeapSizeExcept.csv")
WORLD_MAP_HEAP_GALAXY_PATH = Path("data/HeapMemory/WorldMapHeapGalaxy.csv")
WORLD_MAP_HEAP_RESOURCE_APPEND_PATH = Path("data/HeapMemory/WorldMapHeapResource_append.txt")
MARIO_FACE_SHIP_GALAXY_RESOURCE_PATH = Path("data/UseResourceLogs/MarioFaceShipGalaxy_Scenario2.txt")

ARCHIVE_PATTERN = re.compile(r'^.*ARCHIVE\s+')
HASH_TABLE = SuperMarioGalaxyHashTable()


def update_heap_memory_bcsv_files():
    archive = from_archive_file(str(SYSTEM_DATA_TABLE_PATH))

    update_world_map_heap_resource_bcsv(archive)
    update_world_map_heap_galaxy_bcsv(archive)
    update_heap_size_except_bcsv(archive)

    write_archive_file(archive, "game/files/ObjectData/SystemDataTable.arc",
                       big_endian=True, compression=JKRCompression.SZS)


# ----------------------------------------------------------------------------------------------------------------------

def update_world_map_heap_resource_bcsv(archive: JKRArchive):
    bcsv = archive.get_file("SystemDataTable/WorldMapHeapResource.bcsv")

    jmap_data = from_buffer(SuperMarioGalaxyHashTable(), bcsv.data, 0)
    jmap_data.clear_entries()

    for raw_line in read_lines(MARIO_FACE_SHIP_GALAXY_RESOURCE_PATH):
        if ARCHIVE_PATTERN.match(raw_line):
            resource = ARCHIVE_PATTERN.sub("", raw_line).strip()
            jmap_data.create_entry()["ResourceName"] = resource

    for resource in read_lines(WORLD_MAP_HEAP_RESOURCE_APPEND_PATH):
        jmap_data.create_entry()["ResourceName"] = resource

    bcsv.data = pack_buffer(jmap_data)


def update_world_map_heap_galaxy_bcsv(archive: JKRArchive):
    bcsv = archive.get_file("SystemDataTable/WorldMapHeapGalaxy.bcsv")

    jmap_data = from_csv(HASH_TABLE, str(WORLD_MAP_HEAP_GALAXY_PATH))
    bcsv.data = pack_buffer(jmap_data)


def update_heap_size_except_bcsv(archive: JKRArchive):
    bcsv = archive.get_file("SystemDataTable/HeapSizeExcept.bcsv")

    jmap_data = from_csv(HASH_TABLE, str(HEAP_SIZE_EXCEPT_PATH))
    bcsv.data = pack_buffer(jmap_data)


########################################################################################################################

if __name__ == '__main__':
    update_heap_memory_bcsv_files()
