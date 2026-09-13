#include "Game/Screen/InformationObserver.h"

namespace neo {
    bool explainIllustItemIfAtFirst(LiveActor *pHost);

    namespace NrvInformationObserverExt {
        NERVE(NrvDispIllustItem);
    }
};

#define NrvInformationObserverExtDispIllustItem &neo::NrvInformationObserverExt::NrvDispIllustItem::sInstance
