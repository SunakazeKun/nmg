"""
MessageData archive generator

Generates MessageData archives for all supported languages based on the folders and JSON files in the MessageData
folder. For some archives, the original archives are used as a base to add in new files.
"""

from pymsb import LMSFlows, LMSEntryNode, LMSMessageNode, LMSBranchNode, LMSEventNode, LMSDocument, LMSFlowNode
from pyjkernel import (JKRArchive, JKRArchiveFile, JKRCompression, create_new_archive, from_archive_file,
                       write_archive_file)
from msb_adapter import NeoMarioGalaxyAdapter, TalkType, BalloonType, SoundID, CameraType
from config import GAME_PATH, ORG_LOCALIZE_DATA_PATH, SUPPORTED_LANGUAGES, PARTIAL_REPLACEMENTS
from tools.dataio import read_json, read_bin
from pathlib import Path
from typing import Callable

import logging

__all__ = ["build_message_data"]


# ======================================================================================================================
# Archive generation
# ======================================================================================================================

SRC_MESSAGE_DATA_PATH = Path("data/MessageData")


def build_message_data():
    for language in SUPPORTED_LANGUAGES:
        for folder_path in filter(lambda p: p.is_dir(), SRC_MESSAGE_DATA_PATH.iterdir()):
            archive_name = folder_path.name
            archive_subpath = f'{language}/MessageData/{archive_name}.arc'

            if archive_name in PARTIAL_REPLACEMENTS:
                archive = from_archive_file(str(ORG_LOCALIZE_DATA_PATH / archive_subpath))
            else:
                archive = create_new_archive(archive_name)

            for json_path in filter(lambda p: p.is_file() and p.name.endswith(".json"), folder_path.iterdir()):
                msb_name = json_path.name.removesuffix(".json")
                msbf_path = folder_path / (msb_name + ".msbf")

                __pack_msb_into_archive(archive, language, archive_name, msb_name,
                                        json_path, msbf_path)

            archive_path = GAME_PATH / "files/LocalizeData" / archive_subpath
            archive_path.parent.mkdir(parents=True, exist_ok=True)
            write_archive_file(archive, str(archive_path), big_endian=True, compression=JKRCompression.NONE)


def __pack_msb_into_archive(archive: JKRArchive, language: str, archive_name: str, msb_name: str,
                            json_path: Path, msbf_path: Path):
    document = LMSDocument(NeoMarioGalaxyAdapter)
    msbt_labels = []

    # ------------------------------------------------------

    for json_msg in read_json(json_path):
        message = document.new_message(json_msg["label"])

        message.attributes["talk_type"]    = TalkType[json_msg["attributes"]["talk_type"]]
        message.attributes["balloon_type"] = BalloonType[json_msg["attributes"]["balloon_type"]]
        message.attributes["sound_id"]     = SoundID[json_msg["attributes"]["sound_id"]]
        message.attributes["camera_type"]  = CameraType[json_msg["attributes"]["camera_type"]]
        message.attributes["camera_id"]    = json_msg["attributes"]["camera_id"]
        message.attributes["msg_link_id"]  = json_msg["attributes"]["msg_link_id"]
        message.attributes["unk7"]         = json_msg["attributes"]["unk7"]
        message.attributes["comment"]      = json_msg["attributes"]["comment"]

        text_lines = json_msg["languages"][language]

        if text_lines is None:
            message.text = f'!MISSING! {message.label}'
            logging.warning("Missing text: %s/%s/%s/%s",
                            language, archive_name, msb_name, message.label)
        else:
            message.text = "\n".join(text_lines)

        msbt_labels.append(message.label)

    msbt_file = __create_or_get_msb_file(archive, msb_name + ".msbt")
    msbt_file.data = document.makebin()

    # ------------------------------------------------------

    if msbf_path.is_file():
        msbf_data = read_bin(msbf_path)
    elif (archive_name, msb_name) in __FLOWCHART_GENERATORS:
        generate_flowcharts = __FLOWCHART_GENERATORS[(archive_name, msb_name)]

        flows = LMSFlows(NeoMarioGalaxyAdapter)
        generate_flowcharts(flows)

        msbf_data = __link_and_pack_flows(msbt_labels, flows)
    else:
        return

    msbf_file = __create_or_get_msb_file(archive, msb_name + ".msbf")
    msbf_file.data = msbf_data


