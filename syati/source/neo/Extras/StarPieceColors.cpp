#include "syati.h"

/*********************************************************************************************************************/
/* New Star Bit colors                                                                                               */
/*********************************************************************************************************************/

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

	const s32 cStarPieceColorsCount = sizeof(cStarPieceColors) / sizeof(GXColor);

	// ----------------------------------------------------------------------------------------------------------------

	void initStarPieceColor(StarPiece *pActor, s32 color) {
		if (color < 0 || color >= cStarPieceColorsCount) {
			color = MR::getRandom(0, cStarPieceColorsCount);
		}

		pActor->mColor = ::cStarPieceColors[color];
	}

	kmWrite32(0x80311220, 0x80810018); // r4 = color
	kmWrite32(0x80311224, 0x7FA3EB78); // r3 = StarPiece*
	kmCall(0x80311228, initStarPieceColor);
	kmWrite32(0x8031122C, 0x48000058); // skip garbage

	// StarPiece::setColor(s32) sets from new colors
	void setNewStarPieceColor(StarPiece *pActor, s32 color) {
		pActor->mColor = ::cStarPieceColors[color];
	}

	kmBranch(0x803116C0, setNewStarPieceColor);

	// StarPiece::getNumColor() returns new count
	kmWrite32(0x80311700, 0x38600000 | cStarPieceColorsCount);
}
