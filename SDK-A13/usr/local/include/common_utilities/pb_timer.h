/**
  \brief This file contains timer implementation.

  \author Yuriy Lisovyy
  */


#ifndef PB_OSY_TIMER_H
#define PB_OSY_TIMER_H

#include "pthread.h"
#include <inkview.h>
namespace pocketbook
{
namespace utilities
{

class Timer;

/**
  \brief Callback interface for timer object
  */
class ITimerHandler
{
public:
    /**
      \brief Called when timer event occurs. Calling thread can be canselled so implementation should
        take cancelation points into consideration
    */
    virtual void Handle(Timer* timer) = 0;
    virtual ~ITimerHandler(){};
};

/**
  Function InitTimers should be called in thread safe manner before timers usage
  */
class TimerManager;

/**
  \brief Call handler in predefined t
  */
class Timer
{
 public:
    friend class TimerManager;
    Timer();
    ~Timer() { Stop();}

    static void InitTimers(void);
    int Init(int timeout_ms, bool isRepeatable, ITimerHandler* handler);
    int Stop();

private:

    int WhenToWakeUp();
    void WakeUp();

    static TimerManager * timer_manager_;
    int timeout_;
    struct timespec wakeup_time_;
    bool is_repeatable_;
    ITimerHandler* handler_;
};
class AbstractTimer
{
public:
    explicit AbstractTimer(int delay = 0):m_delay(delay), m_singleShot(true), m_shouldStop(false) {
    }
    virtual ~AbstractTimer() {Stop();}
    bool IsRunning() const;
    void Start(int ms);
    void Start() {
        Start(m_delay);
    }
    void Stop();
    void SetSingleShot(bool set = true);

protected:
    virtual void SetTimerEx(const char* name, iv_timerprocEx tproc, void* context, int ms) = 0;
    virtual void privateActivate() = 0;
    static void timerprocEx(void* context);//just inkview thread 
    int m_delay;
    bool m_singleShot;
    bool m_shouldStop;
};

template<class Functor>
class CommonTimer : public AbstractTimer
{
public:
    CommonTimer(Functor f):m_functor(f) {
    }
    CommonTimer() {
    }
    void SetFunctor(Functor functor) {
        m_functor = functor;
    }
    
private:
    virtual void privateActivate() {
        m_functor();
    }
    Functor m_functor;
};
template<class Functor>
class WeakTimer : public CommonTimer<Functor>
{
    typedef CommonTimer<Functor> TBase;
public:
    WeakTimer(Functor f):CommonTimer<Functor>(f) {
    }
    WeakTimer(){}
    void SetTimerEx(const char* name, iv_timerprocEx tproc, void* context, int ms)
    {
        SetWeakTimerEx(name, tproc, context, ms);
    }
};
template<class Functor>
class HardTimer : public CommonTimer<Functor>
{
    typedef CommonTimer<Functor> TBase;
public:
    HardTimer(Functor f):CommonTimer<Functor>(f) {
    }
    HardTimer(){}
    void SetTimerEx(const char* name, iv_timerprocEx tproc, void* context, int ms)
    {
        SetHardTimerEx(name, tproc, context, ms);
    }
};
} // namespace utilities
} // namespace pocketbook

#endif // PB_OSY_TIMER_H
