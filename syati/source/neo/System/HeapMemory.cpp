#include "JSystem/JKernel.h"
#include "Game/Util/MemoryUtil.h"
#include "revolution/os.h"
#include "kamek/hooks.h"

namespace {
    // =====================================================================================================================
    // Print heap sizes
    // =====================================================================================================================
    
#ifdef DEBUG
    void printHeapSizeInfo(JKRHeap *pHeap, const char *pName) {
        u32 totalSize = pHeap->mSize;
        u32 freeSize = pHeap->getTotalFreeSize();

        OSReport("%s: total = %d, free = %d\n", pName,  totalSize, freeSize);
    }

    void printWorldMapHeapSizeAndDestroy(JKRHeap **pHeapPtr) {
        JKRHeap* pHeap = *pHeapPtr;

        printHeapSizeInfo(pHeap, "WorldMapHeap");

        JKRHeap::destroy(pHeap);
        *pHeapPtr = nullptr;
    }

    void printFileCacheHeapSizeAndDestroy(JKRHeap **pHeapPtr) {
        JKRHeap* pHeap = *pHeapPtr;

        printHeapSizeInfo(pHeap, "FileCacheHeap");

        JKRHeap::destroy(pHeap);
        *pHeapPtr = nullptr;
    }

    void printStationedHeapNapaSizeAndAdjust(JKRExpHeap *pHeap) {
        printHeapSizeInfo(pHeap, "StationedHeapNapa");
        MR::adjustHeapSize(pHeap);
    }

    void printStationedHeapGDDR3SizeAndAdjust(JKRExpHeap *pHeap) {
        printHeapSizeInfo(pHeap, "StationedHeapGDDR3");
        MR::adjustHeapSize(pHeap);
    }

#if defined(SB4_JEP)
    kmBranch(0x804BCBF4, printStationedHeapNapaSizeAndAdjust);
    kmBranch(0x804BCC04, printStationedHeapGDDR3SizeAndAdjust);
    kmBranch(0x804BCD34, printWorldMapHeapSizeAndDestroy);
    kmBranch(0x804BCD44, printFileCacheHeapSizeAndDestroy);
#elif defined(SB4_WK)
    kmBranch(0x804BCC64, printStationedHeapNapaSizeAndAdjust);
    kmBranch(0x804BCC74, printStationedHeapGDDR3SizeAndAdjust);
    kmBranch(0x804BCDA4, printWorldMapHeapSizeAndDestroy);
    kmBranch(0x804BCDB4, printFileCacheHeapSizeAndDestroy);
#endif

#endif


    // =====================================================================================================================
    // New heap sizes
    // =====================================================================================================================

#define WORLD_MAP_HEAP_SIZE (0x9A0000u)

    // WorldMapHeap
    kmWrite16(0x80040F12, -(WORLD_MAP_HEAP_SIZE >> 16));
#if defined(SB4_JEP)
    kmWrite16(0x804BCB16, (WORLD_MAP_HEAP_SIZE >> 16));
#elif defined(SB4_WK)
    kmWrite16(0x804BCB86, (WORLD_MAP_HEAP_SIZE >> 16));
#endif


    // =====================================================================================================================
    // Allocation error callback
    // =====================================================================================================================

    void memoryErrorCallback(JKRHeap* pHeap, u32 size, int align) {
        u32 totalSize = pHeap->mSize;
        u32 freeSize = pHeap->getFreeSize();

        OSPanic(__FILE__, __LINE__, "Failed to allocate memory: total=%d, free=%d, requested=%d\n",
            totalSize, freeSize, size);
    }

#if defined(SB4_JEP)
    kmBranch(0x804BD010, memoryErrorCallback);
#elif defined(SB4_WK)
    kmBranch(0x804BD080, memoryErrorCallback);
#endif
}
