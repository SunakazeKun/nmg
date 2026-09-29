#include "gst/GstRecordSystem.h"
#include "kamek/hooks.h"

#define ADDR_GST_RECORDER_INFO_PTR (0x80003FF8)

namespace gst {
    static GstRecorderInfo** sPtrForScript = (GstRecorderInfo**)ADDR_GST_RECORDER_INFO_PTR;

    GstRecorderInfo::GstRecorderInfo() {
        *sPtrForScript = this;
        mIsSMG2 = true;
        mRecorderMode = GstRecorderMode_Waiting;
        mCurrentStageName = NULL;
        mCurrentStageScenario = -1;
        mGhostDataType = GhostType_Invalid;
        mUpdateFrame = 0;
        GhostPacketSyncData(mGhostDataStruct);
    }

    GstRecorderInfo(GstRecorderInfo::sInstance);

    // ----------------------------------------------------------------------------------------------------------------

    bool isEnableDebugGstRecorder() {
#ifdef DEBUG
        return true;
#else
        return false;
#endif
    }

    void waitGstRecorder() {
        GstRecorderInfo::sInstance.mRecorderMode = GstRecorderMode_Waiting;
    }

    void prepareGstRecorder(const char *stageName, s32 scenarioId, GhostType ghostType) {
        GstRecorderInfo::sInstance.mCurrentStageName = stageName;
        GstRecorderInfo::sInstance.mCurrentStageScenario = scenarioId;
        GstRecorderInfo::sInstance.mGhostDataType = ghostType;
        GstRecorderInfo::sInstance.mUpdateFrame = 0;
        GhostPacketSyncData(mGhostDataStruct);

        GstRecorderInfo::sInstance.mRecorderMode = GstRecorderMode_Preparing;
    }

    void startGstRecorder() {
        GstRecorderInfo::sInstance.mRecorderMode = GstRecorderMode_Recording;
    }

    void stopGstRecorder() {
        GstRecorderInfo::sInstance.mRecorderMode = GstRecorderMode_Stopped;
    }

    void supplyGstFrameSyncWithActor(const LiveActor *actor, bool syncAnims) {
        GstRecorderInfo::sInstance.mGhostDataStruct.syncWithActor(actor, syncAnims);
        GstRecorderInfo::sInstance.mUpdateFrame++;
    }

    void supplyGstFrameSyncWithPlayer() {
        GstRecorderInfo::sInstance.mGhostDataStruct.syncWithPlayer();
        GstRecorderInfo::sInstance.mUpdateFrame++;
    }

    void incGstFrame() {
        GstRecorderInfo::sInstance.mUpdateFrame++;
    }
}

namespace {
    void handleAnySceneDestroyed() {
        gst::waitGstRecorder();
    }

#if defined(SB4_JEP)
    kmBranch(0x804BAB30, handleAnySceneDestroyed);
#elif defined(SB4_WK)
    kmBranch(0x804BABA0, handleAnySceneDestroyed);
#endif
}
