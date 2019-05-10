#pragma once

#define PB_IF_CPP11 __cplusplus >= 201103L
#if PB_IF_CPP11
#   define PB_NON_COPYABLE(Name) \
    private:\
        Name(const Name&) = delete; \
        Name& operator=(const Name&) = delete;
#else
#   define PB_NON_COPYABLE(Name) \
    private: \
        Name(const Name&) {} \
        Name& operator=(const Name&) {}
#endif


