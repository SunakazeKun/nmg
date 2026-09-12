#pragma once

#include "syati.h"

namespace neo {
	class IllustItem : public LiveActor {
	public:
		IllustItem(const char *pName);

		virtual void init(const JMapInfoIter &rIter);
		virtual void initAfterPlacement();
		virtual void appear();
		virtual void control();
		virtual void calcAndSetBaseMtx();
		virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver);
		virtual bool receiveOtherMsg(u32 msg, HitSensor *pSender, HitSensor *pReceiver);

		void doGetItem();
		void exeShow();
		void exeHide();

		MapObjConnector* mConnector;
		f32 mRotateY;
		bool mCollected;
		TMtx34f mRotateXMtx;
		s32 mNearPointsCount;
	};

	namespace NrvIllustItem {
		NERVE(NrvShow);
		NERVE(NrvHide);
	};
};

#define NrvIllustItemShow &neo::NrvIllustItem::NrvShow::sInstance
#define NrvIllustItemHide &neo::NrvIllustItem::NrvHide::sInstance
