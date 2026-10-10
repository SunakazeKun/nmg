"""
UseResource archive generator

Generates UseResource archives for all configured stages based on asset loading logs. The log files are copied from the
UseResourceLogs folder and processed using 'smguseres'.
"""

from config import GAME_PATH, USE_RESOURCE_STAGE_INFOS
from smguseres import UseResourceGenerator
from argparse import ArgumentParser
from pathlib import Path

import shutil
import logging

__all__ = ["pack_use_resource_archives", "clear_use_resource_archives"]


########################################################################################################################

SRC_USE_RESOURCE_LOGS_PATH = Path("data/UseResourceLogs")


def pack_use_resource_archives():
    game_files_path = GAME_PATH / "files"
    game_use_resource_logs_path = game_files_path / "UseResourceLogs"

    if game_use_resource_logs_path.exists():
        shutil.rmtree(game_use_resource_logs_path)
    shutil.copytree(SRC_USE_RESOURCE_LOGS_PATH, game_use_resource_logs_path)

    for stage_name, unique_scenarios_count, use_common, in USE_RESOURCE_STAGE_INFOS:
        logging.info("Packing UseResource archive for %s", stage_name)

        builder = UseResourceGenerator(str(game_files_path), stage_name, unique_scenarios_count, use_common)
        builder.write_analyzed()


def clear_use_resource_archives():
    game_files_path = GAME_PATH / "files"

    for stage_name, unique_scenarios_count, use_common in USE_RESOURCE_STAGE_INFOS:
        logging.info("Clearing UseResource archive for %s", stage_name)

        builder = UseResourceGenerator(str(game_files_path), stage_name, unique_scenarios_count, use_common)
        builder.write_dummy()


########################################################################################################################

def __main():
    parser = ArgumentParser()
    parser.add_argument("command", choices=["pack", "clear"])
    args = parser.parse_args()

    match args.command:
        case "pack":
            pack_use_resource_archives()
        case "clear":
            clear_use_resource_archives()
        case _:
            exit(1)


if __name__ == '__main__':
    __main()
