#include "loader/KamekModule.h"
#include "loader/KamekAccess.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "Game/Util/FileUtil.h"
#include "Game/Util/MemoryUtil.h"
#include "revolution/os.h"
#include "kamek/hooks.h"

namespace {
    // =================================================================================================================
    // CustomCode binary paths
    // =================================================================================================================

#if defined(SB4P)
    const char* cCustomCodePath = "/CustomCode/CustomCode_SB4P.bin";
#elif defined(SB4E)
    const char* cCustomCodePath = "/CustomCode/CustomCode_SB4E.bin";
#elif defined(SB4J)
    const char* cCustomCodePath = "/CustomCode/CustomCode_SB4J.bin";
#elif defined(SB4W)
    const char* cCustomCodePath = "/CustomCode/CustomCode_SB4W.bin";
#elif defined(SB4K)
    const char* cCustomCodePath = "/CustomCode/CustomCode_SB4K.bin";
#endif


    // =================================================================================================================
    // Install Kamek module
    // =================================================================================================================

    static void installCustomCodeModule() {
        if (!MR::isFileExist(cCustomCodePath, false)) {
            setKamekModule(nullptr);
        }
        
        JKRHeap* pHeap = MR::getStationedHeapNapa();
        
        KamekModule* pModule = new (pHeap, 4) KamekModule(cCustomCodePath);
        pModule->install(pHeap);
        
        setKamekModule(pModule);

        OSReport(
            "Installed module at %p\n"
            "  code : ptr=%p size=%d\n"
            "  bss  : ptr=%p size=%d\n",
            pModule,
            pModule->getCodePtr(), pModule->getCodeSize(),
            pModule->getBssPtr(), pModule->getBssSize()
        );
    }

#if defined(SB4_JEP)
        kmBranch(0x804B7D38, installCustomCodeModule);
#elif defined(SB4_WK)
        kmBranch(0x804B7DA8, installCustomCodeModule);
#endif


    // =================================================================================================================
    // Enable crash debugger
    // =================================================================================================================

#if defined(SB4_JEP)
        kmWrite32(0x804B7D90, 0x60000000);
        kmWrite32(0x804B7E54, 0x60000000);
        kmWrite32(0x805B63A8, 0x60000000);
        kmWrite32(0x805B66B4, 0x60000000);
#elif defined(SB4_WK)
        kmWrite32(0x804B7E00, 0x60000000);
        kmWrite32(0x804B7EC4, 0x60000000);
        kmWrite32(0x805B64A8, 0x60000000);
        kmWrite32(0x805B67B4, 0x60000000);
#endif
}
