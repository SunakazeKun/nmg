#include "Game/Util/BgmUtil.h"
#include "Game/Util/EventUtil.h"
#include "kamek/hooks.h"

namespace {
    // =================================================================================================================
    // Disable introduction after IslandFleetGalaxy's first Power Star
    // =================================================================================================================

#if defined(SB4_JEP)
    kmWrite32(0x804B4E08, PPC_LI(3, 0));
    kmWrite32(0x804B52D4, PPC_LI(3, 0));
    kmWrite32(0x804B5720, PPC_LI(3, 0));
    kmWrite32(0x804B58F8, PPC_LI(3, 0));
    kmWrite32(0x804D66F0, PPC_LI(3, 0));
#elif defined(SB4_WK)
    kmWrite32(0x804B4E78, PPC_LI(3, 0));
    kmWrite32(0x804B5344, PPC_LI(3, 0));
    kmWrite32(0x804B5790, PPC_LI(3, 0));
    kmWrite32(0x804B5968, PPC_LI(3, 0));
    kmWrite32(0x804D6780, PPC_LI(3, 0));
#endif


    // =================================================================================================================
    // Starship Mario music sequence
    // =================================================================================================================

    bool startMarioFaceShipBGM(int bgmType) {
        s32 numStars = MR::getPowerStarNum();

        if (numStars < 15) {
            return MR::startStageBGMIfNotPlaying("MBGM_SMG2_MARIO_SHIP01", false);
        }
        else if (numStars < 30) {
            return MR::startStageBGMIfNotPlaying("MBGM_SMG2_MARIO_SHIP02", false);
        }
        else {
            return MR::startStageBGMIfNotPlaying("MBGM_SMG2_MARIO_SHIP03", false);
        }

        return false;
    }

#if defined(SB4_JEP)
    kmCall(0x804B49B0, startMarioFaceShipBGM);
    kmCall(0x804B5378, startMarioFaceShipBGM);
    kmCall(0x804B5BAC, startMarioFaceShipBGM);
#elif defined(SB4_WK)
    kmCall(0x804B4A20, startMarioFaceShipBGM);
    kmCall(0x804B53E8, startMarioFaceShipBGM);
    kmCall(0x804B5C1C, startMarioFaceShipBGM);
#endif


    // =================================================================================================================
    // Disable Postman Toad appearance
    // =================================================================================================================

    kmWrite32(0x8034F620, PPC_LI(3, 0));
    kmWrite32(0x8034FA74, PPC_LI(3, 0));
}
