from airium import Airium
from pathlib import Path
from datetime import datetime

import os
import shutil
import logging

__all__ = ["generate_web_page"]


########################################################################################################################

AUTHOR_NAME = "Aurum"
AUTHOR_URL = "https://aurumsmods.com"

SITE_LANGUAGE = "en"
SITE_AUTHOR = AUTHOR_NAME
SITE_NAME = "Neo Mario Galaxy"
SITE_URL = "https://aurumsmods.com/nmg"
SITE_IMAGE = "https://aurumsmods.com/nmg/assets/preview.png"
SITE_DESCRIPTION = f"A Super Mario Galaxy 2 mod featuring 42 Power Stars in eight all-new Galaxies!"
SITE_KEYWORDS = [
    "super", "mario", "galaxy", "neo", "smg1", "smg2", "nmg", "daredevil", "homebrew", "riivolution",
    "mod", "modding", "hack", "hacking", "level", "world", "custom", "wii", "wiiu"
]

URL_DISCORD_SERVER = "https://discord.gg/C3px4rejUX"
URL_NMG_TRAILER = "https://www.youtube.com/embed/NE4SSfk1iLs"
URL_NMG_FILES = "https://mega.nz/folder/xhcDWTzQ#sf7Iya2oNGqQ15LWnWYjWQ"
URL_SMG2_TEXTURES = "https://forums.dolphin-emu.org/Thread-super-mario-galaxy-2-hd-texture-mod"
URL_DOLPHIN_WIKI = "https://wiki.dolphin-emu.org/index.php?title=Riivolution_Mods"
URL_RIIVOLUTION_WIKI = "https://wiki.hacks.guide/wiki/Wii:Riivolution"

TRAILER_EMBED_WIDTH = 448
TRAILER_EMBED_HEIGHT = 252

FEATURES_ITEMS = [
    ("lumaPurple",   f'Supports all major game versions.'),
    ("coinBlock",    f'<b>Eight new Galaxies</b> featuring <b>42 Power Stars</b>!'),
    ("iceFlower",    f'The return of the <b>Ice Flower</b> and <b>Flying Star</b>!'),
    ("kinokoLifeUp", f'Choose between <b>Normal</b> and <b>Daredevil</b> mode.'),
    ("noteBlock",    f'Custom and remixed <b>music</b>.'),
    ("illustItem",   f'New collectable <b>items</b>!'),
]

SB4P_LANGUAGES = ["English", "German", "Italian", "Spanish", "French"]  # Dutch, Portuguese, Russian
SB4E_LANGUAGES = ["English", "Spanish", "French"]  # Portuguese
SB4J_LANGUAGES = ["Japanese"]
SB4W_LANGUAGES = ["Chinese (Traditional)"]  # Chinese (Simplified)
SB4K_LANGUAGES = ["Korean"]

COMPATIBILITY_ITEMS = [
    ("SB4P", SB4P_LANGUAGES, "Super Mario Galaxy 2", "Europe / Australia"),
    ("SB4E", SB4E_LANGUAGES, "Super Mario Galaxy 2", "North America"),
    ("SB4J", SB4J_LANGUAGES, "スーパーマリオギャラクシー 2", "Japan"),
    ("SB4W", SB4W_LANGUAGES, "超級瑪利歐銀河２", "Taiwan / Hong Kong"),
    ("SB4K", SB4K_LANGUAGES, "슈퍼 마리오 Wii 2", "Korea"),
]

SCREENSHOTS_ITEMS = [
    ("title01",          "File Select"),
    ("cloudygarden01",   "Cloudy Garden Galaxy"),
    ("sweettreat01",     "Sweet Treat Galaxy"),
    ("cosmicspirit01",   "Cosmic Spirit Intro"),
    ("turnlog01",        "Turnlog Galaxy"),
    ("sweettreat02",     "World Map Screen"),
    ("gloomygalleon01",  "Gloomy Galleon Galaxy"),
    ("itemcollection01", "Item Collection"),
    ("battleblast01",    "Battle Blast Galaxy"),
    ("cometobs01",       "Comet Observatory"),
    ("eepbeep01",        "Eep Beep Galaxy"),
    ("krakenking01",     "Kraken King Galaxy"),
]

INSTALLATION_LINKS = [
    ("fa-solid fa-fw fa-download", URL_NMG_FILES, "Download Neo Mario Galaxy"),
    ("fa-brands fa-fw fa-discord", URL_DISCORD_SERVER, "Official Discord server"),
]

