#include "Game/MapObj/PowerStar.h"
#include "Game/System/AllData/GalaxyStatusAccessor.h"
#include "Game/System/AllData/GameDataFunction.h"
#include "Game/System/AllData/GameSequenceFunction.h"
#include "Game/System/ScenarioDataParser.h"
#include "Game/Util/ActionSoundUtil.h"
#include "Game/Util/ActorSensorUtil.h"
#include "Game/Util/DemoUtil.h"
#include "Game/Util/EventUtil.h"
#include "Game/Util/LiveActorUtil.h"
#include "Game/Util/ObjUtil.h"
#include "Game/Util/PlayerUtil.h"
#include "Game/Util/SceneUtil.h"
#include "neo/System/InStageSave.h"
#include "kamek/hooks.h"

namespace {
    // =================================================================================================================
    // Star Chance music prohibition
    // =================================================================================================================

    bool isAllowPlayStarChance(s32 powerStarId) {
        s32 noPlayStarChance;

        GalaxyStatusAccessor gsa = MR::makeCurrentGalaxyStatusAccessor();
        gsa.mScenarioData->getValueS32("NoPlayStarChance", powerStarId, &noPlayStarChance);

        return noPlayStarChance == 0;
    }

    kmCall(0x802E07E0, isAllowPlayStarChance);
    kmCall(0x802E0A90, isAllowPlayStarChance);


    // =================================================================================================================
    // Green Star nerves
    // =================================================================================================================

    void GreenStar_exeGot(PowerStar *pStar);
    void GreenStar_exeGotDisappear(PowerStar *pStar);

    namespace NrvGreenStar {
        NERVE(NrvGot);
        NERVE(NrvGotDisappear);

        NrvGot(NrvGot::sInstance);
        NrvGotDisappear(NrvGotDisappear::sInstance);

        void NrvGot::execute(Spine* pSpine) const {
            PowerStar* pActor = (PowerStar*)pSpine->mExecutor;
            GreenStar_exeGot(pActor);
        }

        void NrvGotDisappear::execute(Spine* pSpine) const {
            PowerStar* pActor = (PowerStar*)pSpine->mExecutor;
            GreenStar_exeGotDisappear(pActor);
        }

#define NrvGreenStarGot &::NrvGreenStar::NrvGot::sInstance
#define NrvGreenStarGotDisappear &::NrvGreenStar::NrvGotDisappear::sInstance
    }


    // =================================================================================================================
    // Green Star execution
    // =================================================================================================================

    void GreenStar_exeGot(PowerStar *pStar) {
        pStar->processWait(30.0f);

        if (MR::isFirstStep(pStar)) {
            MR::tryRumblePadMiddle(pStar, 0);
            MR::startActionSound(pStar, "SyStarGet", -1, -1, -1);
        }

        if (MR::isStep(pStar, 20) && !MR::isPlayerDead()) {
            MR::tryStartDemo(pStar, "GreenStarDemo");
            neo::performInStageSave(pStar->mPowerStarId);
        }

        if (MR::isGreaterEqualStep(pStar, 20) && !GameSequenceFunction::isActiveSaveDataHandleSequence()) {
            if (MR::isDemoActive("GreenStarDemo")) {
                MR::endDemo(pStar, "GreenStarDemo");
            }

            pStar->setNerve(NrvGreenStarGotDisappear);
        }
    }

    void GreenStar_exeGotDisappear(PowerStar *pStar) {
        pStar->processWait(30.0f);

        f32 xz = MR::calcNerveValue(pStar, 20, 1.0f, 0.0001f);
        f32 y = MR::calcNerveValue(pStar, 20, 1.0f, 5.0f);

        pStar->mScale.set(xz, y, xz);

        if (MR::isGreaterEqualStep(pStar, 20)) {
            pStar->makeActorDead();
        }
    }


    // =================================================================================================================
    // Get Power Star overrides
    // =================================================================================================================

    bool tryDoStarGet(PowerStar* pStar) {
        if (neo::allowInStageSave(pStar->mPowerStarId)) {
            if (pStar->isEndAppearDemo()) {
                MR::invalidateHitSensors(pStar);
                pStar->setNerve(NrvGreenStarGot);
            }
            return true;
        }
        
        return false;
    }

    bool canStarMsgRushTakeOver(PowerStar* pStar, u32 msg) {
        return MR::isMsgIsRushTakeOver(msg) ? !tryDoStarGet(pStar) : false;
    }

    // PowerStar::receiveOtherMsg
    kmWrite32(0x802E0000, PPC_NOP);
    kmCall(0x802E0008, canStarMsgRushTakeOver);


    bool doGetStar(PowerStar* pStar, const Nerve* pNerve) {
        return tryDoStarGet(pStar) ? false : pStar->isNerve(pNerve);
    }

    // PowerStar::receiveOtherMsg
    kmCall(0x802E0034, doGetStar);
}
