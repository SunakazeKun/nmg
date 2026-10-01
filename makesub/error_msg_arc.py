"""
ErrorMessageArchive packaging

Generates ErrorMessageArchive archives for all supported regions based on the files in the ErrorMessageArchive folder.
"""

from pyjkernel import create_new_archive, write_archive_buffer, JKRCompression, write_archive_file
from msb_adapter import NeoMarioGalaxyAdapter, TalkType, BalloonType, SoundID, CameraType
from config import GAME_PATH, SUPPORTED_REGIONS, iter_languages_by_region
from pymsb import LMSDocument
from pathlib import Path
from tools.dataio import read_json
from typing import Any

import logging

__all__ = ["pack_error_msg_archives", "pack_error_msg_archive"]


########################################################################################################################

SRC_ERROR_MSG_ARCHIVE = Path("data/ErrorMessageArchive")


def pack_error_msg_archives():
    for region in SUPPORTED_REGIONS:
        pack_error_msg_archive(region)


def pack_error_msg_archive(region: str):
    root_name = f'ErrorMessageArchive{region}'

    logging.info("Packing %s.arc", root_name)

    error_msg_arc = create_new_archive(f'ErrorMessageArchive{region}')
    error_msg_arc_path = GAME_PATH / f'files/SystemData/{root_name}.arc'

    # Prepare folders
    error_msg_arc.create_folder(f'{root_name}/LayoutData')
    error_msg_arc.create_folder(f'{root_name}/LocalizeData')

    for language in iter_languages_by_region(region):
        error_msg_arc.create_folder(f'{root_name}/LocalizeData/{language}')
        error_msg_arc.create_folder(f'{root_name}/LocalizeData/{language}/LayoutData')
        error_msg_arc.create_folder(f'{root_name}/LocalizeData/{language}/MessageData')

    # ErrorMessageWindow
    arc_path = SRC_ERROR_MSG_ARCHIVE / f'ErrorMessageWindow_{region}.arc'

    error_msg_arc.create_file(
        f'{root_name}/LayoutData/ErrorMessageWindow.arc',
        data=arc_path.read_bytes()
    )

    # EmbeddedFont
    for language in iter_languages_by_region(region):
        brfnt_path = SRC_ERROR_MSG_ARCHIVE / f'MessageFont26_{language}.brfnt'

        error_msg_arc.create_file(
            f'{root_name}/LocalizeData/{language}/LayoutData/EmbeddedFont.arc',
            data=__create_and_pack_embedded_font_archive(brfnt_path)
        )

    # EmbeddedMessage
    json_path = SRC_ERROR_MSG_ARCHIVE / "EmbeddedMessage.json"
    json_data = read_json(json_path)

    for language in iter_languages_by_region(region):
        error_msg_arc.create_file(
            f'{root_name}/LocalizeData/{language}/MessageData/EmbeddedMessage.arc',
            data=__create_and_pack_embedded_message_archive(language, json_data)
        )

    # Write ErrorMessageArchive
    write_archive_file(error_msg_arc, str(error_msg_arc_path))


def __create_and_pack_embedded_font_archive(brfnt_path: Path):
    arc = create_new_archive("EmbeddedFont")
    arc.create_file(
        "EmbeddedFont/MessageFont26.brfnt",
        data=brfnt_path.read_bytes()
    )

    return write_archive_buffer(arc, compression=JKRCompression.SZS)


def __create_and_pack_embedded_message_archive(language: str, json_data: list[dict[str, Any]]):
    arc = create_new_archive("EmbeddedMessage")
    document = LMSDocument(NeoMarioGalaxyAdapter)

    for json_msg in json_data:
        message = document.new_message(json_msg["label"])

        message.attributes["talk_type"]    = TalkType[json_msg["attributes"]["talk_type"]]
        message.attributes["balloon_type"] = BalloonType[json_msg["attributes"]["balloon_type"]]
        message.attributes["sound_id"]     = SoundID[json_msg["attributes"]["sound_id"]]
        message.attributes["camera_type"]  = CameraType[json_msg["attributes"]["camera_type"]]
        message.attributes["camera_id"]    = json_msg["attributes"]["camera_id"]
        message.attributes["msg_link_id"]  = json_msg["attributes"]["msg_link_id"]
        message.attributes["unk7"]         = json_msg["attributes"]["unk7"]
        message.attributes["comment"]      = json_msg["attributes"]["comment"]

        message.text = "\n".join(json_msg["languages"][language])

    msbt_file = arc.create_file("EmbeddedMessage/EmbeddedMessage.msbt")
    msbt_file.data = document.makebin()

    return write_archive_buffer(arc)


########################################################################################################################

if __name__ == '__main__':
    pack_error_msg_archives()
