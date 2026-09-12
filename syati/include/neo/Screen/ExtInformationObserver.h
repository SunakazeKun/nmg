#include "syati.h"

#define INFORMATION_OBSERVER_EXT_ILLUST_ITEM 0

namespace {
    struct InformationObserverExtInfo {
        const char* mMessageName;
        bool _1;
        const char* mDispBgmName;
        const char* mAfterBgmName;
    };

    const Nerve* getNerveForInformationObserverExt(s32 extType);
    void setupInformationObserverExt(InformationObserver *pActor, s32 extType, LiveActor *pHost, bool pauseTimeKeepHostDemo);
    void exeInformationObserverDispExt(InformationObserver *pActor, s32 extType);
};

namespace neo {
    bool explainIllustItemIfAtFirst(LiveActor *pHost);

    namespace NrvInformationObserverExt {
        NERVE(NrvDispIllustItem);
    }
};

#define NrvInformationObserverExtDispIllustItem &neo::NrvInformationObserverExt::NrvDispIllustItem::sInstance
