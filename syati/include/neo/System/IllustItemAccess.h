#pragma once

#include "syati.h"

#define SOME_GALAXY_STORAGE_FLAG_ILLUST_ITEM 4

#define IN_TEMP_STAGE_ILLUST_ITEM_FLAG_COLLECTED 1
#define IN_TEMP_STAGE_ILLUST_ITEM_FLAG_SAVED 2
#define IN_TEMP_STAGE_ILLUST_ITEM_FLAG_NOTIFY 4

namespace neo {
	bool isIllustItemInTempStageGet();
	bool isIllustItemInTempStageCollected();
	bool isIllustItemInTempStageNotify();
	void collectIllustItemInTempStage();
	void tryIllustItemStoreAndNotifyInTempStage();

	bool isOnGalaxyFlagIllustItem(const char *pName);
	void onGalaxyFlagIllustItem(const char *pName);
	bool hasIllustItemAndTicoCoinInStage(const char *pName);
	s32 calcIllustItemNum();
};
