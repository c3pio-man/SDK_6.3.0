#ifndef TIME_MEASUREMENT_H
#define TIME_MEASUREMENT_H

#include <time.h>

namespace pocketbook
{
namespace utilities
{

class TimeMeasurement
{
public:
    TimeMeasurement() {
        clock_gettime(CLOCK_MONOTONIC, &ts_beg);
    }

    double elapsed() {
        clock_gettime(CLOCK_MONOTONIC, &ts_end);
        time_diff = (ts_end.tv_sec - ts_beg.tv_sec) + ((ts_end.tv_nsec - ts_beg.tv_nsec) / 1e9);
        return time_diff;
    }

    void start() {
        clock_gettime(CLOCK_MONOTONIC, &ts_beg);
    }

private:
    struct timespec ts_beg, ts_end;
    double time_diff;
};

}
}
#endif // TIME_MEASUREMENT_H
