#pragma once

#include "revolution/types.h"

class KamekBinary;
class JKRHeap;

class KamekModule {
public:
	KamekModule(const char *pFilePath);
	
	void install(JKRHeap *pHeap);
	
	const char* getFilePath() const { return mFilePath; }
	JKRHeap* getHeap() const { return mHeap; }
	
	void* getCodePtr() const { return mCodePtr; }
	u32 getCodeSize() const { return mCodeSize; }
	
	void* getBssPtr() const { return mBssPtr; }
	u32 getBssSize() const { return mBssSize; }
	
	bool isInstalled() const { return mInstalled; }
	
private:
	void load();
	void link();
	void init();
	void clean();
	
	void error(const char *msg);
	
	char mFilePath[64];
	JKRHeap* mHeap;
	void* mCodePtr;
	void* mBssPtr;
	u32 mCodeSize;
	u32 mBssSize;
	KamekBinary* mBinary;
	u32 mRelaSize;
	bool mInstalled;
};
