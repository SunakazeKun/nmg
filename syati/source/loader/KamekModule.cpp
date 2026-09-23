#include "loader/KamekModule.h"
#include "loader/KamekBinary.h"
#include "revolution/dvd.h"
#include "revolution/os.h"
#include "JSystem/JKernel/JKRHeap.h"

// ---------------------------------------------------------------------------------------------------------------------

typedef void (*CtorFunc)();

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
	DVDFileInfo fileHandle;
    
    if (!DVDOpen(mFilePath, &fileHandle)) {
        error("KAMEK ERROR\n\nFailed DVDFastOpen\n");
    }
    if (fileHandle.mLength < sizeof(KamekHeader)) {
        error("KAMEK ERROR\n\nBinary too small\n");
    }
	
	// Analyze header
    {
        u8 buffer[ALIGN_32(sizeof(KamekHeader) * 2)];
        KamekHeader* pHeader = (KamekHeader*)ALIGN_32(buffer);

        DVDReadPrio(&fileHandle, pHeader, sizeof(KamekHeader), 0, 2);

		if (pHeader->mMagic1 != 'Kame' || pHeader->mMagic2 != 'k\0') {
			error("KAMEK ERROR\n\nUnknown module format\n");
		}
		if (pHeader->mVersion != 2) {
			error("KAMEK ERROR\n\nIncompatible version\n");
		}

        mCodeSize = pHeader->mCodeSize;
        mBssSize = pHeader->mBssSize;
    }

    u32 totalSize = mCodeSize + mBssSize;
    u32 binarySize = fileHandle.mLength;
    mRelaSize = binarySize - sizeof(KamekHeader) - mCodeSize;
	
    // Allocate module
	{
        u8* memory = (u8*)JKRHeap::alloc(totalSize, 4, mHeap);
        mCodePtr = memory;
        mBssPtr = memory + mBssSize;
    }
     
    // Preload binary
    {
        mBinary = (KamekBinary*)JKRHeap::alloc(binarySize, 32, mHeap);
        DVDReadPrio(&fileHandle, mBinary, binarySize, 0, 2);
    }
	
	DVDClose(&fileHandle);
	
    memcpy(mCodePtr, mBinary->mData, mCodeSize);
    memset(mBssPtr, 0, mBssSize);
}

void KamekModule::link() {
	u32 codeStartAddr = (u32)mCodePtr;
	const u32* relaStart = (const u32*)((u8*)mBinary->mData + mCodeSize);
	const u32* relaEnd = (const u32*)((u8*)relaStart + mRelaSize);

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
            error("KAMEK ERROR\n\nUnknown command detected\n");
        }

        DCICInvalidate((void*)address);
    }

    __sync();
    __isync();
}

void KamekModule::init() {
	CtorFunc* ctorStart = (CtorFunc*)((u8*)mCodePtr + mBinary->mHeader.mCtorStart);
	CtorFunc* ctorEnd = (CtorFunc*)((u8*)mCodePtr + mBinary->mHeader.mCtorEnd);

	for (CtorFunc* f = ctorStart; f < ctorEnd; f++) {
		(*f)();
	}
}

void KamekModule::clean() {
	//JKRHeap::free(mBinary, nullptr);

	mBinary = nullptr;
    mRelaSize = 0;
}

void KamekModule::error(const char *msg) {
    GXColor bg = { 0, 0, 0, 0 };
    GXColor fg = { 255, 255, 255, 255 };
    OSFatal(fg, bg, msg);
}
