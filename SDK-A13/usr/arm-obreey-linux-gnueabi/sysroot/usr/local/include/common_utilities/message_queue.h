#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include "pthread.h"
#include "pblog.h"
#include "pb_error.h"
#include "lock.h"
#include <deque>
#include <queue>
#include <time.h>
#include <errno.h>

namespace pocketbook
{
namespace utilities
{

inline struct timespec GetTimeoutAbsTime(int timeout_ms)
{
    const long kNanoSecondsInSecond = 1000L * 1000L * 1000L;
    timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    time.tv_nsec += (timeout_ms % 1000L) * 1000L * 1000L;
    time.tv_sec += timeout_ms / 1000;
    if (time.tv_nsec >= kNanoSecondsInSecond) {
        time.tv_sec += time.tv_nsec / kNanoSecondsInSecond;
        time.tv_nsec = time.tv_nsec % kNanoSecondsInSecond;
    }
    return time;
}

template<typename T> class IPushable
{
public:
    virtual int Push(const T& item) = 0;
    virtual int PushFront(const T& item) = 0;

    virtual ~IPushable(){}
};

template<typename T>
class MessageQueue: public IPushable<T>
{
public:
    MessageQueue():is_inited_(false){}
    ~MessageQueue()
    {
        if (is_inited_) {
            pthread_cond_destroy(&cond_);
            pthread_mutex_destroy(&mutex_);
        }
    }

    int Init()
    {
        PB_FRETURN_IF_NOT(false == is_inited_, PB_WRONG_STATE);
        int err = pthread_mutex_init(&mutex_, NULL);
        PB_FRETURN_IF_NOT(0 == err, PB_FAILED);

        err = pthread_cond_init(&cond_, NULL);
        if (0 != err) {
            PB_ERROR("pthread_cond_init failed with [%d]", err);
            pthread_mutex_destroy(&mutex_);
            return PB_FAILED;
        }

        is_inited_ = true;
        return PB_OK;
    }

    int Push(const T& item)
    {
        PB_FRETURN_IF_NOT(is_inited_, PB_NOT_IMPEMENTED);

        pocketbook::ScopedLock lock(&mutex_);
        queue_.push_back(item);
        pthread_cond_signal(&cond_);
        return 0;
    }

    int PushFront(const T& item)
    {
        PB_FRETURN_IF_NOT(is_inited_, PB_NOT_IMPEMENTED);

        pocketbook::ScopedLock lock(&mutex_);
        queue_.push_front(item);
        pthread_cond_signal(&cond_);
        return 0;
    }

    int IsEmpty(void)
    {
        PB_FRETURN_IF_NOT(is_inited_, PB_NOT_IMPEMENTED);
        pocketbook::ScopedLock lock(&mutex_);
        return queue_.empty();
    }

    int Pop(T* item)
    {
        PB_FRETURN_IF_NOT(is_inited_, PB_NOT_IMPEMENTED);
        pocketbook::ScopedLock lock(&mutex_);
        if (!queue_.empty()) {
            *item = queue_.front();
            queue_.pop_front();
            return PB_OK;
        } else {
            return PB_FAILED;
        }
    }

    int Wait(long timeout_ms, T* item)
    {
        PB_FRETURN_IF_NOT(is_inited_, PB_NOT_IMPEMENTED);
        pocketbook::ScopedLock lock(&mutex_);
        int err = 0;

        struct timespec time = GetTimeoutAbsTime(timeout_ms);

        while (queue_.empty()) {
            if (timeout_ms) {
                err = pthread_cond_timedwait(&cond_, &mutex_, &time);
            } else {
                err = pthread_cond_wait(&cond_, &mutex_);
            }
            if (ETIMEDOUT == err) {
                break;
            } else if (err) {
                PB_ERROR("pthread_cond_timedwait failed with [%d]", err);
                break;
            }
        }

        if (!queue_.empty()) {
            *item = queue_.front();
            queue_.pop_front();
            return PB_OK;
        } else {
            return PB_FAILED;
        }

    }
    
    int Clear() {
        PB_FRETURN_IF_NOT(is_inited_, PB_NOT_IMPEMENTED);
        pocketbook::ScopedLock lock(&mutex_);
        queue_.clear();
        return PB_OK;
    }

private:
    std::deque<T> queue_;
    pthread_mutex_t mutex_;
    pthread_cond_t cond_;
    bool is_inited_;
};

} // namespace utilities
} // namespace pocketbook

#endif // MESSAGE_QUEUE_H
