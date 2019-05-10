#ifndef WIFI_ASYNC_WRAPPER_H
#define WIFI_ASYNC_WRAPPER_H

#include "inkview.h"
#include "common_utilities/working_thread.h"
#include "common_utilities/lock.h"
#include "common_utilities/message_queue.h"
#include <set>

namespace pocketbook {
namespace system {

class WifiAsyncWrapper
{
public:
    enum Status {
        kIdle,
        kSwitchingNetworkManager,
        kAdding,
        kSelecting,
        kSwitching, // No current task performed but task queue isn't empty
    };

    enum CallbackTypes {
        kCbNone,
        kCbSwitchNetworkManager,
        kCbAdd,
        kCbSelect,
    };

    class Cb {
    public:
        virtual void HandleWifiResults(WifiAsyncWrapper::CallbackTypes type, int result) = 0;

        virtual ~Cb(){}
    };

    static WifiAsyncWrapper& GetInstance();    // Only 1 instance allowed
    ~WifiAsyncWrapper();
    
    int SwitchNetworkManagerAsync(int status, Cb* cb);
    int AddAsync(const std::string& filename, Cb* cb);
    int SelectAsync(const std::string& ssid, Cb* cb);

    int CancelCallback(Cb* cb);

    Status GetStatus();

    void ProcessResults();
    
    bool IsCallbackCancelled(Cb* cb)
    {
        return cancelled_callbacks_.count(cb) > 0;
    }

private:
    WifiAsyncWrapper();
    int PerformNetManagerSwitching(int status, Cb* cb);
    int PerformAdd(const std::string& filename, Cb* cb);
    int PerformSelect(const std::string& ssid, Cb* cb);

    struct Result
    {
        Cb* cb_;
        CallbackTypes type_;
        int result_;
        Result(Cb* cb, CallbackTypes type, int result): cb_(cb),type_(type), result_(result){}
        Result():cb_(NULL),type_(kCbNone),result_(0){}
    };
    pocketbook::utilities::MessageQueue<Result> results_;
    pocketbook::utilities::WorkingThread thread_;
    pthread_mutex_t mutex_;
    Status status_;
    std::set<Cb*> cancelled_callbacks_;
};


} // namespace system
} // namespace pocketbook

#endif // WIFI_ASYNC_WRAPPER_H
