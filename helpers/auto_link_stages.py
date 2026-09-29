from pyjkernel import from_archive_file, write_archive_file, JKRArchiveFile, JKRCompression
from pyjmap import from_buffer, pack_buffer, JMapInfo, JMapEntry, SuperMarioGalaxyHashTable

from config import GAME_PATH


########################################################################################################################

RELINK_GALAXIES = [
    "IslandFleetGalaxy",
    "YosshiHomeGalaxy",
    "DigMineGalaxy",
    "MokumokuValleyGalaxy",
    "AbekobeGalaxy",
    "RedBlueExGalaxy",
    "VsKoopaJrLv1Galaxy",
    "AstroGalaxy",
    "PeachCastleGalaxy",
    "TestGalaxy"
]
RELINK_ZONES = [
    "NeoBonusAttackZone",
]


def auto_link_stages():
    for galaxy in RELINK_GALAXIES:
        auto_link_galaxy(galaxy)

    for zone in RELINK_ZONES:
        auto_link_zone(zone)


########################################################################################################################

HASH_TABLE = SuperMarioGalaxyHashTable()

STAGE_FILE_INFOS = [
    ("Placement", "ObjInfo"),
    ("MapParts", "MapPartsInfo"),
    ("Placement", "AreaObjInfo"),
    ("Placement", "CameraCubeInfo"),
    ("Placement", "PlanetObjInfo"),
    ("Placement", "StageObjInfo"),
    ("GeneralPos", "GeneralPosInfo"),
]


def auto_link_galaxy(galaxy: str):
    scenario_arc_path = str(GAME_PATH / f'files/StageData/{galaxy}/{galaxy}Scenario.arc')
    scenario_arc = from_archive_file(scenario_arc_path)

    zone_list_bcsv = from_buffer(HASH_TABLE, scenario_arc.get_file(f"{galaxy}Scenario/ZoneList.bcsv").data, 0)
    zone_names = [e["ZoneName"] for e in zone_list_bcsv]

    for zone in filter(lambda z: z not in RELINK_ZONES, zone_names):
        auto_link_zone(zone)


def auto_link_zone(zone: str):
    map_arc_path = str(GAME_PATH / f'files/StageData/{zone}/{zone}Map.arc')
    map_arc = from_archive_file(map_arc_path)

    layer_names = map_arc.list_folders("Stage/jmp/Placement")
    all_bcsvs: dict[str, tuple[JMapInfo, JKRArchiveFile]] = {}

    obj_id_references: list[JMapEntry] = []
    map_parts_id_references: list[JMapEntry] = []
    generator_id_references: list[JMapEntry] = []
    follow_id_references: list[JMapEntry] = []

    # Track all references and sort entries alphabetically
    for folder_name, file_name in STAGE_FILE_INFOS:
        for layer_name in layer_names:
            jmp_path = format_jmp_path(folder_name, layer_name, file_name)
            bcsv = map_arc.get_file(jmp_path)
            jmap = from_buffer(HASH_TABLE, bcsv.data, 0)

            all_bcsvs[jmp_path] = (jmap, bcsv)

            jmap.sort_entries(lambda e: e["name"])

            for entry in jmap:
                try_track_reference(obj_id_references, entry, "Obj_ID")
                try_track_reference(map_parts_id_references, entry, "MapParts_ID")
                try_track_reference(generator_id_references, entry, "GeneratorID")
                try_track_reference(follow_id_references, entry, "FollowId")

    if len(follow_id_references):
        print("WARNING, " + zone + " has FollowId stuff, skipping...")
        return

    # Update link IDs and relink referencing objects
    for folder_name, file_name in STAGE_FILE_INFOS:
        # GeneralPosInfo does not have link IDs
        if file_name == "GeneralPosInfo":
            continue

        link_id = 0

        for layer_name in layer_names:
            jmp_path = format_jmp_path(folder_name, layer_name, file_name)
            jmap, bcsv = all_bcsvs[jmp_path]

            for entry in jmap:
                old_link_id = entry["l_id"]
                new_link_id = link_id
                entry["l_id"] = new_link_id

                link_id += 1

                match file_name:
                    case "ObjInfo":
                        relink_reference(generator_id_references, "GeneratorID", old_link_id, new_link_id)
                        relink_reference(obj_id_references, "Obj_ID", old_link_id, new_link_id)

                    case "MapPartsInfo":
                        relink_reference(map_parts_id_references, "MapParts_ID", old_link_id, new_link_id)

            bcsv.data = pack_buffer(jmap)

    write_archive_file(map_arc, map_arc_path, compression=JKRCompression.SZS)


# ======================================================================================================================

def format_jmp_path(folder: str, layer: str, file: str) -> str:
    return f'Stage/jmp/{folder}/{layer}/{file}'


def try_track_reference(references: list[JMapEntry], entry: JMapEntry, reference_key: str):
    if reference_key in entry and entry[reference_key] != -1:
        references.append(entry)


def relink_reference(references: list[JMapEntry], reference_key: str, old_link_id: int, new_link_id: int):
    remove_entries = []

    for entry in references:
        if entry[reference_key] == old_link_id:
            entry[reference_key] = new_link_id
            remove_entries.append(entry)

    for entry in remove_entries:
        references.remove(entry)


########################################################################################################################

if __name__ == '__main__':
    auto_link_stages()
