#include "neo/NPC/HelperWitch.h"
#include "neo/System/DaredevilModeAccess.h"
#include "Game/System/AllData/GameDataFunction.h"
#include "Game/System/AllData/GameSequenceFunction.h"
#include "Game/Util/ActionSoundUtil.h"
#include "Game/Util/BgmUtil.h"
#include "Game/Util/EventUtil.h"
#include "Game/Util/MessageUtil.h"
#include "Game/Util/JMapUtil.h"
#include "Game/Util/SoundUtil.h"
#include "pt/Util/TalkUtil.h"

namespace neo {
    HelperWitch::HelperWitch(const char *pName)
        : NPCActor(pName),
        mRestartId(),
        mShouldReset()
    {
        
    }

    void HelperWitch::init(const JMapInfoIter &rIter) {
        NPCActorCaps caps("HelperWitch");

        caps.setDefault();
        caps.mTalkOffset.set(0.0f, 220.0f, 0.0f);

        initialize(rIter, caps, "HelperWitch", nullptr, false);

        mTalkParam.mActionWait = "WaitA";
        mTalkParam.mActionWaitTurn = "WaitA";

        mActionSpin = "Spin";
        mActionTrampled = "Trampled";
        mActionPointing = "Pointing";
        mActionReaction = "Reaction";

        MR::registerEventFunc(mTalkCtrl, TalkMessageFunc(this, &eventFunc));
        MR::registerBranchFunc(mTalkCtrl, TalkMessageFunc(this, &branchFunc));

        MR::getJMapInfoArg0NoInit(rIter, &mRestartId.mId);
        mRestartId.mZoneId = mLinkedInfo.mZoneId;
        
        mScale.setAll(1.35f);
    }

    bool HelperWitch::eventFunc(u32 funcType) {
        switch (funcType) {
        case HELPER_WITCH_EVENT_DAREDEVIL_MODE_ON:
            mShouldReset |= neo::tryOnDaredevilMode();
            return true;
        case HELPER_WITCH_EVENT_DAREDEVIL_MODE_OFF:
            mShouldReset |= neo::tryOffDaredevilMode();
            return true;
        case HELPER_WITCH_EVENT_SET_PLAYER_MARIO:
            mShouldReset |= MR::isPlayerLuigi();
            GameDataFunction::forceMarioPlayer();
            return true;
        case HELPER_WITCH_EVENT_SET_PLAYER_LUIGI:
            mShouldReset |= !MR::isPlayerLuigi();
            GameDataFunction::forceLuigiPlayer();
            return true;
        case HELPER_WITCH_EVENT_MSG_ARG_DAREDEVIL:
            if (neo::isDaredevilMode()) {
                pt::setMessageArg(mTalkCtrl, MR::getGameMessageDirect("HelperWitchOption_DaredevilMode"));
            }
            else {
                pt::setMessageArg(mTalkCtrl, MR::getGameMessageDirect("HelperWitchOption_NormalMode"));
            }
            return true;
        case HELPER_WITCH_EVENT_MSG_ARG_PLAYER:
            if (MR::isPlayerLuigi()) {
                pt::setMessageArg(mTalkCtrl, MR::getGameMessageDirect("HelperWitchOption_PlayerLuigi"));
            }
            else {
                pt::setMessageArg(mTalkCtrl, MR::getGameMessageDirect("HelperWitchOption_PlayerMario"));
            }
            return true;
        case HELPER_WITCH_EVENT_RESET_SCENE:
            if (mRestartId.mId >= 0) {
                MR::setRestartMarioNo(mRestartId);
            }

            GameSequenceFunction::restartCurrentStageAndClearAfterMiss();
            MR::stopStageBGM(30);
            MR::stopSubBGM(30);
            MR::onDestroySceneKeepAllSound();
            MR::startActionSound(this, "WipeIn", -1, -1, -1);

            mShouldReset = false;
            return false;
        default:
            return false;
        }
    }

    bool HelperWitch::branchFunc(u32 funcType) {
        switch (funcType) {
        case HELPER_WITCH_BRANCH_RESET_BEFORE:
            return mRestartId == *MR::getRestartMarioNo();
        case HELPER_WITCH_BRANCH_SHOULD_RESET:
            return mShouldReset;
        case HELPER_WITCH_BRANCH_DAREDEVIL_MODE:
            return neo::isDaredevilMode();
        default:
            return false;
        }
    }
}
