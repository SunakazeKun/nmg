#include "neo/NPC/HelperWitch.h"
#include "neo/System/DaredevilModeAccess.h"
#include "pt/Util/TalkUtil.h"

namespace neo {
	HelperWitch::HelperWitch(const char *pName) : NPCActor(pName) {
		mRestartId.mId = -1;
		mRestartId.mZoneId = 0;
		mShouldReset = false;
	}

	void HelperWitch::init(const JMapInfoIter &rIter) {
		NPCActorCaps capabilities("HelperWitch");
		capabilities.setDefault();

		mTalkParam.mActionWait = "WaitA";
		mTalkParam.mActionWaitTurn = "WaitA";
		mActionSpin = "Spin";
		mActionTrampled = "Trampled";
		mActionPointing = "Pointing";
		mActionReaction = "Reaction";

		initialize(rIter, capabilities, "HelperWitch", NULL, false);

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
