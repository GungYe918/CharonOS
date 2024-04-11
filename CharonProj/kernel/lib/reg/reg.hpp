/**
 * @file reg.hpp
*/

#pragma once

#include <cstddef>
#include <cstdint>

template <typename T>
struct ArrayLength {};

template <typename T, size_t N>
struct ArrayLength<T[N]> {
    static const size_t value = N;
};