def __create_or_get_msb_file(archive: JKRArchive, file_name: str) -> JKRArchiveFile:
    file_path = f'{archive.root_name}/{file_name}'

    if archive.directory_exists(file_path):
        return archive.get_file(file_path)
    else:
        return archive.create_file(file_path)


def __link_and_pack_flows(msbt_labels: list[str], flows: LMSFlows) -> bytes:
    scanned_nodes = set()

    for flowchart in flows.flowcharts:
        remaining_nodes: list[LMSFlowNode] = [flowchart]

        while len(remaining_nodes):
            node = remaining_nodes.pop(0)
            next_node = node.next_node

            scanned_nodes.add(node)

            if next_node is not None and next_node not in scanned_nodes:
                remaining_nodes.append(node.next_node)

            if isinstance(node, LMSBranchNode):
                next_node = node.next_node_else

                if next_node is not None and next_node not in scanned_nodes:
                    remaining_nodes.append(node.next_node_else)

            if isinstance(node, LMSMessageNode):
                node.arg2 = msbt_labels.index(node.message_label)

    return flows.makebin()


# ======================================================================================================================
# Flowchart generators
# ======================================================================================================================

__FLOWCHART_GENERATORS: dict[tuple[str, str], Callable[[LMSFlows], None]] = {}


def register_flows(archive_name: str, msb_name: str):
    def inner(generator_func):
        __FLOWCHART_GENERATORS[(archive_name, msb_name)] = generator_func
        return generator_func
    return inner


@register_flows("AstroGalaxy", "AstroGalaxy")
def flows_AstroGalaxy(flows: LMSFlows):
    # Rosetta000
    entry: LMSEntryNode = flows.new_flowchart("Rosetta000")

    message_rosetta000_flow000 = LMSMessageNode()
    message_rosetta000_flow000.message_label = "Rosetta000_Flow000"

    event_on_switch_a = LMSEventNode()
    event_on_switch_a.event_type = 5
    event_on_switch_a.event_param = 0

    message_rosetta000_flow001 = LMSMessageNode()
    message_rosetta000_flow001.message_label = "Rosetta000_Flow001"

    entry.next_node = message_rosetta000_flow000
    message_rosetta000_flow000.next_node = event_on_switch_a
    event_on_switch_a.next_node = message_rosetta000_flow001
    message_rosetta000_flow001.next_node = message_rosetta000_flow001

    # ------------------------------------------------------

    # Tico001
    entry: LMSEntryNode = flows.new_flowchart("Tico001")

    branch_power_star_0 = LMSBranchNode()
    branch_power_star_0.arg2 = 8

    message_tico001_flow000 = LMSMessageNode()
    message_tico001_flow000.message_label = "Tico001_Flow000"

    branch_power_star_1 = LMSBranchNode()
    branch_power_star_1.arg2 = 8

    message_tico001_flow001 = LMSMessageNode()
    message_tico001_flow001.message_label = "Tico001_Flow001"

    branch_power_star_2 = LMSBranchNode()
    branch_power_star_2.arg2 = 8

    message_tico001_flow002 = LMSMessageNode()
    message_tico001_flow002.message_label = "Tico001_Flow002"

    entry.next_node = branch_power_star_0

    branch_power_star_0.next_node = message_tico001_flow002
    branch_power_star_0.next_node_else = message_tico001_flow000
    message_tico001_flow000.next_node = branch_power_star_1

    branch_power_star_1.next_node = message_tico001_flow002
    branch_power_star_1.next_node_else = message_tico001_flow001
    message_tico001_flow001.next_node = branch_power_star_2

    branch_power_star_2.next_node = message_tico001_flow002
    branch_power_star_2.next_node_else = message_tico001_flow001
    message_tico001_flow002.next_node = message_tico001_flow002

    # ------------------------------------------------------

    # TicoManiac000
    entry: LMSEntryNode = flows.new_flowchart("TicoManiac000")

    message_tico_maniac000_flow000 = LMSMessageNode()
    message_tico_maniac000_flow000.message_label = "TicoManiac000_Flow000"

    event_kill_func = LMSEventNode()
    event_kill_func.event_type = 7
    event_kill_func.event_param = 0

    entry.next_node = message_tico_maniac000_flow000
    message_tico_maniac000_flow000.next_node = event_kill_func
    event_kill_func.next_node = None


