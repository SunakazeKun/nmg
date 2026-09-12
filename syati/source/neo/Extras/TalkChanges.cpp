#include "syati.h"

namespace {
	/*****************************************************************************************************************/
	/* Talk type that displays "Read" instead of "Talk"                                                              */
	/*****************************************************************************************************************/
	bool isMsgOpenWithRead(TalkMessageInfo *pInfo) {
		return pInfo->mBalloonType == 3 || pInfo->mBalloonType == 5;
	}

	kmCall(0x8037CA9C, isMsgOpenWithRead);


	/*****************************************************************************************************************/
	/* Integrate for new "YesNo" branch labels                                                                       */
	/*****************************************************************************************************************/
	const char* getExtBranchLabel() {
		register s32 branchId;
		__asm {
			mr branchId, r0
		}

		switch (branchId) {
		case 18:
			return "HelperWitchDaredevil";
		case 19:
			return "HelperWitchPlayer";
		default:
			return NULL;
		}
	}

	kmCall(0x8037B670, getExtBranchLabel);


	/*****************************************************************************************************************/
	/* Adjust sounds for new YesNo options                                                                           */
	/*****************************************************************************************************************/
	bool isExtSelectYesNo() {
		register s32 branchId;
		__asm {
			mr branchId, r0
		}

		switch (branchId) {
		case 18:
			return false;
		case 19:
			return false;
		default:
			return true;
		}
	}

	kmCall(0x8037B618, isExtSelectYesNo);


	/*****************************************************************************************************************/
	/* Ruby spacing fix (3.0 -> -2.0)                                                                                */
	/*****************************************************************************************************************/
#if defined(PAL)
	kmWrite32(0x807E6A24, 0xC0000000);
#elif defined(USA)
	kmWrite32(0x807E1324, 0xC0000000);
#elif defined(JPN)
	kmWrite32(0x807E0AE4, 0xC0000000);
#elif defined(TWN)
	kmWrite32(0x8073A944, 0xC0000000);
#elif defined(KOR)
	kmWrite32(0x807394E4, 0xC0000000);
#endif
}
