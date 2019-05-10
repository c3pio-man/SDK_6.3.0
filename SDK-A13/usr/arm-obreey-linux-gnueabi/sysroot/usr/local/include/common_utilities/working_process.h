#pragma once

#include <sys/types.h>
#include <unistd.h>
#include <memory>
#include <queue>
#include <errno.h>
#include <functional>

namespace pocketbook {
namespace utilities {
namespace process {

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

/**
 * @brief The WorkingProcess class. Create a new process and run functions from queue. 
 * Queue must be set before calling function to run a new process
 * When process has been finished a function it send evenet EVT_CALLBACK to main process with
 * parameters : par1 = command, par2 = function id;
 * U can't run more then one process.
 */
class WorkingProcess
{
    struct QueueItem {
        QueueItem() : task_id_(0), cmd_(0) {}
        QueueItem(std::shared_ptr<IRunnable> f, const int cmd_id, const int task_id) : f_(f), task_id_(task_id), cmd_(cmd_id) {}
        const std::shared_ptr<IRunnable> GetTask() const {return f_;} 
        int GetTaskId() const {return task_id_;}
        int GetCommand() const {return cmd_;}
    private:
        std::shared_ptr<IRunnable> f_;
        int task_id_;
        int cmd_;
    };

public:
    
    WorkingProcess();
    ~WorkingProcess();
    int Run();
    void EnqueueTask(std::shared_ptr<IRunnable> task, const int task_id, const int cmd);
    void Stop();
    bool TasksQueueEmpty();
    bool IsRuning() const {return is_running_;}
private:
    int Work();
    static void SignalHandler(int signum);
    bool is_running_;
    std::shared_ptr<std::queue<QueueItem> > task_queue_;
    pid_t pid_;
    static WorkingProcess *s_pThis_;
};



/*******************************************************************************
 * class WorkingProcess_c11
 *******************************************************************************/

class Task
{
public:
    Task(std::function<void()> task)
    : task_void_(task)
    {
        SetIsVoidFlag(true);
    }
    
    Task(std::function<int()> task)
    : task_int_(task)
    {
        SetIsVoidFlag(false);
    }
    
    virtual ~Task() {}
    
    int Run() {
        if(this->IsVoid()) {
            task_void_();
            return 0;
        } else {
            return task_int_();
        }
    }
    
    bool IsVoid() const {return is_void_;}
    
    explicit operator bool() const noexcept 
    {
        if (IsVoid()) return task_void_ ? true : false;
        else return task_int_ ? true : false;
    }
private:
    void SetIsVoidFlag(const bool flag) {is_void_ = flag;}    
    
private:
    std::function<void()> task_void_;
    std::function<int()> task_int_;
    bool is_void_ = false;
};

class WorkingProcess_c11
{
    struct QueueItem {
        QueueItem(const Task &f, const int cmd_id, const int task_id) : f_(f), task_id_(task_id), cmd_(cmd_id) {}
        const Task GetTask() const {return f_;} 
        int GetTaskId() const {return task_id_;}
        int GetCommand() const {return cmd_;}
    private:
        Task f_;
        int task_id_;
        int cmd_;
    };
    enum ePipeId {
        kRead = 0,
        kWrite = 1
    };
public:
    WorkingProcess_c11();
    ~WorkingProcess_c11();
    
    // first param is a task id in queue, second is an error number
    typedef std::function<void(int, int)> ErrorHandlerType;
    
    /**
     * @brief SetErrorHandler - set callback function, that will be called when some error happened
     * @param fn - callback function. First parameter is a task id, second is an error number
     */
    void SetErrorHandler(ErrorHandlerType fn) {fn_callbac_err_ = fn;}
    
    /**
     * @brief EnqueueTask - add task to a queue. 
     * @param task - task to run
     * @param task_id - task id will be sent to parent process in EVT_CALLBACK as par2
     * @param cmd - command id. This command will be sent to parent process int EVT_CALLBACK as par1, when task has finished
     * @return - return PB_OK if task was added or PB_FAIL in otherwise
     */
    int EnqueueTask(const Task &task, const int task_id, const int cmd);
    
    /**
     * @brief Run - fork from parent process and run tasks from queue step by step
     * @return - 0 if all right, or PB_FAILED in otherwise
     */
    int Run();
    
    /**
     * @brief Stop - kill current process and clear queue of tasks 
     */
    void Stop();
    
    /**
     * @brief TasksQueueEmpty
     * @return 
     */
    bool TasksQueueEmpty();
    
    /**
     * @brief IsRuning
     * @return 
     */
    bool IsRuning() const {return is_running_;}
    
    /**
     * @brief GetChildPid
     * @return
     */
    int GetChildPid() const {return pid_;}

private:
    int Work();
    static void SignalHandler_c11(int signum);

private:
    bool is_running_ = false;
    std::shared_ptr<std::queue<QueueItem> > task_queue_;
    pid_t pid_ = 0;
    ErrorHandlerType fn_callbac_err_;
    int pipefd_[2];
    static WorkingProcess_c11 *s_pWorker_;
};

} // namespace process
} // namespace utilities
} // namespace pocketbook
