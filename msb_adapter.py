from __future__ import annotations
from enum import IntEnum
from pymsb import LMSAdapter, LMSException, BinaryMemoryIO
import io

__all__ = ["NeoMarioGalaxyAdapter",
           "PictureCode", "FontColor", "FontSize", "RaceTime",
           "TalkType", "BalloonType", "CameraType", "SoundID"]


class NeoMarioGalaxyAdapter(LMSAdapter):
    def __init__(self):
        super(NeoMarioGalaxyAdapter, self).__init__()
        self.charset = "utf-16-be"
        self.set_big_endian()

    def use_fixed_buckets(self):
        return True

    @property
    def supports_flows(self) -> bool:
        return True

    # ------------------------------------------------------------------------------------------------------------------

    def read_tag(self, stream: BinaryMemoryIO):
        tag_position = stream.tell()
        group_id = stream.read_u16()
        tag_id = stream.read_u16()
        len_data = stream.read_u16()

        # System
        if group_id == 0:
            if tag_id == 0:
                len_kanji = stream.read_u16()
                len_furigana = stream.read_u16()

                try:
                    furigana = stream.read(len_furigana).decode(self.charset)
                    kanji = stream.read(len_kanji).decode(self.charset)
                except:
                    raise LMSException(f"Couldn't decode ruby characters (tag at 0x{tag_position})")

                return f'[ruby:{kanji};{furigana}]'

            if tag_id == 3:
                if len_data != 2:
                    raise LMSException(f"Color tag length should be 2 (tag at 0x{tag_position})")

                color_id = stream.read_u16()

                if color_id == 0xFFFF:
                    return '[defcolor]'

                if color_id not in FontColor:
                    raise LMSException(f"Illegal Color tag color ID: {color_id} (tag at 0x{tag_position})")

                font_color = FontColor(color_id)

                return f'[color:{font_color.name.lower()}]'

        # Display
        elif group_id == 1:
            if tag_id == 0:
                if len_data != 2:
                    raise LMSException(f"Wait tag length should be 2 (tag at 0x{tag_position})")

                wait_time = stream.read_u16()
                return f'[delay:{wait_time}]'
            elif tag_id == 1:
                if len_data != 0:
                    raise LMSException(f"Page break tag length should be 0 (tag at 0x{tag_position})")

                return '[pagebreak]'
            elif tag_id == 2:
                if len_data != 0:
                    raise LMSException(f"Offset page tag length should be 0 (tag at 0x{tag_position})")

                return '[ycenter]'
            elif tag_id == 3:
                if len_data != 0:
                    raise LMSException(f"Center page tag length should be 0 (tag at 0x{tag_position})")

                return '[xcenter]'

        # Sound
        elif group_id == 2:
            if len_data < 2:
                raise LMSException(f"Minimum Sound tag length should be 2 (tag at 0x{tag_position})")

            len_sound = stream.read_u16()
            raw_sound = stream.read(len_sound)
            sound_name = raw_sound.decode(self.charset)

            return f'[sound:{sound_name}]'

        # Picture
        elif group_id == 3:
            if len_data != 2:
                raise LMSException(f"Picture group tag length should be 2 (tag at 0x{tag_position})")

            picture_offset = stream.read_u16()

            if tag_id not in PictureCode:
                raise LMSException(f"Illegal Picture group tag ID: {tag_id} (tag at 0x{tag_position})")

            picture_code = PictureCode(tag_id)

            return f'[icon:{picture_code.name.lower()}]'

        # FontSize
        elif group_id == 4:
            if len_data != 0:
                raise LMSException(f"FontSize group tag length should be 0 (tag at 0x{tag_position})")

            if tag_id not in FontSize:
                raise LMSException(f"Illegal FontSize group tag ID: {tag_id} (tag at 0x{tag_position})")

            font_size = FontSize(tag_id)

            return f'[size:{font_size.name.lower()}]'

        # Localize
        elif group_id == 5:
            if len_data != 2:
                raise LMSException(f"Localize group tag length should be 2 (tag at 0x{tag_position})")

            if tag_id == 0:
                preset_type = stream.read_u8()
                unknown_arg = stream.read_u8()
                return f'[player:{preset_type}]'

            raise LMSException(f"Illegal Localize group tag ID: {tag_id} (tag at 0x{tag_position})")

        # Number
        elif group_id == 6:
            if len_data != 8:
                raise LMSException(f"Number group tag length should be 8 (tag at 0x{tag_position})")

            default_value = stream.read_s32()
            va_arg_idx = stream.read_u32()
            return f'[intvar:{tag_id};{va_arg_idx};{default_value}]'

        # String
        elif group_id == 7:
            if len_data != 8:
                raise LMSException(f"String group tag length should be 8 (tag at 0x{tag_position})")

            unknown_arg_1 = stream.read_u32()
            va_arg_idx = stream.read_u32()
            return f'[stringvar:{tag_id};{va_arg_idx};0x{unknown_arg_1:08X}]'

        # RaceTime
        elif group_id == 9:
            if len_data != 0:
                raise LMSException(f"RaceTime group tag length should be 0 (tag at 0x{tag_position})")

            if tag_id not in RaceTime:
                raise LMSException(f"Illegal RaceTime group tag ID: {tag_id} (tag at 0x{tag_position})")

            race_time = RaceTime(tag_id)

            return f'[race:{race_time.name.lower()}]'

        # Font
        elif group_id == 10:
            if len_data < 2:
                raise LMSException(f"Minimum Sound tag length should be 2 (tag at 0x{tag_position})")

            len_sound = stream.read_u16()
            raw_sound = stream.read(len_sound)
            sound_name = raw_sound.decode(self.charset)

            return f'[numberfont:{sound_name}]'

        data = stream.read(len_data)
        return f"[{group_id}:{tag_id};{data.hex()}]"

    def write_tag(self, stream: BinaryMemoryIO, tag: str):
        if tag.find(":") >= 0:
            tag_name, tag_attrs = tag.split(":", 1)
            tag_name = tag_name.strip()
            tag_attrs = tag_attrs.strip().split(";")
        else:
            tag_name = tag.strip()
            tag_attrs = ()

        if tag_name == "":
            raise LMSException("Empty tag found!")

        # System
        if tag_name == "ruby":
            self.__verify_tag_attr_size(tag_name, tag_attrs, 2, tag)

            try:
                encoded_kanji = tag_attrs[0].encode(self.charset)
                encoded_furigana = tag_attrs[1].encode(self.charset)
            except LMSException:
                raise LMSException(f"Couldn't write ruby tag. Full tag was '{tag}'")

            data_size = 4 + len(encoded_furigana)

            self.__write_tag_info(stream, 0, 0, data_size)
            stream.write_u16(len(encoded_kanji))
            stream.write_u16(len(encoded_furigana))
            stream.write(encoded_furigana)
            stream.write(encoded_kanji)

        elif tag_name == "defcolor":
            self.__verify_tag_attr_size(tag_name, tag_attrs, 0, tag)
            self.__write_tag_info(stream, 0, 3, 2)
            stream.write_u16(0xFFFF)

        elif tag_name == "color":
            self.__verify_tag_attr_size(tag_name, tag_attrs, 1, tag)
            color_name = tag_attrs[0].upper()

            if color_name not in FontColor.__members__:
                raise LMSException(f"Invalid text color '{color_name}', full tag was '{tag}'")

            font_color = FontColor[color_name]
            color_id = font_color.value

            self.__write_tag_info(stream, 0, 3, 2)
            stream.write_u16(color_id)

        # Display
        elif tag_name == "delay":
            self.__verify_tag_attr_size(tag_name, tag_attrs, 1, tag)
            wait_time = self.__get_tag_attr_u16(tag_attrs[0], tag)

            self.__write_tag_info(stream, 1, 0, 2)
            stream.write_u16(wait_time)

        elif tag_name == "pagebreak":
            self.__verify_tag_attr_size(tag_name, tag_attrs, 0, tag)
            self.__write_tag_info(stream, 1, 1, 0)

        elif tag_name == "ycenter":
            self.__verify_tag_attr_size(tag_name, tag_attrs, 0, tag)
            self.__write_tag_info(stream, 1, 2, 0)

        elif tag_name == "xcenter":
            self.__verify_tag_attr_size(tag_name, tag_attrs, 0, tag)
            self.__write_tag_info(stream, 1, 3, 0)

        # Sound
        elif tag_name == "sound":
            self.__verify_tag_attr_size(tag_name, tag_attrs, 1, tag)
            sound_name = tag_attrs[0]
            encoded_name = sound_name.encode(self.charset)
            encoded_len = len(encoded_name)

            self.__write_tag_info(stream, 2, 0, 2 + encoded_len)
            stream.write_u16(encoded_len)
            stream.write(encoded_name)

        # Picture
        elif tag_name == "icon":
            self.__verify_tag_attr_size(tag_name, tag_attrs, 1, tag)
            icon_name = tag_attrs[0].upper()

            if icon_name not in PictureCode.__members__:
                raise LMSException(f"Invalid icon name '{icon_name}', full tag was '{tag}'")

            picture_code = PictureCode[icon_name]
            tag_id = picture_code.value
            picture_offset = picture_code.offset

            self.__write_tag_info(stream, 3, tag_id, 2)
            stream.write_u16(picture_offset)

        # FontSize
        elif tag_name == "size":
            self.__verify_tag_attr_size(tag_name, tag_attrs, 1, tag)
            size_name = tag_attrs[0].upper()

            if size_name not in FontSize.__members__:
                raise LMSException(f"Invalid font size '{size_name}', full tag was '{tag}'")

            font_size = FontSize[size_name]
            tag_id = font_size.value

            self.__write_tag_info(stream, 4, tag_id, 0)

        # Localize
        elif tag_name == "player":
            self.__verify_tag_attr_size(tag_name, tag_attrs, 1, tag)
            preset_type = self.__get_tag_attr_u8(tag_attrs[0], tag)

            self.__write_tag_info(stream, 5, 0, 2)
            stream.write_u8(preset_type)
            stream.write_u8(0xCD)

        # Number
        elif tag_name == "intvar":
            self.__verify_tag_attr_size(tag_name, tag_attrs, 3, tag)
            tag_id = self.__get_tag_attr_u16(tag_attrs[0], tag)
            va_arg_idx = self.__get_tag_attr_u32(tag_attrs[1], tag)
            def_val = self.__get_tag_attr_s32(tag_attrs[2], tag)

            self.__write_tag_info(stream, 6, tag_id, 8)
            stream.write_u32(def_val)
            stream.write_u32(va_arg_idx)

        # String
        elif tag_name == "stringvar":
            self.__verify_tag_attr_size(tag_name, tag_attrs, 3, tag)
            tag_id = self.__get_tag_attr_u16(tag_attrs[0], tag)
            va_arg_idx = self.__get_tag_attr_u32(tag_attrs[1], tag)
            def_ptr = self.__get_tag_attr_u32(tag_attrs[2], tag)

            self.__write_tag_info(stream, 7, tag_id, 8)
            stream.write_u32(def_ptr)
            stream.write_u32(va_arg_idx)

        # RaceTime
        elif tag_name == "race":
            self.__verify_tag_attr_size(tag_name, tag_attrs, 1, tag)
            race_name = tag_attrs[0].upper()

            if race_name not in RaceTime.__members__:
                raise LMSException(f"Invalid race name '{race_name}', full tag was '{tag}'")

            race_time = RaceTime[race_name]
            tag_id = race_time.value

            self.__write_tag_info(stream, 9, tag_id, 0)

        # Font
        elif tag_name == "numberfont":
            self.__verify_tag_attr_size(tag_name, tag_attrs, 1, tag)
            number_text = tag_attrs[0]

            encoded_text = number_text.encode(self.charset)
            encoded_len = len(encoded_text)

            self.__write_tag_info(stream, 10, 0, 2 + encoded_len)
            stream.write_u16(encoded_len)
            stream.write(encoded_text)

        else:
            self.__verify_tag_attr_size(tag_name, tag_attrs, 2, tag)
            tag_id = self.__get_tag_attr_u16(tag_name, tag)
            group_id = self.__get_tag_attr_u16(tag_attrs[0], tag)

            try:
                data = bytes.fromhex(tag_attrs[1])
                len_data = len(data)
            except ValueError:
                raise LMSException(f"Couldn't write arbitrary tag. Full tag was '{tag}'")

            if len_data & 1:
                self.__write_tag_info(stream, tag_id, group_id, len_data + 1)
                stream.write(data)
                stream.write_u8(0)
            else:
                self.__write_tag_info(stream, tag_id, group_id, len(data))
                stream.write(data)

    def __write_tag_info(self, stream: BinaryMemoryIO, group_id: int, tag_id: int, data_size: int):
        self.write_chars(stream, "\u000E")
        stream.write_u16(group_id)
        stream.write_u16(tag_id)
        stream.write_u16(data_size)

    def __verify_tag_attr_size(self, tag_name: str, tag_attrs: tuple[...], expected_size: int, tag: str):
        if len(tag_attrs) != expected_size:
            raise LMSException(f"Unexpected attributes count for tag '{tag_name}'. Expected {expected_size}, "
                               f"found {len(tag_attrs)}. Full tag was '{tag}'")

    def __get_tag_attr_u8(self, attr: str, tag: str) -> int:
        try:
            value = int(attr, 0)
        except Exception:
            raise LMSException(f"Couldn't parse tag attribute '{attr}' as integer. Full tag was '{tag}'")
        if value < 0 or 255 < value:
            raise LMSException(f"Tag attribute '{value}' out of range. Full tag was {tag}")
        return value

    def __get_tag_attr_u16(self, attr: str, tag: str) -> int:
        try:
            value = int(attr, 0)
        except Exception:
            raise LMSException(f"Couldn't parse tag attribute '{attr}' as integer. Full tag was '{tag}'")
        if value < 0 or 65535 < value:
            raise LMSException(f"Tag attribute '{value}' out of range. Full tag was {tag}")
        return value

    def __get_tag_attr_s32(self, attr: str, tag: str) -> int:
        try:
            value = int(attr, 0)
        except Exception:
            raise LMSException(f"Couldn't parse tag attribute '{attr}' as integer. Full tag was '{tag}'")
        if value < -0x80000000 or 0x7FFFFFFF < value:
            raise LMSException(f"Tag attribute '{value}' out of range. Full tag was {tag}")
        return value

    def __get_tag_attr_u32(self, attr: str, tag: str) -> int:
        try:
            value = int(attr, 0)
        except Exception:
            raise LMSException(f"Couldn't parse tag attribute '{attr}' as integer. Full tag was '{tag}'")
        if value < 0 or 0xFFFFFFFF < value:
            raise LMSException(f"Tag attribute '{value}' out of range. Full tag was {tag}")
        return value

    # ------------------------------------------------------------------------------------------------------------------

    @property
    def supports_attributes(self):
        return True

    @property
    def attributes_size(self) -> int:
        return 12

    def create_default_attributes(self) -> dict:
        return {
            "talk_type": TalkType.NORMAL,
            "balloon_type": BalloonType.WHITE_BUBBLE,
            "sound_id": SoundID.NONE,
            "camera_type": CameraType.NORMAL,
            "camera_id": 0,
            "msg_link_id": 255,
            "unk7": 255,
            "comment": ""
        }

    def parse_attributes(self, stream: BinaryMemoryIO, root_offset: int, root_size: int) -> dict:
        end_offset = root_offset + root_size

        # Read attributes
        sound_id     = SoundID(stream.read_u8())
        camera_type  = CameraType(stream.read_u8())
        talk_type    = TalkType(stream.read_u8())
        balloon_type = BalloonType(stream.read_u8())
        camera_id    = stream.read_u16()
        msg_link_id  = stream.read_u8()
        unk7         = stream.read_u8()
        off_comment  = stream.read_u32()

        # Read comment string
        stream.seek(root_offset + off_comment)
        comment = ""

        while root_offset < end_offset:
            ch = self.read_char(stream)

            if ch == "\0":
                break
            else:
                comment += ch

        # Pack and return result
        return {
            "talk_type": talk_type,
            "balloon_type": balloon_type,
            "sound_id": sound_id,
            "camera_type": camera_type,
            "camera_id": camera_id,
            "msg_link_id": msg_link_id,
            "unk7": unk7,
            "comment": comment
        }

    def write_attributes(self, stream: BinaryMemoryIO, attributes: dict):
        sound_id = attributes.get("sound_id", SoundID.NONE)
        camera_type = attributes.get("camera_type", CameraType.NORMAL)
        talk_type = attributes.get("talk_type", TalkType.NORMAL)
        balloon_type = attributes.get("balloon_type", BalloonType.WHITE_BUBBLE)
        camera_id = attributes.get("camera_id", 0)
        msg_link_id = attributes.get("msg_link_id", 255)
        unk7 = attributes.get("unk7", 255)
        comment = attributes.get("comment", "")

        stream.write_u8(sound_id.value)
        stream.write_u8(camera_type.value)
        stream.write_u8(talk_type.value)
        stream.write_u8(balloon_type.value)
        stream.write_u16(camera_id)
        stream.write_u8(msg_link_id)
        stream.write_u8(unk7)
        stream.write_u32(stream.size)

        stream.seek(0, io.SEEK_END)
        self.write_chars(stream, comment + "\0")


