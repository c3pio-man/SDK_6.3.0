#ifndef NONCOPYABLE
#define NONCOPYABLE

namespace pocketbook {
namespace utilities {

struct NonCopyable {
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable & operator=(const NonCopyable&) = delete;
};

} // namespace utilities
} // namespace pocketbook

#endif // NONCOPYABLE