@register_flows("AstroLibraryZone", "AstroLibraryZone")
def flows_AstroLibraryZone(flows: LMSFlows):
    # LibraryBook000
    entry: LMSEntryNode = flows.new_flowchart("LibraryBook000")

    message_librarybook000_flow000 = LMSMessageNode()
    message_librarybook000_flow000.message_label = "LibraryBook000_Flow000"

    branch_yes_no = LMSBranchNode()
    branch_yes_no.arg2 = 0
    branch_yes_no.arg3 = 3

    message_librarybook000_flow001 = LMSMessageNode()
    message_librarybook000_flow001.message_label = "LibraryBook000_Flow001"

    message_librarybook000_flow002 = LMSMessageNode()
    message_librarybook000_flow002.message_label = "LibraryBook000_Flow002"

    entry.next_node = message_librarybook000_flow000
    message_librarybook000_flow000.next_node = branch_yes_no
    branch_yes_no.next_node = message_librarybook000_flow001
    branch_yes_no.next_node_else = message_librarybook000_flow002
    message_librarybook000_flow001.next_node = None
    message_librarybook000_flow002.next_node = None

    # ------------------------------------------------------

    # LibraryBook001
    entry: LMSEntryNode = flows.new_flowchart("LibraryBook001")

    message_librarybook001_flow000 = LMSMessageNode()
    message_librarybook001_flow000.message_label = "LibraryBook001_Flow000"

    branch_yes_no = LMSBranchNode()
    branch_yes_no.arg2 = 0
    branch_yes_no.arg3 = 3

    message_librarybook001_flow001 = LMSMessageNode()
    message_librarybook001_flow001.message_label = "LibraryBook001_Flow001"

    message_librarybook001_flow002 = LMSMessageNode()
    message_librarybook001_flow002.message_label = "LibraryBook001_Flow002"

    entry.next_node = message_librarybook001_flow000
    message_librarybook001_flow000.next_node = branch_yes_no
    branch_yes_no.next_node = message_librarybook001_flow001
    branch_yes_no.next_node_else = message_librarybook001_flow002
    message_librarybook001_flow001.next_node = None
    message_librarybook001_flow002.next_node = None

    # ------------------------------------------------------

    # LibraryBook002
    entry: LMSEntryNode = flows.new_flowchart("LibraryBook002")

    message_librarybook002_flow000 = LMSMessageNode()
    message_librarybook002_flow000.message_label = "LibraryBook002_Flow000"

    branch_yes_no = LMSBranchNode()
    branch_yes_no.arg2 = 0
    branch_yes_no.arg3 = 3

    message_librarybook002_flow001 = LMSMessageNode()
    message_librarybook002_flow001.message_label = "LibraryBook002_Flow001"

    message_librarybook002_flow002 = LMSMessageNode()
    message_librarybook002_flow002.message_label = "LibraryBook002_Flow002"

    entry.next_node = message_librarybook002_flow000
    message_librarybook002_flow000.next_node = branch_yes_no
    branch_yes_no.next_node = message_librarybook002_flow001
    branch_yes_no.next_node_else = message_librarybook002_flow002
    message_librarybook002_flow001.next_node = None
    message_librarybook002_flow002.next_node = None

    # ------------------------------------------------------

    # LibraryBook003
    entry: LMSEntryNode = flows.new_flowchart("LibraryBook003")

    message_librarybook003_flow000 = LMSMessageNode()
    message_librarybook003_flow000.message_label = "LibraryBook003_Flow000"

    branch_yes_no = LMSBranchNode()
    branch_yes_no.arg2 = 0
    branch_yes_no.arg3 = 3

    message_librarybook003_flow001 = LMSMessageNode()
    message_librarybook003_flow001.message_label = "LibraryBook003_Flow001"

    message_librarybook003_flow002 = LMSMessageNode()
    message_librarybook003_flow002.message_label = "LibraryBook003_Flow002"

    entry.next_node = message_librarybook003_flow000
    message_librarybook003_flow000.next_node = branch_yes_no
    branch_yes_no.next_node = message_librarybook003_flow001
    branch_yes_no.next_node_else = message_librarybook003_flow002
    message_librarybook003_flow001.next_node = None
    message_librarybook003_flow002.next_node = None


