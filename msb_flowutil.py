from pymsb import LMSMessageNode, LMSEventNode, LMSBranchNode

__all__ = [
    "new_message",
    "new_event_talk_func", "new_event_talk_func_no_advance", "new_event_advance_node", "new_event_anime_func",
    "new_event_on_switch_a", "new_event_on_switch_b", "new_event_kill_func", "new_event_off_switch_a",
    "new_event_off_switch_b", "new_event_hide_bubble_beak", "new_event_show_bubble_beak",
    "new_branch_yes_no_result", "new_branch_talk_func", "new_branch_near_player_anytime", "new_branch_on_switch_a",
    "new_branch_on_switch_b", "new_branch_player_element_mode_normal", "new_branch_player_element_mode_bee",
    "new_branch_player_element_mode_teresa", "new_branch_power_star_appeared", "new_branch_player_luigi",
    "new_branch_time_keep_demo_active", "new_branch_message_already_read", "new_branch_ending_120_stars",
    "new_branch_already_done_flag", "new_branch_player_element_mode_yoshi", "new_branch_player_element_mode_cloud",
    "new_branch_player_element_mode_rock"
]


# ======================================================================================================================
# Message nodes
# ======================================================================================================================

def new_message(label: str) -> LMSMessageNode:
    node = LMSMessageNode()
    node.message_label = label
    return node


# ======================================================================================================================
# Event nodes
# ======================================================================================================================

def new_event_talk_func(param: int) -> LMSEventNode:
    return __create_event_node(0, param)


def new_event_talk_func_no_advance(param: int) -> LMSEventNode:
    return __create_event_node(1, param)


def new_event_advance_node() -> LMSEventNode:
    return __create_event_node(3, 0)


def new_event_anime_func(param: int) -> LMSEventNode:
    return __create_event_node(4, param)


def new_event_on_switch_a() -> LMSEventNode:
    return __create_event_node(5, 0)


def new_event_on_switch_b() -> LMSEventNode:
    return __create_event_node(6, 0)


def new_event_kill_func(param: int) -> LMSEventNode:
    return __create_event_node(7, param)


def new_event_off_switch_a() -> LMSEventNode:
    return __create_event_node(8, 0)


def new_event_off_switch_b() -> LMSEventNode:
    return __create_event_node(9, 0)


def new_event_hide_bubble_beak() -> LMSEventNode:
    return __create_event_node(10, 0)


def new_event_show_bubble_beak() -> LMSEventNode:
    return __create_event_node(11, 0)


# ======================================================================================================================
# Branch nodes
# ======================================================================================================================

def new_branch_yes_no_result(options: int) -> LMSBranchNode:
    return __create_branch_node(0, options)


def new_branch_talk_func(param: int) -> LMSBranchNode:
    return __create_branch_node(1, param)


def new_branch_near_player_anytime() -> LMSBranchNode:
    return __create_branch_node(2, 0)


def new_branch_on_switch_a() -> LMSBranchNode:
    return __create_branch_node(3, 0)


def new_branch_on_switch_b() -> LMSBranchNode:
    return __create_branch_node(4, 0)


def new_branch_player_element_mode_normal() -> LMSBranchNode:
    return __create_branch_node(5, 0)


def new_branch_player_element_mode_bee() -> LMSBranchNode:
    return __create_branch_node(6, 0)


def new_branch_player_element_mode_teresa() -> LMSBranchNode:
    return __create_branch_node(7, 0)


def new_branch_power_star_appeared() -> LMSBranchNode:
    return __create_branch_node(8, 0)


def new_branch_player_luigi() -> LMSBranchNode:
    return __create_branch_node(9, 0)


def new_branch_time_keep_demo_active() -> LMSBranchNode:
    return __create_branch_node(10, 0)


def new_branch_message_already_read() -> LMSBranchNode:
    return __create_branch_node(11, 0)


def new_branch_ending_120_stars() -> LMSBranchNode:
    return __create_branch_node(12, 0)


def new_branch_already_done_flag() -> LMSBranchNode:
    return __create_branch_node(13, 0)


def new_branch_player_element_mode_yoshi() -> LMSBranchNode:
    return __create_branch_node(14, 0)


def new_branch_player_element_mode_cloud() -> LMSBranchNode:
    return __create_branch_node(15, 0)


def new_branch_player_element_mode_rock() -> LMSBranchNode:
    return __create_branch_node(16, 0)


# ======================================================================================================================
# Utility
# ======================================================================================================================

def __create_event_node(type: int, param: int) -> LMSEventNode:
    node = LMSEventNode()
    node.event_type = type
    node.event_param = param
    return node


def __create_branch_node(type: int, param: int) -> LMSBranchNode:
    node = LMSBranchNode()
    node.condition_type = type
    node.condition_param = param
    return node
