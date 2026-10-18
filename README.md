# Neo Mario Galaxy
**If you are looking to just install the mod for yourself, check the [official website](https://aurumsmods.com/nmg)
for download links and more information.**

This repository contains the entire source for **Neo Mario Galaxy** (NMG), a modification of *Super Mario Galaxy 2*
(SMG2). The source does not include the source code and ISO files of the original SMG2 titles. NMG supports all five
regional versions of SMG2.

## To-dos
- Review Italian localization (``EuItalian``)
- Review Japanese localization (``JpJapanese``)
- Dutch localization (``EuDutch``)
- Simplified Chinese localization (``CnSimpChinese``)
- Portuguese localization (``EuPortuguese``)
- Brazilian Portuguese localization (``UsPortuguese``)
- Russian localization (``EuRussian``)

## Setup
This section describes how to build NMG from source. Specifically, this describes the process using the PyCharm IDE.
You can also use other IDEs; however, the run configurations in this repository explicitly work with PyCharm only.
Again, check the [official website](https://aurumsmods.com/nmg) for download links.

### Requirements
- [Python 3.12](https://www.python.org/downloads/) (or newer)
- [PyCharm](https://www.jetbrains.com/pycharm/) (Community Edition is fine)
- CodeWarrior PPC EABI C/C++ Compiler and Assembler, version 4.0.0
- [Kamek linker](https://github.com/Treeki/Kamek)
- [Dolphin Emulator](https://dolphin-emu.org/)
- Dump of NMG's ``AudioRes`` folder (see existing releases)
- Valid Super Mario Galaxy 2 game dump (ISO, RVZ, etc.)
- Unmodified ``main.dol`` files for all SMG2 versions

### Instructions
#### 1. Python environment
1. Open the project in PyCharm.
2. Create a new Python virtual environment (venv) using Python 3.12.
3. Install all requirements listed in ``requirements.txt``.

#### 2. Active game dump
1. Open Dolphin Emulator.
2. Right-click an unmodified *Super Mario Galaxy 2* game and select ``Properties``.
3. Keep note of the ``Game ID`` displayed in the ``Info`` tab (e.g. ``SB4J`` for the Japanese version).
4. Navigate to the ``Filesystem`` tab, right-click on ``Data Partition``, and select ``Extract Entire Partition``.
5. Extract the files to a new folder. **Keep note of this folder's full path**, as this is your active game dump.
6. In Dolphin, navigate to ``Config`` -> ``Paths`` tab.
7. Click ``Add`` and select the ``sys`` folder inside your newly extracted game folder.

#### 3. Original ``main.dol`` files
1. Create a dedicated folder for your original binaries.
2. Move all unmodified ``main.dol`` files into this folder and rename each file to match its original game ID. For example, rename the Japanese version's ``main.dol`` to ``SB4J.dol``.

#### 4. Environment file (``.env``)
1. Copy the ``.env.example`` file in the project root and rename the copy to ``.env``.
2. Open ``.env`` in a text editor and fill in the following values:
   - ``GAME_ID``: Your active game dump's ID (e.g., ``SB4J``).
   - ``GAME_PATH``: Path to the folder containing the extracted SMG2 game.
   - ``MWCCEPPC_PATH``: Path to CodeWarrior's ``mwcceppc.exe``.
   - ``MWASMEPPC_PATH``: Path to CodeWarrior's ``mwasmeppc.exe``.
   - ``KAMEK_PATH``: Path to Kamek's ``Kamek.exe``.
   - ``NEW_AUDIO_RES_PATH``: Path to the directory containing NMG's ``AudioRes`` folder.
   - ``ORG_MAIN_DOLS_PATH``: Path to the directory containing all unmodified ``main.dol`` files.
3. Save your changes.

#### 5. Build and test
1. Run ``main.py`` to modify the active game dump and build NMG and its assets.
2. Launch the active game dump in Dolphin to verify that everything works correctly.

## Usage
The codebase includes many CLI scripts to build the mod or parts of it. This section briefly outlines all the available 
building-related tools. Further, there's a bunch of useful helper scripts. All helper scripts can be found in the 
``helpers`` folder.

| Run configuration              | Description                                                                                                                                         |
|--------------------------------|-----------------------------------------------------------------------------------------------------------------------------------------------------|
| ``make``                       | Cleans ``build`` folder and rebuilds everything by running all tools at once.                                                                       |
| ``copy_static_files_to_game``  | Copies raw assets from the ``data/arcs`` directory to the active game dump.                                                                         |
| ``pack_all_stage_data``        | Packs all stage archives based on the data in the ``data/StageData`` folders and outputs them to the active game dump.                              |
| ``pack_use_resource_archives`` | Packs ``UseResource`` archives for all stages based on the logs in the ``data/UseResourceLogs`` directory and outputs them to the active game dump. |
| ``pack_other_archives``        | Packs miscellaneous simple archive files, for example ``SystemDataTable.arc`` and ``StageBgmInfo.arc``, and outputs them to the active game dump.   |
| ``build_message_data``         | Builds ``MessageData`` archives for all supported languages and outputs them to the active game dump.                                               |
| ``pack_error_msg_archives``    | Packs error message archives for all target regions and outputs them to the active game dump.                                                       |
| ``build_syati_module``         | Builds the Syati module (``CustomCode_*.bin``) and loader for all supported regions and outputs the binaries to the active game dump.               |
| ``pack_zip_archive``           | Generates Riivolution XML files and collects all non-vanilla files in the active game dump. This also creates a ZIP file for distribution.          |
| ``generate_web_page``          | Generates the web page.                                                                                                                             |

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
- Jau_CR
- MikaMusic
- Mixlas
- Mr. BMY
- MTLenz
- Myron Dragon
- Samurai47
- StationAlpha
- Super Hackio
- Super 0'Brothers
- SY24