@register_flows("IntroRosettaZone", "IntroRosettaZone")
def flows_IntroRosettaZone(flows: LMSFlows):
    # HelperWitch000
    entry: LMSEntryNode = flows.new_flowchart("HelperWitch000")

    event_msg_arg_player = LMSEventNode()
    event_msg_arg_player.event_type = 0
    event_msg_arg_player.event_param = 5

    branch_reset_before = LMSBranchNode()
    branch_reset_before.condition_type = 1
    branch_reset_before.condition_param = 0

    message_helperwitch000_flow000 = LMSMessageNode()
    message_helperwitch000_flow000.message_label = "HelperWitch000_Flow000"

    branch_yes_no_player = LMSBranchNode()
    branch_yes_no_player.condition_type = 0
    branch_yes_no_player.condition_param = 19

    event_player_luigi = LMSEventNode()
    event_player_luigi.event_type = 0
    event_player_luigi.event_param = 3

    event_player_mario = LMSEventNode()
    event_player_mario.event_type = 0
    event_player_mario.event_param = 2

    event_msg_arg_daredevil = LMSEventNode()
    event_msg_arg_daredevil.event_type = 0
    event_msg_arg_daredevil.event_param = 4

    message_helperwitch000_flow001 = LMSMessageNode()
    message_helperwitch000_flow001.message_label = "HelperWitch000_Flow001"

    branch_yes_no_daredevil = LMSBranchNode()
    branch_yes_no_daredevil.condition_type = 0
    branch_yes_no_daredevil.condition_param = 18

    event_on_daredevil_mode = LMSEventNode()
    event_on_daredevil_mode.event_type = 0
    event_on_daredevil_mode.event_param = 0

    event_off_daredevil_mode = LMSEventNode()
    event_off_daredevil_mode.event_type = 0
    event_off_daredevil_mode.event_param = 1

    branch_should_reset = LMSBranchNode()
    branch_should_reset.condition_type = 1
    branch_should_reset.condition_param = 1

    message_helperwitch000_flow002 = LMSMessageNode()
    message_helperwitch000_flow002.message_label = "HelperWitch000_Flow002"

    message_helperwitch000_flow002_for_reset = LMSMessageNode()
    message_helperwitch000_flow002_for_reset.message_label = "HelperWitch000_Flow002"

    event_reset_scene = LMSEventNode()
    event_reset_scene.event_type = 0
    event_reset_scene.event_param = 6

    message_helperwitch000_flow003 = LMSMessageNode()
    message_helperwitch000_flow003.message_label = "HelperWitch000_Flow003"

    branch_yes_no_settings = LMSBranchNode()
    branch_yes_no_settings.condition_type = 0
    branch_yes_no_settings.condition_param = 4

    event_msg_arg_player_2 = LMSEventNode()
    event_msg_arg_player_2.event_type = 0
    event_msg_arg_player_2.event_param = 5

    message_helperwitch000_flow004 = LMSMessageNode()
    message_helperwitch000_flow004.message_label = "HelperWitch000_Flow004"

    entry.next_node = event_msg_arg_player
    event_msg_arg_player.next_node = branch_reset_before
    branch_reset_before.next_node = message_helperwitch000_flow003
    branch_reset_before.next_node_else = message_helperwitch000_flow000
    message_helperwitch000_flow000.next_node = branch_yes_no_player

    message_helperwitch000_flow003.next_node = branch_yes_no_settings
    branch_yes_no_settings.next_node = event_msg_arg_player_2
    branch_yes_no_settings.next_node_else = message_helperwitch000_flow002
    event_msg_arg_player_2.next_node = message_helperwitch000_flow004
    message_helperwitch000_flow004.next_node = branch_yes_no_player

    branch_yes_no_player.next_node = event_player_luigi
    branch_yes_no_player.next_node_else = event_player_mario
    event_player_luigi.next_node = event_msg_arg_daredevil
    event_player_mario.next_node = event_msg_arg_daredevil
    event_msg_arg_daredevil.next_node = message_helperwitch000_flow001
    message_helperwitch000_flow001.next_node = branch_yes_no_daredevil
    branch_yes_no_daredevil.next_node = event_on_daredevil_mode
    branch_yes_no_daredevil.next_node_else = event_off_daredevil_mode
    event_on_daredevil_mode.next_node = branch_should_reset
    event_off_daredevil_mode.next_node = branch_should_reset
    branch_should_reset.next_node = message_helperwitch000_flow002_for_reset
    branch_should_reset.next_node_else = message_helperwitch000_flow002
    message_helperwitch000_flow002_for_reset.next_node = event_reset_scene
    message_helperwitch000_flow002.next_node = message_helperwitch000_flow003
    event_reset_scene.next_node = None


