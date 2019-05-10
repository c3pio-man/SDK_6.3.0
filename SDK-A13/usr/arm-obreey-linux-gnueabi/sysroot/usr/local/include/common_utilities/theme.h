#pragma once
#include <string>
namespace pocketbook {
namespace system {
class Theme
{
public:
    static float GetFloat(const std::string & key, float def);
};
}
}
