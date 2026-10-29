from pymsb import LMSFlows
from msb_defines import *
from msb_flowutil import *
from typing import Callable

__all__ = ["FlowchartGenerator", "has_flowchart_generator", "get_flowchart_generator"]


# ======================================================================================================================
# Generator registry
# ======================================================================================================================

FlowchartGenerator = Callable[[LMSFlows], None]

__FLOWCHART_GENERATORS: dict[tuple[str, str], FlowchartGenerator] = {}


def has_flowchart_generator(archive_name: str, msb_name: str) -> bool:
    return (archive_name, msb_name) in __FLOWCHART_GENERATORS


def get_flowchart_generator(archive_name: str, msb_name: str) -> FlowchartGenerator:
    return __FLOWCHART_GENERATORS[(archive_name, msb_name)]


def register_flowchart_generator(archive_name: str, msb_name: str):
    def inner(generator_func: FlowchartGenerator):
        __FLOWCHART_GENERATORS[(archive_name, msb_name)] = generator_func
        return generator_func
    return inner


# ======================================================================================================================
# Generator registry
# ======================================================================================================================

@register_flowchart_generator("AstroGalaxy", "AstroGalaxy")
def flows_AstroGalaxy(flows: LMSFlows):
    # Rosetta000
    entry = flows.new_flowchart("Rosetta000")

    rosetta000_flow000 = new_message("Rosetta000_Flow000")
    rosetta000_flow001 = new_message("Rosetta000_Flow001")
    event_on_switch_a = new_event_on_switch_a()

    entry.next_node = rosetta000_flow000
    rosetta000_flow000.next_node = event_on_switch_a
    event_on_switch_a.next_node = rosetta000_flow001
    rosetta000_flow001.next_node = rosetta000_flow001

    # ------------------------------------------------------

    # Tico001
    entry = flows.new_flowchart("Tico001")

    tico001_flow000 = new_message("Tico001_Flow000")
    tico001_flow001 = new_message("Tico001_Flow001")
    tico001_flow002 = new_message("Tico001_Flow002")

    branch_power_star_appeared_1 = new_branch_power_star_appeared()
    branch_power_star_appeared_2 = new_branch_power_star_appeared()
    branch_power_star_appeared_3 = new_branch_power_star_appeared()

    entry.next_node = branch_power_star_appeared_1

    branch_power_star_appeared_1.next_node = tico001_flow002
    branch_power_star_appeared_1.next_node_else = tico001_flow000
    tico001_flow000.next_node = branch_power_star_appeared_2

    branch_power_star_appeared_2.next_node = tico001_flow002
    branch_power_star_appeared_2.next_node_else = tico001_flow001
    tico001_flow001.next_node = branch_power_star_appeared_3

    branch_power_star_appeared_3.next_node = tico001_flow002
    branch_power_star_appeared_3.next_node_else = tico001_flow001
    tico001_flow002.next_node = tico001_flow002

    # ------------------------------------------------------

    # TicoManiac000
    entry = flows.new_flowchart("TicoManiac000")

    tico_maniac000_flow000 = new_message("TicoManiac000_Flow000")
    event_metamorphosis = new_event_kill_func(TicoManiacKillFunc.METAMORPHOSIS)

    entry.next_node = tico_maniac000_flow000
    tico_maniac000_flow000.next_node = event_metamorphosis
    event_metamorphosis.next_node = None