@register_flows("RedBlueExGalaxy", "RedBlueExGalaxy")
def flows_RedBlueExGalaxy(flows: LMSFlows):
    # Moc000
    entry: LMSEntryNode = flows.new_flowchart("Moc000")

    message_moc000_flow000 = LMSMessageNode()
    message_moc000_flow000.message_label = "Moc000_Flow000"

    event_event_func = LMSEventNode()
    event_event_func.arg1 = 0

    message_moc000_flow001 = LMSMessageNode()
    message_moc000_flow001.message_label = "Moc000_Flow001"

    entry.next_node = message_moc000_flow000
    message_moc000_flow000.next_node = event_event_func
    event_event_func.next_node = message_moc000_flow001
    message_moc000_flow001.next_node = message_moc000_flow001


@register_flows("UnderwaterCaveZone", "UnderwaterCaveZone")
def flows_UnderwaterCaveZone(flows: LMSFlows):
    # PenguinCoach000
    entry: LMSEntryNode = flows.new_flowchart("PenguinCoach000")

    message_penguincoach000_flow000 = LMSMessageNode()
    message_penguincoach000_flow000.message_label = "PenguinCoach000_Flow000"

    message_penguincoach001_flow001 = LMSMessageNode()
    message_penguincoach001_flow001.message_label = "PenguinCoach000_Flow001"

    branch_power_star = LMSBranchNode()
    branch_power_star.arg2 = 8

    message_penguincoach001_flow002 = LMSMessageNode()
    message_penguincoach001_flow002.message_label = "PenguinCoach000_Flow002"

    entry.next_node = message_penguincoach000_flow000
    message_penguincoach000_flow000.next_node = message_penguincoach001_flow001
    message_penguincoach001_flow001.next_node = branch_power_star
    branch_power_star.next_node = message_penguincoach001_flow002
    branch_power_star.next_node_else = message_penguincoach001_flow001
    message_penguincoach001_flow002.next_node = message_penguincoach001_flow002

    # Kinopio000
    entry: LMSEntryNode = flows.new_flowchart("Kinopio000")

    message_kinopio000_flow000 = LMSMessageNode()
    message_kinopio000_flow000.message_label = "Kinopio000_Flow000"

    message_kinopio001_flow000 = LMSMessageNode()
    message_kinopio001_flow000.message_label = "Kinopio000_Flow001"

    entry.next_node = message_kinopio000_flow000
    message_kinopio000_flow000.next_node = message_kinopio001_flow000
    message_kinopio001_flow000.next_node = message_kinopio001_flow000


########################################################################################################################

if __name__ == '__main__':
    build_message_data()
