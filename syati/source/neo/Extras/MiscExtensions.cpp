#include "Game/Util/ActorSensorUtil.h"
#include "Game/Util/ActorSwitchUtil.h"
#include "Game/Util/JMapUtil.h"
#include "Game/Util/LiveActorUtil.h"
#include "Game/Util/SceneUtil.h"
#include "Game/MapObj/TicoCoin.h"
#include "kamek/hooks.h"
#include "neo/core.h"

extern "C" {
    bool getJMapInfoArg6NoInit__2MRFRC12JMapInfoIterPb(const JMapInfoIter &, bool *);
}

namespace {
    // ==================================================================================================================
    // Increase Coin receiver sensors count
    // ==================================================================================================================

    // Coin::init
    kmWrite16(0x8028BDC6, 8);


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
        nofralloc
        stwu r1, -0x10(r1)
        mflr r0
        stw r0, 0x14(r1)

        li r0, 0
        stb r0, 0x8(r1)
        stw r3, 0xC(r1)

        mr r3, r28
        addi r4, r1, 0x8
        bl getJMapInfoArg6NoInit__2MRFRC12JMapInfoIterPb

        lbz r5, 0x8(r1)
        lwz r3, 0xC(r1)
        mr r4, r30

        lwz r0, 0x14(r1)
        mtlr r0
        addi r1, r1, 0x10
        blr
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
    // Comet Medal activates SW_DEAD on collection
    // ==================================================================================================================

    void extTicoCoinExeCollect(TicoCoin *pCoin, const Nerve *pNerve) {
        if (MR::isValidSwitchDead(pCoin)) {
            MR::onSwitchDead(pCoin);
        }

        pCoin->setNerve(pNerve);
    }

    kmCall(0x80323024, extTicoCoinExeCollect);
}
