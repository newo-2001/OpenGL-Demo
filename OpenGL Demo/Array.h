#pragma once

#include <memory>

template<typename T>
struct Array
{
    std::unique_ptr<T[]> Data;
    size_t Length;
};
