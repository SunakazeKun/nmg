#include "gst/GhostLuigiRecordHelper.h"
#include "gst/GstRecordSystem.h"
#include "Game/Player/MarioAccess.h"
#include "Game/Util/ObjUtil.h"
#include "Game/Util/PlayerUtil.h"

namespace gst {
    GhostLuigiRecordHelper::GhostLuigiRecordHelper(const char *pName)
        : GstRecordHelper(pName)
    {

    }

    GhostType GhostLuigiRecordHelper::getGhostType() const {
        return GhostType_GhostAttackGhost;
    }

    void GhostLuigiRecordHelper::supplyGstFrame() const {
        supplyGstFrameSyncWithPlayer();
    }

    void GhostLuigiRecordHelper::initModelAndScene(const JMapInfoIter &rIter) {
        initModelManagerWithAnm("GhostAttackGhost", NULL, NULL, false);
        MR::connectToScene(this, MR::MovementType_Npc, MR::CategoryList_Null, MR::DrawBufferType_IndirectNpc, MR::CategoryList_Null);
    }

    void GhostLuigiRecordHelper::handlePrepare() {
        MarioAccess::setBaseMtx(getBaseMtx());
        MarioAccess::setStateWait();
        MR::startBckPlayer("Wait", NULL);
        MR::hideModel(this);
    }
}