CREDITS_ITEMS = [
    (AUTHOR_NAME,        AUTHOR_URL),
    ("AJ DiSpirito",     "https://www.youtube.com/@AJDiSpirito"),
    ("Alex SMG",         "https://www.youtube.com/@Alex-SMG"),
    ("DDinghoya",        None),
    ("Floaty",           "https://www.youtube.com/@FloatyTico"),
    ("froggo",           "https://www.youtube.com/@froggo4321"),
    ("Gabbo",            "https://www.youtube.com/@gabbo6559"),
    ("Hadau",            None),
    ("Jau_CR",           None),
    ("MarioGaming64",    "https://www.youtube.com/@MarioGaming64"),
    ("MikaMusic",        None),
    ("Mixlas",           None),
    ("Mr. BMY",          "https://www.youtube.com/@MrBMY-dc5md"),
    ("MTLenz",           "https://www.youtube.com/@MTLenz"),
    ("Myron Dragon",     "https://www.youtube.com/@myrondragon"),
    ("Samurai47",        None),
    ("StationAlpha",     None),
    ("Super Hackio",     "https://www.youtube.com/@SuperHackio"),
    ("Super 0'Brothers", "https://www.youtube.com/@super0brothers46"),
    ("SY24",             "https://www.youtube.com/@SY24_"),
]

# ----------------------------------------------------------------------------------------------------------------------

PAGE_DESCRIPTION = (f'<b>Neo Mario Galaxy</b> is an unofficial modification of <em>Super Mario Galaxy 2</em> that can '
                    f'be played legally on any homebrew-enabled Wii or Wii U console. No backup images or hardware '
                    f'modifications are required. First published in July 2017, this is the first full SMG2 hack. '
                    f'Since then, it has undergone numerous updates to improve gameplay and add new features.')

COPYRIGHT_START_YEAR = 2015
COPYRIGHT_DESCRIPTION = (f'<em>Super Mario</em> characters and images belong to Nintendo. This website is not '
                         f'affiliated with or endorsed by Nintendo. <em>Neo Mario Galaxy</em> requires an original '
                         f'and legally obtained copy of <em>Super Mario Galaxy 2</em> to be played.')

# ----------------------------------------------------------------------------------------------------------------------

COMPATIBILITY_DESCRIPTION = (f'<em>Neo Mario Galaxy</em> works with all versions of <em>Super Mario Galaxy 2</em> and '
                             f'is available in the languages listed below.')

INSTALLATION_DESCRIPTION = (f'This guide provides step-by-step instructions for running <em>Neo Mario Galaxy</em> '
                            f'on a physical Wii or Wii U console. If you want to play the game on Dolphin Emulator, '
                            f'please check out the <a href="{URL_DOLPHIN_WIKI}" target="_blank">wiki page about '
                            f'launching Riivolution mods</a>. On Dolphin, you can also use the HD texture pack '
                            f'which is linked above. Huge thanks to SY24 for creating these textures! You can combine '
                            f'these textures with the <a href="{URL_SMG2_TEXTURES}" target="_blank">Super Mario '
                            f'Galaxy 2 HD texture pack</a> for the complete experience.')

REQUIREMENTS_ITEMS = [
    f'Wii or Wii U console',

    f'<a href="{URL_RIIVOLUTION_WIKI}">Riivolution v1.06</a>',

    f'Super Mario Galaxy 2 game disc',

    f'SD card with at least 96MB free space, formatted to FAT',

    f'A way to access SD cards from your computer',
]

SETUP_ITEMS = [
    f'Download the <em>Neo Mario Galaxy</em> package from the link above.',

    (f'Extract the <span class="code">nmg</span> and <span class="code">riivolution</span> folders and move them to '
     f'the root of your SD card.'),

    f'Safely eject the SD card and insert it into your console. Make sure that the SD card is unlocked.',
    
    f'Insert a retail <em>Super Mario Galaxy 2</em> disc into your console and launch the Riivolution app.',

    (f'In Riivolution, set <span class="code">Neo Mario Galaxy</span> to <span class="code">Enabled</span>. Make sure '
     f'that no other hacks are enabled to avoid conflicts!'),

    (f'You may also configure <span class="code">Override Language</span> if you want to use a language different '
     f'than the system-wide language.'),

    f'Press <span class="code">Launch</span> to start the game. <em>Neo Mario Galaxy</em> is now ready to play. Enjoy!',
]


# ======================================================================================================================

COPY_WEB_PATH = Path("data/web")
BUILD_WEB_PATH = Path("build/web")


