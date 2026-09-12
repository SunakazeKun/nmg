#pragma once

#include "syati.h"

namespace neo {
	class IllustItemCounter : public LayoutActor {
	public:
		IllustItemCounter();

		virtual void init(const JMapInfoIter &rIter);
		virtual void appear();
		virtual void control();

		void forceAppear();
		void disappear();
		bool isWait();
		void exeHidden();
		void exeAppear();
		void exeWait();
		void exeFlash();
		void exeDisappear();

		CounterLayoutAppearer* mAppearer;
		bool mAlreadyShown;
		bool mForceAppear;
	};

	namespace NrvIllustItemCounter {
		NERVE(NrvHidden);
		NERVE(NrvAppear);
		NERVE(NrvWait);
		NERVE(NrvFlash);
		NERVE(NrvDisappear);
	};
};

#define NrvIllustItemCounterHidden &neo::NrvIllustItemCounter::NrvHidden::sInstance
#define NrvIllustItemCounterAppear &neo::NrvIllustItemCounter::NrvAppear::sInstance
#define NrvIllustItemCounterWait &neo::NrvIllustItemCounter::NrvWait::sInstance
#define NrvIllustItemCounterFlash &neo::NrvIllustItemCounter::NrvFlash::sInstance
#define NrvIllustItemCounterDisappear &neo::NrvIllustItemCounter::NrvDisappear::sInstance
