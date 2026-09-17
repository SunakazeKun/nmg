#include "revolution/types.h"
#include "kamek/hooks.h"

typedef void (*Func)();
extern Func __ctor_loc;
extern Func __ctor_end;

namespace {
    void init() {
        for (Func* f = &__ctor_loc; f < &__ctor_end; f++) {
            (*f)();
        }
    }

    kmBranch(0x8003B344, init); // Called after initAcosTable
}