def generate_web_page():
    if BUILD_WEB_PATH.exists():
        shutil.rmtree(BUILD_WEB_PATH)

    logging.info("Copying base web folder ...")
    shutil.copytree(COPY_WEB_PATH, BUILD_WEB_PATH)
    os.remove(BUILD_WEB_PATH / "assets/style.css")

    # ------------------------------------------------------

    logging.info("Generating web page ...")

    time = datetime.now()
    path = BUILD_WEB_PATH / "index.html"
    html = Airium(source_minify=True)
    html("<!DOCTYPE html>")

    with html.html(lang=SITE_LANGUAGE):
        with html.head():
            html.title(_t=SITE_NAME)
            html.meta(charset="UTF-8")
            html.meta(name="description", content=SITE_DESCRIPTION)
            html.meta(name="keywords", content=",".join(SITE_KEYWORDS))
            html.meta(name="author", content=SITE_AUTHOR)
            html.meta(name="og:title", content=SITE_NAME)
            html.meta(name="og:description", content=SITE_DESCRIPTION)
            html.meta(name="og:type", content="website")
            html.meta(name="og:image", content=SITE_IMAGE)
            html.meta(name="og:url", content=SITE_URL)
            html.meta(name="og:site_name", content=SITE_NAME)
            html.meta(name="viewport", content="width=device-width, initial-scale=1.0")
            html.link(rel="shortcut icon", type="image/x-icon", href="assets/favicon.ico")
            html.link(rel="stylesheet", href="assets/style.min.css")
            html.link(rel="stylesheet", href="assets/fonts/fontawesome/css/fontawesome.min.css")
            html.link(rel="stylesheet", href="assets/fonts/fontawesome/css/solid.min.css")
            html.link(rel="stylesheet", href="assets/fonts/fontawesome/css/brands.min.css")

        with html.body():
            with html.header():
                with html.a(href="."):
                    html.img(src="assets/logo.webp", alt=SITE_NAME, title=SITE_NAME)
                with html.p():
                    html("by ")
                    html.a(href="/", _t=SITE_AUTHOR)
                    html(f' (last updated {time.strftime("%B %d, %Y")})')

            with html.div(klass="container"):
                html.p(_t=PAGE_DESCRIPTION)

                with html.div(klass="split_flex"):
                    with html.div(klass="split_item"):
                        __insert_features(html)
                    with html.div(klass="split_item"):
                        __insert_trailer(html)

                __insert_screenshots(html)
                __insert_compatibility(html)
                __insert_installation(html)
                __insert_credits(html)

            with html.div(klass="container"), html.footer(), html.p():
                html(f'<b>Copyright &copy; {COPYRIGHT_START_YEAR}-{time.year} {SITE_AUTHOR}</b><br>')
                html(COPYRIGHT_DESCRIPTION)

    path.write_bytes(bytes(html))


# ======================================================================================================================

def __insert_features(html: Airium):
    with html.section():
        html.h2(id="features", _t="Features")

        with html.ul(klass="bullets"):
            for icon, text in FEATURES_ITEMS:
                html.li(klass=icon, _t=text)


def __insert_trailer(html: Airium):
    with html.section():
        html.h2(id="trailer", _t="2023 Trailer")

        with html.div(klass="youtube_wrapper"):
            html.iframe(klass="youtube_video", src=URL_NMG_TRAILER, width=str(TRAILER_EMBED_WIDTH),
                        height=str(TRAILER_EMBED_HEIGHT), frameborder="0", allowfullscreen="true")


def __insert_screenshots(html: Airium):
    with html.section():
        html.h2(id="screenshots", _t="Screenshots")

        with html.div(klass="screenshots"):
            for name, title in SCREENSHOTS_ITEMS:
                url = f'assets/{name}.jpg'
                alt = title

                html.img(klass="screenshot", src=url, alt=alt, title=title, width="320", height="175")


def __insert_compatibility(html: Airium):
    with html.section():
        html.h2(id="compatibility", _t="Compatibility")
        html.p(_t=COMPATIBILITY_DESCRIPTION)

        with html.div(klass="table_wrapper"), html.table():
            with html.tr():
                html.th(_t="ID")
                html.th(_t="Region")
                html.th(_t="Game")
                html.th(_t="Languages")

            for id_, languages, game, region in COMPATIBILITY_ITEMS:
                with html.tr():
                    with html.td():
                        html.span(klass="code", _t=id_)

                    html.td(_t=region)
                    html.td(_t=game)
                    html.td(_t=", ".join(languages))


def __insert_installation(html: Airium):
    with html.section():
        html.h2(id="installation", _t="Installation")

        with html.ul(klass="urls"):
            for icon_class, url, title in INSTALLATION_LINKS:
                with html.li(), html.a(href=url, target="_blank"):
                    html.i(klass=icon_class)
                    html(" ")
                    html(title)

        html.p(_t=INSTALLATION_DESCRIPTION)

        with html.section():
            html.h3(id="requirements", _t="Requirements")

            with html.ul():
                for text in REQUIREMENTS_ITEMS:
                    html.li(_t=text)

        with html.section():
            html.h3(id="setup", _t="Setup")

            with html.ol():
                for text in SETUP_ITEMS:
                    html.li(_t=text)


def __insert_credits(html: Airium):
    with html.section():
        html.h2(id="credits", _t="Credits")

        with html.ul():
            for name, url in CREDITS_ITEMS:
                with html.li():
                    if url is None:
                        html(name)
                    else:
                        with html.a(href=url):
                            html(name)


########################################################################################################################

if __name__ == '__main__':
    generate_web_page()
