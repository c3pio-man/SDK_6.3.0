#ifndef EINKINTERFACE_H
#define EINKINTERFACE_H

#include <functional>
#include "inkview.h"
#include <map>

namespace pocketbook { namespace utilities {
class EinkPlatformInterface {
public:
    enum class UpdateType {
        None,
        Full,
        FullHQ,
        Partial,
        PartialBW,
        PartialHQ,
        PartialBlack,
        PartialUpdateDU4,
        Soft,
        SoftHQ,
        Dynamic,
        DynamicBW,
    };
    virtual void requireFullUpdate() = 0;
    virtual void enterA2ForRegion(irect region) = 0;
    virtual void leaveA2() = 0;
    virtual bool isInA2() = 0;
    virtual void timedLeaveA2(int timeout_ms) = 0;
    virtual void setAllowRedraw(bool value) = 0;
    virtual void setAllowRedrawEx(bool value, bool force_redraw) = 0;
    virtual void setDrawPanelFunc(void (*draw_panel_func)(void *context), void *context) = 0;
    virtual void requireUpdateType(UpdateType updateType) = 0;
    virtual void setHandlerForInkvewEvents(std::function<void(int, int, int)> f) = 0;
    virtual void a2ForFlick(irect region) = 0;
    virtual void a2ForPinch(irect region) = 0;
    virtual void setGamma(double gamma, int y_offset) = 0;
    virtual void clearImageRects() = 0;
    virtual void addImageRect(int x, int y, int w, int h) = 0;
    virtual void replateHoldKeysByPress(std::map<int, int> key_correspondance) = 0;
    virtual void setKeyFilter(std::function<bool(int /*event*/, int /*key*/)> filter) = 0;
    virtual void setQuickKeyboardEnterUpdateMode(bool value) = 0;
    virtual void forceUpdatePanel() = 0;
    // mode:
    // 0 - test to possibility do redraw now (timeout between updates or something else)
    // 1 - do update
    virtual void setUpdateManualFunc(bool (*update_manual_func)(void *context, int mode), void *context) = 0;
    virtual void setKeyboardUpdateAfterClose(bool value) = 0;
    virtual void setAdditionalKeyboardFlags(int flags) = 0;
};

class EinkInterface
{
    EinkInterface();
public:
    static EinkInterface * instance();
    ~EinkInterface();
    static void setPlatformInterface(EinkPlatformInterface* interface);
    EinkPlatformInterface* platform() { return platform_; }
private:
    EinkPlatformInterface* platform_;
};

inline EinkPlatformInterface* einkPlatform() {
    return EinkInterface::instance()->platform();
}

}}

#endif // EINKINTERFACE_H

