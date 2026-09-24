#pragma once

#include "revolution/types.h"

struct KamekHeader {
	u32 mMagic1;
	u16 mMagic2;
	u16 mVersion;
	u32 mBssSize;
	u32 mCodeSize;
	u32 mCtorStart;
	u32 mCtorEnd;
	u8 __padding[8];
};

struct KamekBinary {
	KamekHeader mHeader;
	u8 mData[];
};

#define KAMEK_CMD_Addr32 1
#define KAMEK_CMD_Addr16Lo 4
#define KAMEK_CMD_Addr16Hi 5
#define KAMEK_CMD_Addr16Ha 6
#define KAMEK_CMD_Rel24 10
#define KAMEK_CMD_Write32 32
#define KAMEK_CMD_Write16 33
#define KAMEK_CMD_Write8 34
#define KAMEK_CMD_CondWritePointer 35
#define KAMEK_CMD_CondWrite32 36
#define KAMEK_CMD_CondWrite16 37
#define KAMEK_CMD_CondWrite8 38
#define KAMEK_CMD_Branch 64
#define KAMEK_CMD_BranchLink 65