# ======================================================================================================================
# Tag enums
# ======================================================================================================================

class PictureCode(IntEnum):
    A_BUTTON             = 0
    B_BUTTON             = 1
    C_BUTTON             = 2
    WIIMOTE              = 3
    NUNCHUCK             = 4
    ONE_BUTTON           = 5
    TWO_BUTTON           = 6
    STAR                 = 7
    LAUNCH_STAR          = 8
    PULL_STAR            = 9
    POINTER              = 10
    PURPLE_STARBIT       = 11
    COCONUT              = 12
    ORANGE_ARROW         = 13
    STAR_BUNNY           = 14
    ANALOG_STICK         = 15
    X_MARK               = 16
    COIN                 = 17
    MARIO                = 18
    DPAD                 = 19
    BLUE_CHIP            = 20
    STAR_CHIP            = 21
    HOME_BUTTON          = 22
    MINUS_BUTTON         = 23
    PLUS_BUTTON          = 24
    Z_BUTTON             = 25
    SILVER_STAR          = 26
    GRAND_STAR           = 27
    LUIGI                = 28
    CO_POINTER           = 29
    PURPLE_COIN          = 30
    GREEN_COMET          = 31
    GOLD_CROWN           = 32
    CROSS_HAIR           = 33
    BLANK                = 34
    BOWSER               = 35
    HAND_GRAB            = 36
    HAND_POINT           = 37
    HAND_HOLD            = 38
    RAINBOW_STARBIT      = 39
    PEACH                = 40
    LETTER               = 41
    WHITE_QMARK          = 42
    CURRENT_PLAYER       = 43
    ONE_UP_MUSHROOM      = 49
    LIFE_MUSHROOM        = 50
    HUNGRY_LUMA          = 51
    LUMA                 = 52
    COMET                = 53
    GREEN_QMARK          = 54
    STOPWATCH            = 55
    MASTER_LUMA          = 56
    YOSHI                = 57
    COMET_MEDAL          = 58
    SILVER_CROWN         = 59
    YOSHI_GRAPPLE        = 60
    CHECKPOINT_FLAG      = 61
    EMPTY_STAR           = 62
    EMPTY_COMET_MEDAL    = 63
    EMPTY_COMET          = 64
    EMPTY_SECRET_STAR    = 65
    BRONZE_STAR          = 66
    BLIMP_FRUIT          = 67
    PLATINUM_CROWN       = 68
    BRONZE_GRAND_STAR    = 69
    TOPMAN               = 70
    GOOMBA               = 71
    COINS                = 72
    DPAD_UP              = 73
    DPAD_DOWN            = 74
    ORANGE_LUMA          = 75
    TOAD                 = 76
    BRONZE_COMET         = 77
    STAMP                = 78
    EMPTY_STAMP          = 79
    RED_STAR             = 80
    POLARI               = 81

    def __new__(cls, offset: int):
        value = len(cls.__members__)
        obj = int.__new__(cls, value)
        obj._value_ = value
        obj.offset = offset
        return obj


