# Neo Mario Galaxy
**If you are looking to just install the mod for yourself, check the [official website](https://aurumsmods.com/nmg) for download links and more information.**

This repository contains the entire source for **Neo Mario Galaxy** (NMG), a modification of *Super Mario Galaxy 2*
(SMG2). The source does not include the source code and ISO files of the original SMG2 titles. Generally, NMG supports
all five regional versions of SMG2.

## To-dos
- Review Italian localization (``EuItalian``)
- Review Japanese localization (``JpJapanese``)
- Dutch localization (``EuDutch``)
- Simplified Chinese localization (``CnSimpChinese``)
- Portuguese localization (``EuPortuguese``)
- Brazilian Portuguese localization (``UsPortuguese``)
- Russian localization (``EuRussian``)

## Setup
```TODO: This section needs to be rewritten once the codebase becomes more stable in terms of structure.```

This project requires **Python 3.12+** and additional modules (see [requirements.txt](requirements.txt).) The
[PyCharm](https://www.jetbrains.com/pycharm/) IDE was primarily used to develop this project, though other IDEs will
work just fine. To build the Syati module, the CodeWarrior compiler (``mwcceppc.exe`` and ``mwasmeppc.exe``) and the 
Kamek linker (``Kamek.exe``) are required.

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
- MarioGaming64
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
