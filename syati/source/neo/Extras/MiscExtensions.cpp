#include "Game/Util/ActorSensorUtil.h"
#include "Game/Util/LiveActorUtil.h"
#include "Game/Util/SceneUtil.h"
#include "kamek/hooks.h"
#include "neo/core.h"

namespace {
    // =================================================================================================================
    // Don't disable IslandFleetGalaxy's intro camera
    // =================================================================================================================

    kmWrite32(0x80142C34, LI_R3_0);
    kmWrite32(0x80451804, LI_R3_0);


    // ==================================================================================================================
    // Skippable intro after collecting a Power Star
    // ==================================================================================================================

    // GameDataFunction::setPowerStar
#if defined(SB4P) || defined(SB4E) || defined(SB4J)
    kmWrite32(0x804D254C, 0x60000005);
#elif defined(SB4W) || defined(SB4K)
    kmWrite32(0x804D25DC, 0x60000005);
#endif


    // ==================================================================================================================
    // Increase Coin receiver sensors count
    // ==================================================================================================================

    // Coin::init
    kmWrite32(0x8028BDC4, 0x38C00008);


    // ==================================================================================================================
    // Adjust Warp Pipe clipping range
    // ==================================================================================================================

    bool extSetEarthenPipeClipping(const char* origStageCheck) {
        register LiveActor* earthenPipe;
        __asm {
            mr earthenPipe, r28
        }

        if (MR::isEqualStageName(STAGE_GLOOMY_GALLEON_GALAXY)) {
            MR::setClippingTypeSphere(earthenPipe, 2000.0f);
            return false;
        }
        else {
            return MR::isEqualStageName(origStageCheck);
        }
    }

    // EarthenPipe::init
    kmCall(0x80296F90, extSetEarthenPipeClipping);


    // ==================================================================================================================
    // Message responses
    // ==================================================================================================================
    
    // Petari
    bool extPetariCheckPlayerAttacks(u32 msg) {
        return MR::isMsgFreezeAttack(msg) || MR::isMsgInvincibleAttack(msg);
    }

    kmCall(0x801E9B80, extPetariCheckPlayerAttacks);


    // Pukupuku
    bool extPukupukuCheckPlayerAttacks(u32 msg) {
        return MR::isMsgYoshiProjectile(msg) || MR::isMsgInvincibleAttack(msg);
    }

    kmCall(0x801F17F4, extPukupukuCheckPlayerAttacks);


    // ==================================================================================================================
    // Fix Rocto hide behavior
    // ==================================================================================================================

    // OtaRock::tryToHide
    kmWrite32(0x801DCBB4, 0x40820014);


    // ==================================================================================================================
    // Cosmic Spirit also appears when playing as Luigi
    // ==================================================================================================================

    // SuperDreamer::init
    kmWrite32(0x80360D54, LI_R3_0);

    // GameSequenceFunction::isAllowDreamerInStage
#if defined(SB4P) || defined(SB4E) || defined(SB4J)
    kmWrite32(0x804D7EAC, LI_R3_0);
#elif defined(SB4W) || defined(SB4K)
    kmWrite32(0x804D7F3C, LI_R3_0);
#endif
}
