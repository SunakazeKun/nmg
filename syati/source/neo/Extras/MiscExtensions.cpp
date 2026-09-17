#include "Game/Util/ActorSensorUtil.h"
#include "Game/Util/JMapUtil.h"
#include "Game/Util/LayoutUtil.h"
#include "Game/Util/LiveActorUtil.h"
#include "Game/Util/SceneUtil.h"
#include "kamek/hooks.h"
#include "neo/core.h"


extern "C" {
    bool getJMapInfoArg6NoInit__2MRFRC12JMapInfoIterPb(const JMapInfoIter&, bool*);
}

namespace {
    // =================================================================================================================
    // Don't disable IslandFleetGalaxy's intro camera
    // =================================================================================================================

    kmWrite32(0x80142C34, PPC_LI(3, 0));
    kmWrite32(0x80451804, PPC_LI(3, 0));


    // ==================================================================================================================
    // Skippable intro after collecting a Power Star
    // ==================================================================================================================

    // GameDataFunction::setPowerStar
#if defined(SB4P) || defined(SB4E) || defined(SB4J)
    kmWrite32(0x804D254C, 0x60000005);
#elif defined(SB4W) || defined(SB4K)
    kmWrite32(0x804D25DC, 0x60000005);
#endif


    // =================================================================================================================
    // Don't display World names
    // =================================================================================================================

    void setTextBoxMessageAndHidePaneRecursive(LayoutActor *pLayout, const char *pPaneName, const wchar_t *pMessage) {
        MR::setTextBoxMessageRecursive(pLayout, pPaneName, pMessage);
        MR::hidePaneRecursive(pLayout, pPaneName);
    }

    kmCall(0x8045E318, setTextBoxMessageAndHidePaneRecursive);
#if defined(SB4P) || defined(SB4E) || defined(SB4J)
    kmWrite32(0x80486FE0, PPC_NOP);
    kmBranch(0x804A9CAC, setTextBoxMessageAndHidePaneRecursive);
    kmWrite32(0x804A9C00, PPC_BLR);
#elif defined(SB4W) || defined(SB4K)
    kmWrite32(0x80486FF0, PPC_NOP);
    kmBranch(0x804A9D1C, setTextBoxMessageAndHidePaneRecursive);
    kmWrite32(0x804A9C70, PPC_BLR);
#endif


    // =================================================================================================================
    // Move World Map Power Star count
    // =================================================================================================================

#if defined(SB4P)
    kmWrite32(0x807E6E60, 0x43AC0000);
#elif defined(SB4E)
    kmWrite32(0x807E1760, 0x43AC0000);
#elif defined(SB4J)
    kmWrite32(0x807E0F20, 0x43AC0000);
#elif defined(SB4W)
    kmWrite32(0x8073AD80, 0x43AC0000);
#elif defined(SB4K)
    kmWrite32(0x80739920, 0x43AC0000);
#endif


    // ==================================================================================================================
    // Increase Coin receiver sensors count
    // ==================================================================================================================

    // Coin::init
    kmWrite32(0x8028BDC4, PPC_LI(6, 8));


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
    // Penguins carry Gold Shells when Obj_arg6 is set
    // 
    // Expected registers:
    //   - r3: RemovableTurtle*
    //   - r5: bool "isGolden"
    //   - r28: JMapInfoIter&
    //   - r30: Penguin*
    // ==================================================================================================================

    static __asm void extPenguinInitGetRemovableTurtleType() {
        nofralloc;
        stwu r1, -0x10(r1);
        mflr r0;
        stw r0, 0x14(r1);

        li r0, 0;
        stb r0, 0x8(r1);
        stw r3, 0xC(r1);

        mr r3, r28;
        addi r4, r1, 0x8;
        bl getJMapInfoArg6NoInit__2MRFRC12JMapInfoIterPb;

        lbz r5, 0x8(r1);
        lwz r3, 0xC(r1);
        mr r4, r30;

        lwz r0, 0x14(r1);
        mtlr r0;
        addi r1, r1, 0x10;
        blr;
    }

    kmCall(0x80357D3C, extPenguinInitGetRemovableTurtleType);


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


    // BasaBasaIce
    bool extBasaBasaIceCheckPlayerSpinAttackOnly(u32 msg) {
        return MR::isMsgPlayerSpinAttack(msg);
    }

    kmCall(0x801583F4, extBasaBasaIceCheckPlayerSpinAttackOnly);


    // ==================================================================================================================
    // Fix Rocto hide behavior
    // ==================================================================================================================

    // OtaRock::tryToHide
    kmWrite32(0x801DCBB4, PPC_COND_BRANCH(PPC_COND_NE, 0x14));


    // ==================================================================================================================
    // Cosmic Spirit also appears when playing as Luigi
    // ==================================================================================================================

    // SuperDreamer::init
    kmWrite32(0x80360D54, PPC_LI(3, 0));

    // GameSequenceFunction::isAllowDreamerInStage
#if defined(SB4P) || defined(SB4E) || defined(SB4J)
    kmWrite32(0x804D7EAC, PPC_LI(3, 0));
#elif defined(SB4W) || defined(SB4K)
    kmWrite32(0x804D7F3C, PPC_LI(3, 0));
#endif
}
