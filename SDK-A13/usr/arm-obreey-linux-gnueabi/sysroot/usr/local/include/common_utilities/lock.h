/**
  \brief ScopedLock class to use as RAII for mutex (automatically unlock on destruction)

  \author Yuriy Lisovyy
  */
#ifndef LOCK_H
#define LOCK_H

#include "pthread.h"

namespace pocketbook
{

/**
  \brief Class to lock mutex on scope base. Mutex will be automatically unlocked in destructor
*/
class ScopedLock
{
public:
    explicit ScopedLock(pthread_mutex_t* mutex)
        : mutex_(mutex)
        , is_mutex_locked_(false)
    {
        pthread_mutex_lock(mutex_);
        is_mutex_locked_ = true;
    }

    /**
      \brief unlock the lock explicitly
      */
    void unlock()
    {
        if (is_mutex_locked_) {
            pthread_mutex_unlock(mutex_);
            is_mutex_locked_ = false;
        }
    }

    ~ScopedLock()
    {
        if (is_mutex_locked_) pthread_mutex_unlock(mutex_);
    }
private:
    pthread_mutex_t* mutex_;
    bool is_mutex_locked_;

};

class ScopedRWLock
{
public:
    
    enum eLockType {
        kWriteLock = 1,
        kReadLock,
    };
    
    explicit ScopedRWLock(pthread_rwlock_t *rwlock, const eLockType lock_type)
        : rwlock_(rwlock)
        , is_rwlock_locked_(false)
    {
        switch(lock_type) {
        case kWriteLock:
            pthread_rwlock_wrlock(rwlock_);
            break;
        case kReadLock:
            pthread_rwlock_rdlock(rwlock_);
            break;
        default:
            break;
        }
        is_rwlock_locked_ = true;
    }

    /**
      \brief unlock the lock explicitly
      */
    void unlock()
    {
        if (is_rwlock_locked_) {
            pthread_rwlock_unlock(rwlock_);
            is_rwlock_locked_ = false;
        }
    }

    ~ScopedRWLock()
    {
        if (is_rwlock_locked_) {
            pthread_rwlock_unlock(rwlock_);
        }
    }
private:
    pthread_rwlock_t *rwlock_;
    bool is_rwlock_locked_;

};

} // namespace pocketbook
#endif // LOCK_H
