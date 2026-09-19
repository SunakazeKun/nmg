#include "revolution/types.h"
#include "kamek/hooks.h"

// This should actually be part of the Kamek loader itself. Kamek binaries now store the __ctor_loc and __ctor_end
// addresses in the file's header which can be used for calling the static initializers after linking the binary.

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
