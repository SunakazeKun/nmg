from tools.dataio import read_bin, write_bin
from pathlib import Path


########################################################################################################################

NMG_MEMORY_PATH = Path("data/dist/nmg/memory")
ERROR_MESSAGE_ARCHIVE_SIZES = [
    ("SB4P", 0xB780),
    ("SB4E", 0x6080),
    ("SB4J", 0x5860),
    ("SB4W", 0x5D80),
    ("SB4K", 0x4920),
]


def fix_error_message_archives():
    for game_id, max_arc_size in ERROR_MESSAGE_ARCHIVE_SIZES:
        arc_path = NMG_MEMORY_PATH / f'ErrorMessageArchive{game_id}.arc'
        arc_bin = read_bin(arc_path)
        arc_size = len(arc_bin)

        if arc_size < max_arc_size:
            arc_bin += bytes(max_arc_size - arc_size)
        elif arc_size > max_arc_size:
            raise ValueError(f'ErrorMessageArchive too large for {game_id}!')

        write_bin(arc_path, arc_bin)


########################################################################################################################

if __name__ == '__main__':
    fix_error_message_archives()