class FontColor(IntEnum):
    BLACK  = 0
    RED    = 1
    GREEN  = 2
    BLUE   = 3
    YELLOW = 4
    PURPLE = 5
    ORANGE = 6
    GRAY   = 7


class FontSize(IntEnum):
    SMALL  = 0
    NORMAL = 1
    LARGE  = 2


class RaceTime(IntEnum):
    JUNGLE_GLIDER    = 0
    CHALLENGE_GLIDER = 1
    LAST             = 5


# ======================================================================================================================
# Attribute enums
# ======================================================================================================================

class TalkType(IntEnum):
    NORMAL = 0
    SHOUT  = 1
    AUTO   = 2
    GLOBAL = 3


class BalloonType(IntEnum):
    WHITE_BUBBLE      = 0
    WHITE_BUBBLE_2    = 1
    WHITE_BUBBLE_NO_A = 2
    SIGNBOARD         = 3
    ICON_BUBBLE       = 4
    STORY_BOOK_BOTTOM = 5
    STORY_BOOK_CENTER = 6
    STORY_BOOK_TOP    = 7
    UNKNOWN_8         = 8
    LIBRARY_BOOK      = 9


class CameraType(IntEnum):
    NORMAL = 0
    EVENT  = 1
    NONE   = 2


class SoundID(IntEnum):
    NULL                             = 0
    NONE                             = 1
    SE_SV_KINOPIO_TALK_HEY           = 2
    SE_SV_KINOPIO_TALK_YAHOO         = 3
    SE_SV_KINOPIO_TALK_ANGRY         = 4
    SE_SV_KINOPIO_TALK_SAD           = 5
    SE_SV_KINOPIO_TALK_HAPPY         = 6
    SE_SV_KINOPIO_TALK_SLEEP         = 7
    SE_SV_KINOPIO_TALK_WELCOME       = 8
    SE_SV_KINOPIO_TALK_BEAUTIFUL     = 9
    SE_SV_KINOPIO_TALK_SURPRISE      = 10
    SE_SV_KINOPIO_PUHA               = 11
    SE_SV_KINOPIO_TALK_HELP          = 12
    SE_SV_KINOPIO_TALK_TREMBLE       = 13
    SE_SV_KINOPIO_TALK_STRONG        = 14
    SE_SV_KINOPIO_TALK_LOOK_OUT      = 15
    SE_SV_KINOPIO_TALK_WOW           = 16
    SE_SV_KINOPIO_TALK_WATER         = 17
    SE_SV_KINOPIO_TALK_HEY_SNOR      = 18
    SE_SV_KINOPIO_TALK_SAD_SNOR      = 19
    SE_SV_KINOPIO_NO_MAIL            = 20
    SE_SV_KINOPIO_LOOK_MAIL          = 21
    SE_SV_KINOPIO_TALK_SHOUT         = 22
    SE_SV_KINOPIO_TALK_TIRED         = 23
    SE_SV_KINOPIO_TALK_JOYFUL        = 24
    SE_SV_RABBIT_TALK_NORMAL         = 25
    SE_SV_RABBIT_TALK_CAUGHT         = 26
    SE_SV_RABBIT_TALK_THATS          = 27
    SE_SV_RABBIT_TALK_HELP           = 28
    SE_SV_RABBIT_TALK_THANKS         = 29
    SE_SV_PENGUIN_L_TALK_NORMAL      = 30
    SE_SV_PENGUIN_L_TALK_PLEASED     = 31
    SE_SV_PENGUIN_L_TALK_NG          = 32
    SE_SV_PENGUIN_L_TALK_QUESTION    = 33
    SE_SV_PENGUIN_L_TALK_DISTANT     = 34
    SE_SV_PENGUIN_L_TALK_NORMAL_L    = 35
    SE_SV_PENGUIN_L_TALK_PLEASED_L   = 36
    SE_SV_PENGUIN_L_TALK_NG_L        = 37
    SE_SV_PENGUIN_L_TALK_QUESTION_L  = 38
    SE_SV_PENGUIN_L_TALK_OH          = 39
    SE_SV_PENGUIN_S_TALK_NORMAL      = 40
    SE_SV_PENGUIN_S_TALK_GLAD        = 41
    SE_SV_PENGUIN_S_TALK_GLAD_HIGH   = 42
    SE_SV_PENGUIN_S_TALK_ANGRY       = 43
    SE_SV_PENGUIN_S_TALK_SAD         = 44
    SE_SV_PENGUIN_S_TALK_HAPPY       = 45
    SE_SV_PENGUIN_S_TALK_STRONG      = 46
    SE_SV_PENGUIN_S_TALK_NORMAL_W    = 47
    SE_SV_PENGUIN_S_TALK_GREET       = 48
    SE_SV_PENGUIN_S_TALK_WIN         = 49
    SE_SV_PENGUIN_S_TALK_LOSE        = 50
    SE_SV_PENGUIN_S_TALK_OUCH        = 51
    SE_SV_PENGUIN_ACE_TALK_NORMAL    = 52
    SE_SV_PENGUIN_ACE_TALK_GREET     = 53
    SE_SV_PENGUIN_ACE_TALK_WIN       = 54
    SE_SV_PENGUIN_ACE_TALK_LOSE      = 55
    SE_SV_PENGUIN_SS_HAPPY           = 56
    SE_SV_PENGUIN_SS_GREET           = 57
    SE_SV_PENGUIN_SS_DAMAGE          = 58
    SE_SV_PENGUIN_SS_DISAPPOINTED    = 59
    SE_SV_PENGUIN_SS_PLEASED         = 60
    SE_SV_PENGUIN_SS_ANGRY           = 61
    SE_SV_HONEYBEE_TALK_NORMAL       = 62
    SE_SV_HONEYBEE_TALK_CONFUSION    = 63
    SE_SV_HONEYBEE_TALK_QUESTION     = 64
    SE_SV_HONEYBEE_TALK_SURPRISE     = 65
    SE_SV_HONEYBEE_TALK_ORDER        = 66
    SE_SV_HONEYBEE_TALK_LAUGH        = 67
    SE_SV_HONEYBEE_TALK_GLAD         = 68
    SE_SV_HONEYBEE_TALK_SLEEP        = 69
    SE_SV_TICO_TALK_NORMAL           = 70
    SE_SV_TICO_TALK_GLAD             = 71
    SE_SV_TICO_TALK_ANGRY            = 72
    SE_SV_TICO_TALK_SAD              = 73
    SE_SV_TICO_TALK_HAPPY            = 74
    SE_SV_TICO_TICO                  = 75
    SE_SV_TICO_TALK_CONFUSION        = 76
    SE_SV_TICO_TALK_THANKS           = 77
    SE_SV_TICO_TALK_DIST_HALF_GLAD   = 78
    SE_SV_TICO_TALK_DIST_HALF_NORMAL = 79
    # dummy                          = 80
    SE_SV_LUIGI_FRIGHTENED           = 81
    # dummy                          = 82
    # dummy                          = 83
    SE_SV_LUIGI_HEY                  = 84
    SE_DM_KINOPIO_CHIEF              = 85
    SE_SV_CARETAKER_SHORT            = 86
    SE_SV_CARETAKER_NORMAL           = 87
    SE_SV_CARETAKER_LONG             = 88
    SE_SV_CARETAKER_REPEAT           = 89
    SE_SV_PEACH_TALK_HELP            = 90
    SE_SV_ROSETTA_TALK_NORMAL        = 91
    # dummy                          = 92
    # dummy                          = 93
    # dummy                          = 94
    # dummy                          = 95
    SE_SV_TICOFAT_TALK_NORMAL        = 96
    SE_SV_TICOFAT_TALK_KITA          = 97
    SE_SV_TICOFAT_META               = 98
    SE_SV_KINOPIOCHIEF_TALK_HEY      = 99
    SE_SV_KINOPIOCHIEF_TALK_LAUGH    = 100
    SE_SV_KINOPIOCHIEF_TALK_YAHOO    = 101
    SE_SV_TICOFAT_TALK_GIVE_ME       = 102
    SE_SV_TICOFAT_TALK_WAKU          = 103
    SE_SV_BUTLER_TALK_SURPRISE       = 104
    SE_SV_BUTLER_TALK_AGREE          = 105
    SE_SV_BUTLER_TALK_WORRIED        = 106
    SE_SV_BUTLER_TALK_NORMAL         = 107
    SE_SV_PENGUIN_OLD_GREET          = 108
    SE_SV_PENGUIN_OLD_GRAD           = 109
    SE_SV_PENGUIN_OLD_SAD            = 110
    SE_SV_PENGUIN_OLD_NORMAL         = 111
    SE_SV_LUIGI_TALK_TIRE            = 112
    SE_SV_LUIGI_TALK_YAH             = 113
    # dummy                          = 114
    SE_SV_LUIGI_TALK_OH_YEAH         = 115
    SE_SV_BUTLER_TALK_QUESTION       = 116
    # dummy                          = 117
    # dummy                          = 118
    SE_SV_PENGUIN_OLD_SCARED         = 119
    SE_SV_TICOCOMET_TALK_PURURIN     = 120
    SE_SV_TICOCOMET_TALK_DON         = 121
    SE_SV_TICOSHOP_TALK_PIKARIN      = 122
    SE_SV_TICOSHOP_TALK_KITA         = 123
    SE_BV_KOOPAJR_TLK_PROVOKE        = 124
    SE_BV_KOOPA_TLK_LAUGH            = 125
    SE_BV_KOOPA_TLK_NORMAL           = 126
    SE_BV_KOOPA_TLK_REGRET           = 127
    SE_BV_KOOPA_TLK_CALM             = 128
    SE_BV_KOOPA_TLK_EXCITED          = 129
    SE_SV_TICOFAT_TALK_YEAH          = 130
    # dummy                          = 131
    SE_SV_CARE_TAKER_TRAMPLE         = 132
    SE_SV_KINOPIOCHIEF_TALK_EVASIVE  = 133
    # dummy                          = 134
    # dummy                          = 135
    SE_SV_SIGNBOARD_HEY              = 136
    # dummy                          = 137
    # dummy                          = 138
    # dummy                          = 139
    # dummy                          = 140
    # dummy                          = 141
    # dummy                          = 142
    SE_SV_CARETAKER_ANGRY_FAST       = 143
    SE_SV_HONEYQUEEN_TALK_SURPRISE   = 144
    SE_SV_HONEYQUEEN_TALK_THANKS     = 145
    SE_SV_HONEYQUEEN_TALK_WORRY      = 146
    SE_SV_HONEYQUEEN_TALK_AA         = 147
    SE_SV_HONEYQUEEN_TALK_AN         = 148
    SE_SV_HONEYQUEEN_TALK_UFUFU      = 149
    SE_SV_TICOBIG_TALK_NORMAL        = 150
    SE_SV_TICOBIG_TALK_GLAD          = 151
    SE_SV_TICOBIG_TRAMPLED           = 152
    SE_SV_PICHAN_TALK_NORMAL         = 153
    SE_SV_PICHAN_TALK_GLAD           = 154
    SE_SV_PICHAN_TALK_ANGRY          = 155
    SE_SV_PICHAN_TALK_SAD            = 156
    SE_SV_PICHAN_TALK_HAPPY          = 157
    SE_SV_PICHAN_TALK_DIFFICULT      = 158
    SE_SV_BOMBHEI_RED_TALK_NORMAL    = 159
    SE_SV_BOMBHEI_RED_TALK_SHORT     = 160
    SE_SV_MONTE_TALK_NORMAL          = 161
    SE_SV_MONTE_TALK_ANGRY           = 162
    SE_SV_MONTE_TALK_SURPRISE        = 163
    SE_SV_MONTE_TALK_PROUD           = 164
    SE_SV_MONTE_TALK_WELCOME         = 165
    SE_SV_MONTE_TALK_QUESTION        = 166
    SE_SV_MEISTER_TALK_NORMAL        = 167
    SE_SV_MEISTER_TALK_QUESTION      = 168
    SE_SV_MEISTER_TALK_NOT_SEEN      = 169
    SE_SV_MEISTER_TALK_BAD           = 170
    SE_SV_MEISTER_TALK_STRONG        = 171
    SE_SV_MEISTER_TALK_ALL_RIGHT     = 172
    SE_SV_MEISTER_TALK_BOAST         = 173
    SE_SV_MEISTER_TALK_UN            = 174
    SE_SV_MEISTER_TALK_HAHAN         = 175
    SE_SV_MEISTER_TALK_EAT1          = 176
    SE_SV_MEISTER_TALK_EAT2          = 177
    SE_SV_SASURAI_TALK_NORMAL        = 178
    SE_SV_SASURAI_TALK_QUESTION      = 179
    SE_SV_SASURAI_TALK_ALL_RIGHT     = 180
    SE_SV_SASURAI_TALK_DO_BEST       = 181
    SE_SV_SASURAI_TALK_EXCELLENT     = 182
    SE_SV_SASURAI_TALK_REGRET        = 183
    SE_BV_BATTANKING_TALK_EIO        = 184
    SE_BV_BATTANKING_TALK_OI         = 185
    SE_SV_HELPERWITCH_TALK_SMILE     = 186
    SE_SV_HELPERWITCH_TALK_SMILE_2   = 187
    SE_SV_HELPERWITCH_TALK_LOOK      = 188
    SE_SV_HELPERWITCH_TALK_AHA       = 189
    SE_SV_HINT_TV_TALK_SUGGEST       = 190
    SE_SV_HINT_TV_TALK_OK            = 191
    SE_SV_PEACH_NPC_THANK_YOU        = 192
