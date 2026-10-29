"""
MessageData archive generator

Generates MessageData archives for all supported languages based on the folders and JSON files in the MessageData
folder. For some archives, the original archives are used as a base to add in new files.
"""

from pymsb import LMSFlows, LMSMessageNode, LMSBranchNode, LMSDocument, LMSFlowNode
from pyjkernel import (JKRArchive, JKRArchiveFile, JKRCompression, create_new_archive, write_archive_file)
from msb_flowcharts import has_flowchart_generator, get_flowchart_generator
from msb_adapter import NeoMarioGalaxyAdapter, TalkType, BalloonType, SoundID, CameraType
from config import PROJECT_VERSION, GAME_PATH, SUPPORTED_LANGUAGES, get_language_region
from tools.dataio import read_json, read_bin
from pathlib import Path

import logging

__all__ = ["build_message_data"]


# ======================================================================================================================
# Archive generation
# ======================================================================================================================

SRC_MESSAGE_DATA_PATH = Path("data/MessageData")
IGNORE_MESSAGES = [
    ("KrKorean",      "SystemMessage", "GalaxyNameShort", "GalaxyNameShort_New"),
    ("AsTradChinese", "SystemMessage", "GalaxyNameShort", "GalaxyNameShort_New"),
]


def build_message_data():
    for language in SUPPORTED_LANGUAGES:
        logging.info("Building MessageData for %s", language)

        for folder_path in filter(lambda p: p.is_dir(), SRC_MESSAGE_DATA_PATH.iterdir()):
            archive_name = folder_path.name
            archive_subpath = f'{language}/MessageData/{archive_name}.arc'

            archive = create_new_archive(archive_name)

            for json_path in filter(lambda p: p.is_file() and p.name.endswith(".json"), folder_path.iterdir()):
                msb_name = json_path.name.removesuffix(".json")
                msbf_path = folder_path / (msb_name + ".msbf")

                __pack_msb_into_archive(archive, language, archive_name, msb_name,
                                        json_path, msbf_path)

            archive_path = GAME_PATH / "files/LocalizeData" / archive_subpath
            archive_path.parent.mkdir(parents=True, exist_ok=True)
            write_archive_file(archive, str(archive_path), big_endian=True, compression=JKRCompression.NONE)


def __get_localized_msg_text(localized_lines: dict[str, list[str]], language: str,
                             archive_name: str, msb_name: str, label: str):
    if (msb_name, label) == ("Layout_PressStart", "PressStart_Version"):
        return f'v{PROJECT_VERSION}'

    # TODO: Remove this once Dutch translation is actually worked on.
    if language == "EuDutch":
        language = "EuEnglish"

    region = get_language_region(language)

    text_lines = localized_lines.get(language, None)

    if text_lines is None:
        text_lines = localized_lines.get(region, None)

    if text_lines is None:
        text_lines = localized_lines.get("default", None)

    if text_lines is None:
        logging.warning("Missing text: %s/%s/%s/%s", language, archive_name, msb_name, label)
        return f'!MISSING! {label}'

    return "\n".join(text_lines)


def __pack_msb_into_archive(archive: JKRArchive, language: str, archive_name: str, msb_name: str,
                            json_path: Path, msbf_path: Path):
    document = LMSDocument(NeoMarioGalaxyAdapter)
    msbt_labels = []

    # ------------------------------------------------------

    for json_msg in read_json(json_path):
        label = json_msg["label"]

        if (language, archive_name, msb_name, label) in IGNORE_MESSAGES:
            continue

        message = document.new_message(label)

        message.attributes["talk_type"]     = TalkType[json_msg["attributes"]["talk_type"]]
        message.attributes["balloon_type"]  = BalloonType[json_msg["attributes"]["balloon_type"]]
        message.attributes["sound_id"]      = SoundID[json_msg["attributes"]["sound_id"]]
        message.attributes["camera_type"]   = CameraType[json_msg["attributes"]["camera_type"]]
        message.attributes["camera_id"]     = json_msg["attributes"]["camera_id"]
        message.attributes["msg_link_id"]   = json_msg["attributes"]["msg_link_id"]
        message.attributes["msg_read_flag"] = json_msg["attributes"]["msg_read_flag"]
        message.attributes["comment"]       = json_msg["attributes"]["comment"]

        message.text = __get_localized_msg_text(json_msg["languages"], language, archive_name, msb_name, label)

        msbt_labels.append(message.label)

    msbt_file = __create_or_get_msb_file(archive, msb_name + ".msbt")
    msbt_file.data = document.makebin()

    # ------------------------------------------------------

    if msbf_path.is_file():
        msbf_data = read_bin(msbf_path)
    elif has_flowchart_generator(archive_name, msb_name):
        generate_flowcharts = get_flowchart_generator(archive_name, msb_name)

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


########################################################################################################################

if __name__ == '__main__':
    build_message_data()
