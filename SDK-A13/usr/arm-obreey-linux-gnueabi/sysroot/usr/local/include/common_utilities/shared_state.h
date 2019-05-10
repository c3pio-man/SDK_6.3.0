#ifndef SHARED_STATE_H
#define SHARED_STATE_H
#include <string>
#include <semaphore.h>
namespace pocketbook
{
namespace utilities
{

class SharedStateImpl
{
public:
    SharedStateImpl();
    ~SharedStateImpl();
    int Init(const std::string& name, size_t data_size);
    char* GetState() { if (!shared_state_) return NULL; return shared_state_->data_; }
    void LockState();
    void UnlockState();
private:
    struct State {
        sem_t sem_;
        char data_[];
    };
    State* shared_state_;
    std::string name_;
    int size_;
};

template <typename T>
class SharedState
{
public:
    SharedState(){}
    int Init(const std::string& name) { return impl_.Init(name, sizeof(T)); }
    void LockState() { impl_.LockState(); }
    void UnlockState() { impl_.UnlockState(); }
    T* GetState() { return (T*)impl_.GetState(); }
private:
    SharedStateImpl impl_;
};

} // namespace utilities
} // namespace pocketbook

#endif // SHARED_STATE_H
