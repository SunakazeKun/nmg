#pragma once

#include "revolution.h"


/*********************************************************************************************************************/
/* Recurring instructions                                                                                            */
/*********************************************************************************************************************/
#define NOP 0x60000000
#define BLR 0x4E800020
#define LI_R3_0 0x38600000
#define LI_R3_1 0x38600001


/*********************************************************************************************************************/
/* Friendly stage names                                                                                              */
/*********************************************************************************************************************/
#define STAGE_GALACTIC_GARDEN_GALAXY ("IslandFleetGalaxy")
#define STAGE_SWEET_TREAT_GALAXY ("YosshiHomeGalaxy")
#define STAGE_GLOOMY_GALLEON_GALAXY ("DigMineGalaxy")
#define STAGE_BATTLE_BLAST_GALAXY ("MokumokuValleyGalaxy")
#define STAGE_TURNLOG_GALAXY ("RedBlueExGalaxy")
#define STAGE_EEP_BEEP_GALAXY ("AbekobeGalaxy")
#define STAGE_KRAKEN_KING_GALAXY ("VsKoopaJrLv1Galaxy")
#define STAGE_COMET_OBSERVATORY ("AstroGalaxy")


/*********************************************************************************************************************/
/* Game event flag & value names                                                                                     */
/*********************************************************************************************************************/
#define GAME_EVENT_FLAG_EXPLAIN_ILLUST_ITEM_AT_FIRST ("ExplainIllustItemAtFirst")
#define GAME_EVENT_FLAG_DAREDEVIL_MODE_ON ("DaredevilModeOn")
#define GAME_EVENT_FLAG_COMET_EXPLANATION ("コメット解説")

#define GAME_EVENT_VALUE_HIGHSCORE_SWEET_TREAT_LO ("ベストスコア[YosshiHomeGalaxy]/lo")
#define GAME_EVENT_VALUE_HIGHSCORE_SWEET_TREAT_HI ("ベストスコア[YosshiHomeGalaxy]/hi")


/*********************************************************************************************************************/
/* Game flow parameters                                                                                              */
/*********************************************************************************************************************/
#define GAME_FLOW_TOTAL_NORMAL_STARS 21
#define GAME_FLOW_TOTAL_GREEN_STARS 21
#define GAME_FLOW_TOTAL_POWER_STARS (GAME_FLOW_TOTAL_NORMAL_STARS + GAME_FLOW_TOTAL_GREEN_STARS)
#define GAME_FLOW_TOTAL_COMET_MEDALS 8
#define GAME_FLOW_TOTAL_STAMPS 8

#define GAME_FLOW_POWER_STARS_FOR_COMETS 5
#define GAME_FLOW_POWER_STARS_FOR_GREEN_STARS 21
