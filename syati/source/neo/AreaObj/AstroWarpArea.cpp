#include "neo/AreaObj/AstroWarpArea.h"

namespace neo {
	namespace {
		const char* cAstroWarpBgmName[] = {
			"STM_SMG_ASTROOUT03",
			"STM_SMG_ASTROOUT02"
		};
		const s32 cAstroWarpBgmNameCount = sizeof(cAstroWarpBgmName) / sizeof(const char*);
	}

	AstroWarpArea::AstroWarpArea(const char *pName) : AreaObj(pName) {
		mSpine = NULL;
	}

	void AstroWarpArea::init(const JMapInfoIter &rIter) {
		AreaObj::init(rIter);
		MR::connectToSceneAreaObj(this);

		if (mObjArg0 < 0) {
			mObjArg0 = 0;
		}

		if (mObjArg1 < 0 || mObjArg1 >= cAstroWarpBgmNameCount) {
			mObjArg1 = 0;
		}

		mSpine = new Spine(this, NrvAstroWarpAreaWait, 0);
	}

	void AstroWarpArea::movement() {
		mSpine->update();
	}

	const char* AstroWarpArea::getManagerName() const {
		return "SceneExitArea";
	}

	void AstroWarpArea::exeWait() {
		if (isInVolume(*MR::getPlayerPos())) {
			MR::offPlayerControl();
			MR::curePlayerElementMode();
			MR::closeSystemWipeCircleWithCaptureScreen(60);
			MR::moveVolumeStageBGM(0.0f, 60);
			mSpine->setNerve(NrvAstroWarpAreaStartWarp);
		}
	}

	void AstroWarpArea::exeStartWarp() {
		if (mSpine->mStep == 60) {
			char linkPosName[32];
			sprintf(linkPosName, "AstroWarpPoint%02d", mObjArg0);
			MR::setPlayerPosAndWait(linkPosName);
		}

		if (mSpine->mStep == 120) {
			mSpine->setNerve(NrvAstroWarpAreaEndWarp);
		}
	}

	void AstroWarpArea::exeEndWarp() {
		if (mSpine->mStep == 0) {
			MR::openSystemWipeCircle(60);
			MR::moveVolumeStageBGM(1.0f, 1);
			MR::startStageBGM(cAstroWarpBgmName[mObjArg1], false);
		}

		if (mSpine->mStep == 60) {
			MR::onPlayerControl(true);
			mSpine->setNerve(NrvAstroWarpAreaWait);
		}
	}

	namespace NrvAstroWarpArea {
		void NrvWait::execute(Spine *pSpine) const {
			AstroWarpArea *pActor = (AstroWarpArea*)pSpine->mExecutor;
			pActor->exeWait();
		}

		void NrvStartWarp::execute(Spine *pSpine) const {
			AstroWarpArea *pActor = (AstroWarpArea*)pSpine->mExecutor;
			pActor->exeStartWarp();
		}

		void NrvEndWarp::execute(Spine *pSpine) const {
			AstroWarpArea *pActor = (AstroWarpArea*)pSpine->mExecutor;
			pActor->exeEndWarp();
		}

		NrvWait(NrvWait::sInstance);
		NrvStartWarp(NrvStartWarp::sInstance);
		NrvEndWarp(NrvEndWarp::sInstance);
	}
}
