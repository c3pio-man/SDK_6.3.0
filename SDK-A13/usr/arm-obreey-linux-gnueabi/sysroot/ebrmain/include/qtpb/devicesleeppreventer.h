#ifndef DEVICESLEEPPREVENTER_H
#define DEVICESLEEPPREVENTER_H

#include <string>

#include "qtpb_global.h"

namespace pocketbook {
namespace utils {

class QTPBSHARED_EXPORT DeviceSleepPreventer
{
public:

    DeviceSleepPreventer();
    ~DeviceSleepPreventer();

    void setName(std::string name);
    std::string getName();

    static void sleepPreventerProc(void *context);

    void start();
    void stop();

private:
    std::string name_;
};

}
}

#endif // DEVICESLEEPPREVENTER_H
