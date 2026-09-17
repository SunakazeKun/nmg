from tools.dataio import read_lines, write_lines
from pathlib import Path

import re


########################################################################################################################

USE_RESOURCE_LOGS_PATH = Path("data/UseResourceLogs")

ARCHIVE_PATTERN = re.compile(r'^.*ARCHIVE\s+')
SOUND_PATTERN = re.compile(r'^.*SOUND\s+')

LOCALIZE_DATA_PATTERN = re.compile(r'^/LocalizeData/[^/]+')
PLAYER_ARC_NORMALIZATION = {
    "/ObjectData/Luigi.arc":                             "/ObjectData/Mario.arc",
    "/ObjectData/LuigiCap.arc":                          "/ObjectData/MarioCap.arc",
    "/ObjectData/LuigiFace.arc":                         "/ObjectData/MarioFace.arc",
    "/ObjectData/LuigiHair.arc":                         "/ObjectData/MarioHair.arc",
    "/ObjectData/LuigiHairAnim.arc":                     "/ObjectData/MarioHairAnim.arc",
    "/ObjectData/LuigiHairAnimMarioFaceShipGalaxy2.arc": "/ObjectData/MarioHairAnimMarioFaceShipGalaxy2.arc",
    "/ObjectData/LuigiHairAnimVsKoopaLv1Galaxy.arc":     "/ObjectData/MarioHairAnimVsKoopaLv1Galaxy.arc",
    "/ObjectData/LuigiShadow.arc":                       "/ObjectData/MarioShadow.arc",
    "/ObjectData/LuigiTornado.arc":                      "/ObjectData/MarioTornado.arc",
    "/ObjectData/BoneLuigi.arc":                         "/ObjectData/BoneMario.arc",
    "/ObjectData/BeeLuigi.arc":                          "/ObjectData/BeeMario.arc",
    "/ObjectData/CloudLuigi.arc":                        "/ObjectData/CloudMario.arc",
    "/ObjectData/CloudLuigiCap.arc":                     "/ObjectData/CloudMarioCap.arc",
    "/ObjectData/HopperLuigi.arc":                       "/ObjectData/HopperMario.arc",
    "/ObjectData/HorrorLuigi.arc":                       "/ObjectData/HorrorMario.arc",
    "/ObjectData/IceLuigi.arc":                          "/ObjectData/IceMario.arc",
    "/ObjectData/InvincibleLuigi.arc":                   "/ObjectData/InvincibleMario.arc",
    "/ObjectData/RockLuigi.arc":                         "/ObjectData/RockMario.arc",
    "/ObjectData/RockLuigiRollingRock.arc":              "/ObjectData/RockMarioRollingRock.arc",
    "/ObjectData/TeresaLuigi.arc":                       "/ObjectData/TeresaMario.arc",
}


def clean_use_resource_logs():
    for log_file in USE_RESOURCE_LOGS_PATH.iterdir():
        archive_lines = []
        sound_lines = []

        for raw_line in read_lines(log_file):
            if ARCHIVE_PATTERN.match(raw_line):
                resource = ARCHIVE_PATTERN.sub("", raw_line).strip()

                if LOCALIZE_DATA_PATTERN.match(resource):
                    resource = LOCALIZE_DATA_PATTERN.sub("", resource)

                if resource in PLAYER_ARC_NORMALIZATION:
                    resource = PLAYER_ARC_NORMALIZATION[resource]

                archive_line = "ARCHIVE " + resource

                if archive_line not in archive_lines:
                    archive_lines.append(archive_line)

            elif SOUND_PATTERN.match(raw_line):
                resource = SOUND_PATTERN.sub("", raw_line).strip()

                sound_line = "SOUND " + resource

                if sound_line not in sound_lines:
                    sound_lines.append(sound_line)

        write_lines(log_file, archive_lines + sound_lines)


########################################################################################################################

if __name__ == '__main__':
    clean_use_resource_logs()
