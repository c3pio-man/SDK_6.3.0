#pragma once
#include <map>
namespace pocketbook{
namespace utilities {
template<class FIRST, class SECOND> 
struct TwoSideMap
{
    struct ELEMENT {
        FIRST first;
        SECOND second;
    };

    TwoSideMap(ELEMENT * elements, size_t size)
    {
        for (size_t i = 0; i < size; i++) {
            first2Second[elements[i].first] = elements[i].second;
            second2First[elements[i].second] = elements[i].first;
        }
    }
    std::map<FIRST, SECOND> first2Second;
    std::map<SECOND, FIRST> second2First;
};
}
}