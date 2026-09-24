#include "JSystem/JKernel.h"
#include "revolution/os.h"
#include "kamek/hooks.h"

namespace {
    // =====================================================================================================================
    // New heap sizes
    // =====================================================================================================================

#define WORLD_MAP_HEAP_SIZE (0x1100000u)

    // WorldMapHeap
    kmWrite16(0x80040F12, -(WORLD_MAP_HEAP_SIZE >> 16));
#if defined(SB4P) || defined(SB4E) || defined(SB4J)
    kmWrite16(0x804BCB16, (WORLD_MAP_HEAP_SIZE >> 16));
#elif defined(SB4W) || defined(SB4K)
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

#if defined(SB4P) || defined(SB4E) || defined(SB4J)
    kmBranch(0x804BD010, memoryErrorCallback);
#elif defined(SB4W) || defined(SB4K)
    kmBranch(0x804BD080, memoryErrorCallback);
#endif
}
