#include "syati.h"

class StringSpider : public LiveActor {
public:
    StringSpider(const char *pName);

    u8 _90[0xD0 - 0x90];
};

namespace neo {
    class StringSpiderAttachPoint : public LiveActor {
    public:
        StringSpiderAttachPoint(const char *pName);

        virtual void init(const JMapInfoIter &rIter);
        virtual void control();

        void initClipping(const JMapInfoIter &rIter);

        StringSpider* mStringSpider;
        TVec3f mClippingCenter;
    };
};
