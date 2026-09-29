#pragma once

#include "gst/GhostType.h"
#include "Game/Player/GhostPacketSyncData.h"

namespace gst {
    enum GstRecorderMode {
        GstRecorderMode_Waiting = 0,
        GstRecorderMode_Preparing = 1,
        GstRecorderMode_Recording = 2,
        GstRecorderMode_Stopped = 3
    };

    struct GstRecorderInfo {
        GstRecorderInfo();

        u32 mUpdateFrame;
        GstRecorderMode mRecorderMode;
        const char* mCurrentStageName;
        s32 mCurrentStageScenario;
        bool mIsSMG2;
        GhostType mGhostDataType;
        GhostPacketSyncData mGhostDataStruct;

        static GstRecorderInfo sInstance;
    };

    bool isEnableDebugGstRecorder();

    void waitGstRecorder(); 
    void prepareGstRecorder(const char *stageName, s32 scenarioId, GhostType ghostType);
    void startGstRecorder();
    void stopGstRecorder();
    void supplyGstFrameSyncWithActor(const LiveActor *actor, bool syncAnims);
    void supplyGstFrameSyncWithPlayer();
    void incGstFrame();
}
