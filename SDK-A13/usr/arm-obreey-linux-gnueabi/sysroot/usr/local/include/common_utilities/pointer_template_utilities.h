#pragma once
#include <vector>
namespace pocketbook
{
namespace utilities
{

template <typename T>
void ClearVectorPointers(std::vector<T *> * vector) {
    if (vector) {
        for (size_t i =0; i < vector->size(); i++) {
            delete (*vector)[i];
        }
    }
    vector->clear();
}
}
}
