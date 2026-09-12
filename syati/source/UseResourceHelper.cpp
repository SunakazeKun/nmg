#include "syati.h"

#ifdef DEBUG

bool printArchive(const char *pName) {
	OSReport("ARCHIVE %s\n", pName);
	return MR::isFileExist(pName, false);
}

u32 printSound(const char *pName) {
	OSReport("SOUND %s\n", pName);
	return JGadget::getHashCode(pName);
}

#if defined(TWN)|| defined(KOR)
kmCall(0x804B2050, printArchive);
#else
kmCall(0x804B1FE0, printArchive);
#endif
kmCall(0x80086544, printSound);

#endif // DEBUG
