#pragma once

#include "revolution/types.h"

namespace gst {
    enum GhostType {
        GhostType_GhostAttackGhost = 0,
        GhostType_PichanRacer = 1,
        GhostType_GhostPlayerMario = 2,
        GhostType_GhostPlayerLuigi = 3,

        GhostType_Invalid = -1
    };
}
