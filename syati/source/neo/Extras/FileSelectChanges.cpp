#include "Game/Screen/FileSelectInfo.h"
#include "Game/System/GameData/UserFile.h"
#include "neo/System/DaredevilModeAccess.h"
#include "neo/System/GameProgress.h"
#include "Game/Util/LayoutUtil.h"
#include "kamek/hooks.h"

namespace {
    void extFileSelector_setFileInfo(FileSelectInfo *pInfo, FileSelectFileInfo &rFileInfo) {
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

    kmCall(0x80250550, extFileSelector_setFileInfo);


    // -----------------------------------------------------------------------------------------------------------------

    void extFileSelectInfo_reflectInfo(FileSelectInfo *pInfo) {
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

    kmCall(0x8046DD44, extFileSelectInfo_reflectInfo);


    // -----------------------------------------------------------------------------------------------------------------

    // Always display the death count
    kmWrite32(0x80251190, PPC_NOP);


    // Normal game completion equals Silver Crown progress
    kmWrite32(0x8046DB94, PPC_STB(8, 0xC9, 30));


    // Unlock Rosalina's file icon
    kmCall(0x80252208, neo::checkUserFileAllowRosettaMiiIcon);
}
