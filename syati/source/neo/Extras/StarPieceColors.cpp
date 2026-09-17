#include "Game/MapObj/StarPiece.h"
#include "Game/Util/MathUtil.h"
#include "kamek/hooks.h"

namespace {
    GXColor cStarPieceColors[] = {
        // Old colors
        { 0x80, 0x00, 0x99, 0xFF }, // purple
        { 0xE6, 0xA0, 0x00, 0xFF }, // yellow
        { 0x46, 0xA1, 0x08, 0xFF }, // green
        { 0x37, 0x5A, 0xA0, 0xFF }, // blue
        { 0xBE, 0x33, 0x0B, 0xFF }, // red
        { 0x80, 0x80, 0x80, 0xFF }, // white

        // New colors
        { 0xC0, 0x00, 0xA0, 0xFF }, // pink
        { 0x7F, 0x7F, 0x00, 0xFF }, // gold
        { 0x16, 0x4C, 0x00, 0xFF }, // dark green
        { 0x00, 0x7F, 0x7F, 0xFF }, // teal
        { 0xD8, 0x56, 0x00, 0xFF }, // orange
    };

    // ----------------------------------------------------------------------------------------------------------------

    void initStarPieceColor(StarPiece *pActor, s32 color) {
        if (color < 0 || color >= ARRAY_SIZE(cStarPieceColors)) {
            color = MR::getRandom(0, ARRAY_SIZE(cStarPieceColors));
        }

        pActor->mColor = ::cStarPieceColors[color];
    }

    void setStarPieceColor(StarPiece* pActor, s32 color) {
        pActor->mColor = ::cStarPieceColors[color];
    }

    // StarPiece::init
    kmWrite32(0x80311220, PPC_LWZ(4, 0x18, 1)); // r4 = color
    kmWrite32(0x80311224, PPC_MR(3, 29)); // r3 = StarPiece*
    kmCall(0x80311228, initStarPieceColor);
    kmWrite32(0x8031122C, PPC_B(0x58)); // skip garbage

    // StarPiece::setColor
    kmBranch(0x803116C0, setStarPieceColor);

    // StarPiece::getNumColor
    kmWrite32(0x80311700, PPC_LI(3, ARRAY_SIZE(cStarPieceColors)));
}
