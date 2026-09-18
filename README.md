# Neo Mario Galaxy

**IMPORTANT: If you are looking to just install the mod for yourself, check the [official website]
(https://aurumsmods.com/nmg) for download links and more information.**

This repository contains the entire source for **Neo Mario Galaxy**, a modification of *Super Mario Galaxy 2* (SMG2). 
Throughout this codebase, the modification and all available localizations of it are simply referred to as *NMG*. 
The source does not include the source code and ISO files of the original SMG2 titles. Generally, NMG supports all five 
regional versions of SMG2.

## Setup

```TODO: This section needs to be rewritten once the codebase becomes more stable in terms of structure.```

This project requires **Python 3.12+** and additional modules (see [requirements.txt](requirements.txt).) The
[PyCharm](https://www.jetbrains.com/pycharm/) IDE was primarily used to develop this project, though other IDEs will
work just fine. To build the Syati module, the CodeWarrior compiler (``mwcceppc.exe`` and ``mwasmeppc.exe``) and the 
Kamek linker (``Kamek.exe``) are required. Lastly, a dump of all unchanged ``LocalizeData`` assets from the original
games need to be supplied.

## Usage

The codebase includes many CLI scripts to build the mod or parts of it. Further, there's a bunch of useful helper 
scripts. All helper scripts can be found in the ``helpers`` folder. This section briefly outlines all the available 
building-related tools.

| Run configuration                | Description                                                                                                                                                                   |
|----------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| ``make``                         | Cleans ``dist`` folder and rebuilds everything. Essentially, this runs all tools at once.                                                                                     |
| ``copy_to_game``                 | Copies static resources from the ``data`` folder to the active game dump.                                                                                                     |
| ``build_message_data``           | Builds ``MessageData`` archives for all supported languages and outputs them to the active game dump.                                                                         |
| ``pack_font_archives``           | Packs ``Font.arc`` for all supported languages and outputs them to the active game dump.                                                                                      |
| ``build_syati_module``           | Builds the Syati module (``CustomCode*.bin``) for all supported regions and outputs the binaries to the active game dump.                                                     |
| ``heap_memory``                  | Updates heap-related BCSV files in the active game dump's ``SystemDataTable.arc`` file.                                                                                       |
| ``repack_use_resource_archives`` | Packs ``UseResource`` archives for all stages based on the raw logs in the ``data`` folder.                                                                                   |
| ``pack_zip_archive``             | Recreates ``dist`` folder by generating Riivolution XML files and by collecting all non-vanilla files in the active game dump. This also creates a ZIP file for distribution. |

## Credits

The following people directly contributed to the development of Neo Mario Galaxy. Detailed credits with responsibilities
can be found in [credits.md](data/dist/nmg/credits.md).

- Aurum
- AJ DiSpirito
- Alex SMG
- DDinghoya
- Floaty
- froggo
- Gabbo
- Hadau
- MikaMusic
- Mixlas
- Mr. BMY
- MTLenz
- Samurai47
- StationAlpha
- Super Hackio
- Super 0'Brothers
- SY24
