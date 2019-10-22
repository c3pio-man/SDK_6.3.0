#ifndef DEVICESLEEPPREVENTER_H
#define DEVICESLEEPPREVENTER_H

#include "ttspm_global.h"

namespace pocketbook {
namespace ttspm {

#define BAN_SLEEP_INTERVAL_MS 7000
#define CHECK_INTERVAL_MS 5000
#define POSTPONE_INTERVAL_COUNT 12

class TTSPMSHARED_EXPORT DeviceSleepPreventer
{
public:
    DeviceSleepPreventer();

    void start();
    void stop();

    void preventTimedShutdown();

    void setCheckIntervalMs(int check_interval_ms);
    void setBanSleepIntervalMs(int ban_sleep_interval_ms);
    void setNeedPostponeTimedPowerOff(bool need_postpone_timed_power_off);

    bool needPostponeTimedPowerOff();
    int banSleepIntervalMs();
    int checkIntervalMs();

private:
    int counter_ = 0;
    int ban_sleep_interval_ms_ = BAN_SLEEP_INTERVAL_MS;
    int check_interval_ms_ = CHECK_INTERVAL_MS;
    int postpone_after_interval_count_ = POSTPONE_INTERVAL_COUNT;
    bool postpone_timed_power_off_ = true;

    static void sleepPreventerProc(void *context);
};

}
}

#endif // DEVICESLEEPPREVENTER_H
