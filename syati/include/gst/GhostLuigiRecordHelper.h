#pragma once

#include "gst/GstRecordHelper.h"

namespace gst {
    class GhostLuigiRecordHelper : public GstRecordHelper {
    public:
        GhostLuigiRecordHelper(const char *pName);

        virtual GhostType getGhostType() const;
        virtual void initModelAndScene(const JMapInfoIter &rIter);
        virtual void handlePrepare();
        virtual void supplyGstFrame() const;
    };
}
