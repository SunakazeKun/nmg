#include "syati.h"

namespace {
    void exeStageResultInformerExtAppearGetStamp(StageResultInformer *pActor);
    void exeStageResultInformerExtCountUpStamp(StageResultInformer *pActor);
    void exeStageResultInformerExtDisplayGetStamp(StageResultInformer *pActor);
    void exeStageResultInformerExtDisappearGetStamp(StageResultInformer *pActor);
};

namespace neo {
    namespace NrvStageResultInformerExt {
        NERVE(NrvAppearGetStamp);
        NERVE(NrvWaitBeforeCountUpStamp);
        NERVE(NrvCountUpStamp);
        NERVE(NrvDisplayGetStamp);
        NERVE(NrvDisappearGetStamp);
    }
};

#define NrvStageResultInformerExtAppearGetStamp &neo::NrvStageResultInformerExt::NrvAppearGetStamp::sInstance
#define NrvStageResultInformerExtWaitBeforeCountUpStamp &neo::NrvStageResultInformerExt::NrvWaitBeforeCountUpStamp::sInstance
#define NrvStageResultInformerExtCountUpStamp &neo::NrvStageResultInformerExt::NrvCountUpStamp::sInstance
#define NrvStageResultInformerExtDisplayGetStamp &neo::NrvStageResultInformerExt::NrvDisplayGetStamp::sInstance
#define NrvStageResultInformerExtDisappearGetStamp &neo::NrvStageResultInformerExt::NrvDisappearGetStamp::sInstance
