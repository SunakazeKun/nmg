#include "neo/System/DaredevilModeAccess.h"

namespace {
	// Set 1 health point
	kmCall(0x803B74F4, neo::isDaredevilModeOrComet);


	// Don't increase health in Daredevil Mode
	bool canHealMarioActor(const MarioActor *pMarioActor) {
		return pMarioActor->isEnableNerveChange() && !neo::isDaredevilMode();
	}

	kmCall(0x803B9174, canHealMarioActor);


	// Disable low health sound
	kmCall(0x803C354C, neo::isDaredevilModeOrComet);


	// Don't disable Coins in Daredevil mode
	kmCall(0x801B3AE4, neo::isNotDaredevilModeAndComet); // KickStone::exeBreak
	kmCall(0x8028C2EC, neo::isNotDaredevilModeAndComet); // Coin::makeActorAppeared
	kmCall(0x8028EC14, neo::isNotDaredevilModeAndComet); // CoinHolder::appearCoin
	kmCall(0x8028F198, neo::isNotDaredevilModeAndComet); // CoinHolder::getDeclareRemnantCoinCount
	kmCall(0x80295488, neo::isNotDaredevilModeAndComet); // MR::tryCreateDummyModel


	// Sandy creates Coin projectiles in Daredevil mode
	kmCall(0x800FD4F8, neo::isNotDaredevilModeAndComet);


	// Disable BenefitItemLifeUp
	kmCall(0x8027634C, neo::isDaredevilModeOrComet);


	// Don't give Life-Up in Daredevil mode or comet
	kmBranch(0x800502E4, neo::changeMarioItemStatusIfNotDaredevil);
}
