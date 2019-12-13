#pragma once
#include <cstdint>
#include <immintrin.h>
#include <smmintrin.h>
#include <xmmintrin.h>

namespace sm {
    enum class simd_t : uint8_t { NONE, SSE2, SSE4, AVX };

    namespace detail {
#if defined(SIMPLE_MATH_AVX)
        constexpr simd_t default_simd = simd_t::AVX;
#elif defined(SIMPLE_MATH_SSE4)
        constexpr simd_t default_simd = simd_t::SSE4;
#elif defined(SIMPLE_MATH_SSE2)
        constexpr simd_t default_simd = simd_t::SSE2;
#else
        constexpr simd_t default_simd = simd_t::NONE;
#endif

        constexpr __m128 m128_signmask = {-0.0F, -0.0F, -0.0F, -0.0F};
    } // namespace detail
} // namespace sm