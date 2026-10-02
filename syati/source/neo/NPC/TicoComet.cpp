#include "neo/NPC/TicoComet.h"
#include "Game/Util/ActionSoundUtil.h"
#include "Game/Util/ActorAnimUtil.h"
#include "Game/Util/NPCUtil.h"

namespace neo {
    TicoComet::TicoComet(const char *pName)
        : NPCActor(pName)
    {

    }

    void TicoComet::init(const JMapInfoIter &rIter) {
        NPCActorCaps caps("TicoComet");
        NPCActorItem item("TicoComet");

        caps.setDefault();
        caps.mBodySensorRadius = 100.0f;
        caps.mBodySensorOffset.y = 100.0f;
        caps.mBodySensorJoint = "Center";
        caps.mStarPointerTargetJoint = "Center";
        caps.mStarPointerTargetRadius = 100.0f;

        initialize(rIter, caps, nullptr, nullptr, false);

        MR::getNPCItemData(&item, 0);
        equipment(item, true);

        mTalkParam.mActionWait = "Wait";
        mTalkParam.mActionWaitTurn = "Wait";
        mTalkParam.mActionTalk = "Talk";
        mTalkParam.mActionTalkTurn = "Talk";

        mActionSpin = "Spin";
        mActionTrampled = "Trampled";
        mActionPointing = "Pointing";
        mActionReaction = "Reaction";

        MR::startAction(mGoodsModel0, "LeftRotate");
        MR::startAction(mGoodsModel1, "RightRotate");
        MR::startBtk(this, "TicoComet");
        
        MR::startBva(this, "Small0");
        MR::startBrkAndSetFrameAndStop(this, "Normal", 0.0f);
        
        MR::setDistanceToTalk(mTalkCtrl, 350.0f);
    }

    void TicoComet::control() {
        if (mReactTrampleAttack) {
            MR::startActionSound(this, "SmNpcTrampled", -1, -1, -1);
            MR::startActionSound(this, "SvTicocometTrampled", -1, -1, -1);
        }

        if (NPCActor::isPointingSe()) {
            MR::startActionSound(this, "SvTicocometPoint", -1, -1, -1);
        }

        if (mReactSpinAttack) {
            MR::startActionSound(this, "SmTicocometSpin", -1, -1, -1);
        }

        if (mReactStarPieceAttack) {
            MR::startActionSound(this, "SvTicocometStarPieceHit", -1, -1, -1);
        }
        
        NPCActor::control();
    }
}
