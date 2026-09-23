#include "loader/KamekModule.h"
#include "loader/KamekBinary.h"
#include "revolution/dvd.h"
#include "revolution/os.h"
#include "Game/System/FileRipper.h"
#include "Game/Util/FileUtil.h"
#include "Game/Util/MemoryUtil.h"
#include "JSystem/JKernel/JKRHeap.h"

// ---------------------------------------------------------------------------------------------------------------------

typedef void (*CtorFunc)();

#define RESOLVE_PTR(ptr, off) (void*)((u32)ptr + off)

#define ALIGN_32(a) (((u32)a + 31) & ~31)

inline u32 ResolveAddress(u32 text, u32 address) {
	return address & 0x80000000 ? address : (text + address);
}

inline void DCICInvalidate(register void *address) {
	__asm {
		dcbst 0, address
		sync
		icbi 0, address
	}
}

// ---------------------------------------------------------------------------------------------------------------------

KamekModule::KamekModule(const char *pFilePath) {
	memset(mFilePath, 0, sizeof(mFilePath));
	memcpy(mFilePath, pFilePath, strlen(pFilePath));
	
	mHeap = nullptr;

	mCodePtr = nullptr;
	mBssPtr = nullptr;
	mCodeSize = 0;
	mBssSize = 0;
	
	mBinary = nullptr;
    mRelaSize = 0;
	
	mInstalled = false;
}

void KamekModule::install(JKRHeap *pHeap) {
	mHeap = pHeap;
	
	load();
	link();
	init();
	clean();
	
	mInstalled = true;
}

void KamekModule::load() {
    // TODO: Do we really need StationedHeapHDDR3?
    mBinary = (KamekBinary*)FileRipper::loadToMainRAM(mFilePath, nullptr, false, mHeap, FileRipper::UNK_1);
    u32 binarySize = MR::getFileSize(mFilePath, false);

    if (binarySize < sizeof(KamekHeader)) {
        error("Binary too small");
    }
    if (mBinary->mHeader.mMagic1 != 'Kame' || mBinary->mHeader.mMagic2 != 'k\0') {
        error("Unknown binary format");
    }
    if (mBinary->mHeader.mVersion != 2) {
        error("Incompatible binary version");
    }

    mCodeSize = mBinary->mHeader.mCodeSize;
    mBssSize = mBinary->mHeader.mBssSize;
    mRelaSize = binarySize - sizeof(KamekHeader) - mCodeSize;

    void* memory = new (mHeap, 4) u8[getTotalSize()];
    mCodePtr = RESOLVE_PTR(memory, 0);
    mBssPtr = RESOLVE_PTR(memory, mCodeSize);
	
    MR::copyMemory(mCodePtr, mBinary->mData, mCodeSize);
    MR::zeroMemory(mBssPtr, mBssSize);
}

void KamekModule::link() {
	u32 codeStartAddr = (u32)mCodePtr;
    const u32* relaStart = (const u32*)RESOLVE_PTR(mBinary->mData, mCodeSize);
	const u32* relaEnd = (const u32*)RESOLVE_PTR(relaStart, mRelaSize);

    for (const u32* input = relaStart; input < relaEnd; ) {
        u32 cmdHeader = *(input++);

        u8 cmdType = cmdHeader >> 24;
        u32 address = cmdHeader & 0xFFFFFF;

        // Absolute address
        if (address == 0xFFFFFE) {
            address = *(input++);
        }
        // Relative address
        else {
            address += codeStartAddr;
        }

        switch (cmdType) {
        case KAMEK_CMD_Addr32: {
            u32 target = ResolveAddress(codeStartAddr, *(input++));
            *(u32*)address = target;
        } break;
        case KAMEK_CMD_Addr16Lo: {
            u32 target = ResolveAddress(codeStartAddr, *(input++));
            *(u16*)address = target & 0xFFFF;
        } break;
        case KAMEK_CMD_Addr16Hi: {
            u32 target = ResolveAddress(codeStartAddr, *(input++));
            *(u16*)address = target >> 16;
        } break;
        case KAMEK_CMD_Addr16Ha: {
            u32 target = ResolveAddress(codeStartAddr, *(input++));
            *(u16*)address = target >> 16;

            if (target & 0x8000) {
                *(u16*)address += 1;
            }
        } break;

        case KAMEK_CMD_Rel24: {
            u32 target = ResolveAddress(codeStartAddr, *(input++));
            u32 delta = target - address;
            *(u32*)address &= 0xFC000003;
            *(u32*)address |= (delta & 0x3FFFFFC);
        } break;

        case KAMEK_CMD_Write32: {
            u32 value = *(input++);
            *(u32*)address = value;
        } break;
        case KAMEK_CMD_Write16: {
            u32 value = *(input++);
            *(u16*)address = value & 0xFFFF;
        } break;
        case KAMEK_CMD_Write8: {
            u32 value = *(input++);
            *(u8*)address = value & 0xFF;
        } break;

        case KAMEK_CMD_CondWritePointer: {
            u32 target = ResolveAddress(codeStartAddr, *(input++));
            u32 original = *(input++);

            if (*(u32*)address == original) {
                *(u32*)address = target;
            }
        } break;
        case KAMEK_CMD_CondWrite32: {
            u32 value = *(input++);
            u32 original = *(input++);

            if (*(u32*)address == original) {
                *(u32*)address = value;
            }
        } break;
        case KAMEK_CMD_CondWrite16: {
            u32 value = *(input++);
            u32 original = *(input++);

            if (*(u16*)address == (original & 0xFFFF)) {
                *(u16*)address = value & 0xFFFF;
            }
        } break;
        case KAMEK_CMD_CondWrite8: {
            u32 value = *(input++);
            u32 original = *(input++);

            if (*(u8*)address == (original & 0xFF)) {
                *(u8*)address = value & 0xFF;
            }
        } break;

        case KAMEK_CMD_Branch: {
            u32 target = ResolveAddress(codeStartAddr, *(input++));
            u32 delta = target - address;
            *(u32*)address = 0x48000000;
            *(u32*)address |= (delta & 0x3FFFFFC);
        } break;
        case KAMEK_CMD_BranchLink: {
            u32 target = ResolveAddress(codeStartAddr, *(input++));
            u32 delta = target - address;
            *(u32*)address = 0x48000001;
            *(u32*)address |= (delta & 0x3FFFFFC);
        } break;

        default:
            error("Unknown command detected");
        }

        DCICInvalidate((void*)address);
    }

    __sync();
    __isync();
}

void KamekModule::init() {
    CtorFunc* ctorStart = (CtorFunc*)RESOLVE_PTR(mCodePtr, mBinary->mHeader.mCtorStart);
    CtorFunc* ctorEnd = (CtorFunc*)RESOLVE_PTR(mCodePtr, mBinary->mHeader.mCtorEnd);

	for (CtorFunc* f = ctorStart; f < ctorEnd; f++) {
		(*f)();
	}
}

void KamekModule::clean() {
    delete mBinary;
	mBinary = nullptr;
    mRelaSize = 0;
}

void KamekModule::error(const char *msg) {
    char fullMsg[512];
    snprintf(fullMsg, sizeof(fullMsg), "KAMEK ERROR\n\n%s\n", msg);

    GXColor bg = { 0, 0, 0, 0 };
    GXColor fg = { 255, 255, 255, 255 };
    OSFatal(fg, bg, fullMsg);
}
