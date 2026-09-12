#include "syati.h"

namespace neo {
    void GreenStarExeGot(PowerStar *pStar);
    void GreenStarExeGotDisappear(PowerStar *pStar);

    namespace NrvGreenStar {
        NERVE(NrvGot);
        NERVE(NrvGotDisappear);
    }
}

#define NrvGreenStarGot &neo::NrvGreenStar::NrvGot::sInstance
#define NrvGreenStarGotDisappear &neo::NrvGreenStar::NrvGotDisappear::sInstance
