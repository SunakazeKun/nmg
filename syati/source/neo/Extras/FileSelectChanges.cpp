#include "syati.h"
#include "neo/core.h"
#include "neo/System/DaredevilModeAccess.h"
#include "neo/System/GameProgress.h"

namespace {
	/*****************************************************************************************************************/
	/* Display new file select info                                                                                  */
	/*****************************************************************************************************************/
	void reflectFileSelectInfoExt(FileSelectInfo *pInfo) {
		MR::setTextBoxNumberRecursive(pInfo, "MissCounter", pInfo->mMissCount);
		MR::setTextBoxMessageRecursive(pInfo, "TxtDuration", pInfo->mCumulativePlayTimeString);

		if (pInfo->mIsView120StarsEnding) {
			MR::hidePaneRecursive(pInfo, "PicFileInfo");
			MR::showPaneRecursive(pInfo, "PicFileDaredevil");
			MR::showPaneRecursive(pInfo, "PicDaredevil");
		}
		else {
			MR::showPaneRecursive(pInfo, "PicFileInfo");
			MR::hidePaneRecursive(pInfo, "PicFileDaredevil");
			MR::hidePaneRecursive(pInfo, "PicDaredevil");
		}
	}

	void setFileSelectFileInfoExt(FileSelectInfo *pInfo, FileSelectFileInfo &rFileInfo) {
		register UserFile* pUserFile;
		__asm {
			lwz r0, 0xC4(r30)
			add pUserFile, r0, r28
		}

		rFileInfo.mCompleteSilver = false;
		rFileInfo.mCompleteGold = false;
		rFileInfo.mCompletePlatinum = false;
		rFileInfo.mIsView120StarsEnding = neo::isUserFileDaredevilMode(pUserFile);

		switch (neo::getGameProgressType(pUserFile)) {
		case neo::GameProgress_AllComplete:
			rFileInfo.mCompletePlatinum = true;
		case neo::GameProgress_AllStars:
			rFileInfo.mCompleteGold = true;
		case neo::GameProgress_GreenStars:
			rFileInfo.mCompleteSilver = true;
		case neo::GameProgress_NormalStars:
			break;
		}

		pInfo->setInfo(rFileInfo);
	}

	kmCall(0x8046DD44, reflectFileSelectInfoExt);
	kmCall(0x80250550, setFileSelectFileInfoExt);
	kmWrite32(0x80251190, NOP); // read proper MissCount number
	kmWrite32(0x8046DB94, 0x991E00C9);


	/*****************************************************************************************************************/
	/* Unlock Rosalina's icon                                                                                        */
	/*****************************************************************************************************************/
	bool allowRosettaMiiIcon(const UserFile *pUserFile) {
		GameDataAllGalaxyStorage* allGalaxyStorage = pUserFile->mGameDataHolder->mAllGalaxyStorage;
		return allGalaxyStorage->makeAccessor(STAGE_COMET_OBSERVATORY)->getScenarioAccessor(2)->mFlag & 1;
	}

	kmCall(0x80252208, allowRosettaMiiIcon);
}
