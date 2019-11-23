// Copyright(c) 2019-present, Anton Lilja.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once
#include <cmath>
#include <cstdint>

namespace sm {
#ifdef SIMPLE_MATH_FORCE_ALIGNMENT
#    define SIMPLE_MATH_ALIGN(x) alignas(x)
#else
#    define SIMPLE_MATH_ALIGN(x)
#endif
} // namespace sm