#pragma once

#include "Game/LiveActor/LiveActor.h"

namespace neo {
    class EffectObjLinePoint;

    class EffectObjLine : public LiveActor {
    public:
        EffectObjLine(const char *pName);

        virtual void init(const JMapInfoIter &rIter);
        virtual void makeActorAppeared();
        virtual void makeActorDead();
        virtual void startClipped();
        virtual void endClipped();

        void initPoints(const JMapInfoIter &rIter);
        void initClipping(const JMapInfoIter &rIter);

        const char* mGroupName;
        EffectObjLinePoint** mPointsArray;
        s32 mPointsCount;
    };
    
    class EffectObjLinePoint : public LiveActor {
    public:
        enum PointType {
            StartPoint,
            MiddlePoint,
            EndPoint
        };

        EffectObjLinePoint(EffectObjLine *pHost, PointType pointType, f32 pathCoord);

        virtual void init(const JMapInfoIter &rIter);
        virtual void control();

        const char* getTriggerName() const;

        EffectObjLine* mHost;
        PointType mPointType;
        f32 mPathCoord;
        TPos3f mEffectMtx;
    };
};
