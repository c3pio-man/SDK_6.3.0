#pragma once
#include "two_side_map.hpp"
#define TWO_SIDE_MAP_DECLARATION_EXTENDED(FIRST_TYPE, FIRST_NAME, SECOND_TYPE, SECOND_NAME, NAME, ARRAY)                                          \
static struct TWO_SIDE_MAP_##FIRST_NAME##SECOND_NAME##NAME : public pocketbook::utilities::TwoSideMap<FIRST_TYPE, SECOND_TYPE>   \
{                                                                                                   \
    std::map<SECOND_TYPE, FIRST_TYPE> & FIRST_NAME;                                                 \
    std::map<FIRST_TYPE, SECOND_TYPE> & SECOND_NAME;                                                 \
    TWO_SIDE_MAP_##FIRST_NAME##SECOND_NAME##NAME                                                                \
        (pocketbook::utilities::TwoSideMap<FIRST_TYPE, SECOND_TYPE>::ELEMENT * elements, size_t size) :       \
        pocketbook::utilities::TwoSideMap<FIRST_TYPE, SECOND_TYPE>(elements, size),                                                                 \
        FIRST_NAME(second2First),                                                         \
        SECOND_NAME(first2Second)                                                          \
    {}                                                                                              \
    const FIRST_TYPE & to_##FIRST_NAME(const SECOND_TYPE & arg) {return (FIRST_NAME[arg]);}              \
    const SECOND_TYPE & to_##SECOND_NAME(const FIRST_TYPE & arg) {return (SECOND_NAME[arg]);}              \
} NAME(ARRAY, sizeof(ARRAY) / sizeof(ARRAY[0]));                                                                                           \


