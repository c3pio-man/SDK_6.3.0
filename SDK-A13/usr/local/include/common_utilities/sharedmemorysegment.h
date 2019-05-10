#pragma once
#include <string>
#include <semaphore.h>
namespace pocketbook
{
namespace utilities
{
class SharedMemorySegmentImpl
{
public:
    SharedMemorySegmentImpl();
    ~SharedMemorySegmentImpl();
    int Init(const std::string& name, size_t data_size);
    int Init(std::string *name, size_t data_size);
    char* GetState() { if (!shared_state_) return NULL; return shared_state_->data_; }
    bool NeedRemap();
    void Remap();
    void Remap(size_t size);
    void LockState();
    void UnlockState();
    void Clear();
    struct State {
        sem_t sem_;
        int size_;
        char data_[];
    };
    State* shared_state_;
    size_t data_size_;
    std::string key_;
};

template <typename T>
class SharedMemorySegment
{
public:
    SharedMemorySegment() {}
    int Init(const std::string& name, int size = -1) { return impl_.Init(name, size == -1 ? sizeof(T) : size); }
    int Init(std::string *name, int size = -1) { return impl_.Init(name, size == -1 ? sizeof(T) : size); }
    bool NeedRemap() { return impl_.NeedRemap(); }
    void Remap() { return impl_.Remap(); }
    void Remap(size_t size) { return impl_.Remap(size); }
    void LockState() { impl_.LockState(); }
    void UnlockState() { impl_.UnlockState(); }
    T* Data() { return (T*)impl_.GetState(); }
    void Clear() { impl_.Clear(); }
    std::string Key() const {return impl_.key_; }
    size_t size() const {return impl_.data_size_; }
private:
    SharedMemorySegmentImpl impl_;
};
template <typename T>
class SharedMemoryLocker {
public:
    SharedMemoryLocker(SharedMemorySegment<T> * object):
        m_lockedObject(object)
    {
        m_lockedObject->LockState();
    }
    ~SharedMemoryLocker() {
        m_lockedObject->UnlockState();
    }
private:
    void operator=(SharedMemoryLocker<T> &){}
    SharedMemoryLocker(SharedMemoryLocker<T> &){}
    SharedMemorySegment<T> * m_lockedObject;

};
} // namespace utilities
} // namespace pocketbook