@register_flowchart_generator("IntroRosettaZone", "IntroRosettaZone")
def flows_IntroRosettaZone(flows: LMSFlows):
    # HelperWitch000
    entry = flows.new_flowchart("HelperWitch000")

    helper_witch000_flow000 = new_message("HelperWitch000_Flow000")
    event_msg_arg_player = new_event_talk_func(HelperWitchEventFunc.MSG_ARG_PLAYER)
    branch_reset_before = new_branch_talk_func(HelperWitchBranchFunc.RESET_BEFORE)
    branch_yes_no_player = new_branch_yes_no_result(YesNoOptions.HELPER_WITCH_PLAYER)
    event_set_player_luigi = new_event_talk_func(HelperWitchEventFunc.SET_PLAYER_LUIGI)
    event_set_player_mario = new_event_talk_func(HelperWitchEventFunc.SET_PLAYER_MARIO)

    helper_witch000_flow001 = new_message("HelperWitch000_Flow001")
    event_msg_arg_daredevil = new_event_talk_func(HelperWitchEventFunc.MSG_ARG_DAREDEVIL)
    branch_yes_no_daredevil = new_branch_yes_no_result(YesNoOptions.HELPER_WITCH_DAREDEVIL)
    event_daredevil_mode_on = new_event_talk_func(HelperWitchEventFunc.DAREDEVIL_MODE_ON)
    event_daredevil_mode_off = new_event_talk_func(HelperWitchEventFunc.DAREDEVIL_MODE_OFF)

    helper_witch000_flow002 = new_message("HelperWitch000_Flow002")
    helper_witch000_flow002_for_reset = new_message("HelperWitch000_Flow002")
    branch_should_reset = new_branch_talk_func(HelperWitchBranchFunc.SHOULD_RESET)
    event_reset_scene = new_event_talk_func(HelperWitchEventFunc.RESET_SCENE)

    helper_witch000_flow003 = new_message("HelperWitch000_Flow003")
    helper_witch000_flow004 = new_message("HelperWitch000_Flow004")
    branch_yes_no_settings = new_branch_yes_no_result(YesNoOptions.SUPER_DREAMER)
    event_msg_arg_player_2 = new_event_talk_func(HelperWitchEventFunc.MSG_ARG_PLAYER)

    entry.next_node = event_msg_arg_player
    event_msg_arg_player.next_node = branch_reset_before
    branch_reset_before.next_node = helper_witch000_flow003
    branch_reset_before.next_node_else = helper_witch000_flow000
    helper_witch000_flow000.next_node = branch_yes_no_player

    helper_witch000_flow003.next_node = branch_yes_no_settings
    branch_yes_no_settings.next_node = event_msg_arg_player_2
    branch_yes_no_settings.next_node_else = helper_witch000_flow002
    event_msg_arg_player_2.next_node = helper_witch000_flow004
    helper_witch000_flow004.next_node = branch_yes_no_player

    branch_yes_no_player.next_node = event_set_player_luigi
    branch_yes_no_player.next_node_else = event_set_player_mario
    event_set_player_luigi.next_node = event_msg_arg_daredevil
    event_set_player_mario.next_node = event_msg_arg_daredevil
    event_msg_arg_daredevil.next_node = helper_witch000_flow001
    helper_witch000_flow001.next_node = branch_yes_no_daredevil
    branch_yes_no_daredevil.next_node = event_daredevil_mode_on
    branch_yes_no_daredevil.next_node_else = event_daredevil_mode_off
    event_daredevil_mode_on.next_node = branch_should_reset
    event_daredevil_mode_off.next_node = branch_should_reset
    branch_should_reset.next_node = helper_witch000_flow002_for_reset
    branch_should_reset.next_node_else = helper_witch000_flow002
    helper_witch000_flow002_for_reset.next_node = event_reset_scene
    helper_witch000_flow002.next_node = helper_witch000_flow003
    event_reset_scene.next_node = None


@register_flowchart_generator("RedBlueExGalaxy", "RedBlueExGalaxy")
def flows_RedBlueExGalaxy(flows: LMSFlows):
    # Moc000
    entry = flows.new_flowchart("Moc000")

    moc000_flow000 = new_message("Moc000_Flow000")
    moc000_flow001 = new_message("Moc000_Flow001")
    event_appear_power_star = new_event_talk_func(MocEventFunc.APPEAR_POWER_STAR)

    entry.next_node = moc000_flow000
    moc000_flow000.next_node = event_appear_power_star
    event_appear_power_star.next_node = moc000_flow001
    moc000_flow001.next_node = moc000_flow001


@register_flowchart_generator("UnderwaterCaveZone", "UnderwaterCaveZone")
def flows_UnderwaterCaveZone(flows: LMSFlows):
    # PenguinCoach000
    entry = flows.new_flowchart("PenguinCoach000")

    penguin_coach000_flow000 = new_message("PenguinCoach000_Flow000")
    penguin_coach000_flow001 = new_message("PenguinCoach000_Flow001")
    penguin_coach000_flow002 = new_message("PenguinCoach000_Flow002")
    branch_power_star_appeared = new_branch_power_star_appeared()

    entry.next_node = penguin_coach000_flow000
    penguin_coach000_flow000.next_node = penguin_coach000_flow001
    penguin_coach000_flow001.next_node = branch_power_star_appeared
    branch_power_star_appeared.next_node = penguin_coach000_flow002
    branch_power_star_appeared.next_node_else = penguin_coach000_flow001
    penguin_coach000_flow002.next_node = penguin_coach000_flow002

    # Kinopio000
    entry = flows.new_flowchart("Kinopio000")

    kinopio000_flow000 = new_message("Kinopio000_Flow000")
    kinopio000_flow001 = new_message("Kinopio000_Flow001")

    entry.next_node = kinopio000_flow000
    kinopio000_flow000.next_node = kinopio000_flow001
    kinopio000_flow001.next_node = kinopio000_flow001
