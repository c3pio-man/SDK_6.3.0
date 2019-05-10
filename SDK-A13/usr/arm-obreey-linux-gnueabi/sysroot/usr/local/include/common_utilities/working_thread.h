#ifndef WORKING_THREAD_H
#define WORKING_THREAD_H

#include <memory>
#include "pthread.h"

#include "common_utilities/message_queue.h"

namespace pocketbook
{
namespace utilities
{

class IRunnable
{
public:
    virtual void Run() = 0;
    virtual ~IRunnable(){}
};

template <typename F> class TaskWrapper: public IRunnable
{
    F f_;
public:
    TaskWrapper(const F& f): f_(f){}
    virtual void Run()
    {
        f_();
    }

};

template <typename F>
std::shared_ptr<IRunnable> WrapTask(const F& f)
{
    return std::shared_ptr<IRunnable>(new TaskWrapper<F>(f));
}

class WorkingThread
{
    WorkingThread(const WorkingThread&);
    WorkingThread& operator=(const WorkingThread&);
public:
    WorkingThread(bool wait_finish = false):is_running_(false), wait_thread_finish_(wait_finish){}
    ~WorkingThread();
    int Run();
    int EnqueueTask(std::shared_ptr<IRunnable> task);
    int EnqueueTaskFront(std::shared_ptr<IRunnable> task);
    void StopAndDeleteAsync();
    bool TasksQueueEmpty();
    bool IsRunning();
    void ClearQueueTasks();
    void SetLowPriority(); // This doesn't work
    static void SetMeLowPriority(int priority); // This should be called from thread. priority can by from 1 to 19. 19 - maximum slowly
private:
    static void* Runner(void* context);
    int Work(void);
    bool is_running_;
    MessageQueue<std::shared_ptr<IRunnable> > task_queue_;
    pthread_t thread_;
    bool wait_thread_finish_;
};

} // namespace utilities
} // namespace pocketbook



#endif // WORKING_THREAD_H
