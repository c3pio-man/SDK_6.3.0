#pragma once
#include <functional>
namespace pocketbook
{
namespace utilities
{
void SingleHardTimerShot(int msec, std::function<void() > functor);
void SingleWeakTimerShot(int msec, std::function<void() > functor);
}
}
