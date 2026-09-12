"""
UseResource archive generator

Generates UseResource archives for all configured stages based on asset loading logs. The logs are loaded from
'<GAME_PATH>/files/UseResourceLogs' and are processed using 'smguseres'.
"""

from config import GAME_PATH, USE_RESOURCE_STAGE_INFOS
from smguseres import UseResourceGenerator
from argparse import ArgumentParser
from pathlib import Path

__all__ = ["repack_use_resource_archives", "clear_all_use_resource_archives"]


########################################################################################################################

def repack_use_resource_archives(game_files_path: Path):
    for stage_name, unique_scenarios_count, use_common, in USE_RESOURCE_STAGE_INFOS:
        builder = UseResourceGenerator(str(game_files_path), stage_name, unique_scenarios_count, use_common)
        builder.write_analyzed()


def clear_all_use_resource_archives(game_files_path: Path):
    for stage_name, unique_scenarios_count, use_common in USE_RESOURCE_STAGE_INFOS:
        builder = UseResourceGenerator(str(game_files_path), stage_name, unique_scenarios_count, use_common)
        builder.write_dummy()


########################################################################################################################

def __main():
    game_files_path = GAME_PATH / "files"

    parser = ArgumentParser()
    sub_parsers = parser.add_subparsers(dest="command", required=True)

    repack_parser = sub_parsers.add_parser("repack")
    repack_parser.set_defaults(func=lambda a: repack_use_resource_archives(game_files_path))

    clear_parser = sub_parsers.add_parser("clear")
    clear_parser.set_defaults(func=lambda a: clear_all_use_resource_archives(game_files_path))

    args = parser.parse_args()
    args.func(args)


if __name__ == '__main__':
    __main()
