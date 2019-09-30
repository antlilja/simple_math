// Copyright(c) 2019-present, Anton Lilja.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once
#include <cmath>
#include <cstdint>
#include <cstring>
#include <immintrin.h>
#include <smmintrin.h>
#include <xmmintrin.h>

#if defined(SIMPLE_MATH_ENABLE_ASSERTS)
#    include <iostream>
#endif

namespace sm {
    constexpr float PI = 3.1415926F;

    constexpr __m128 M128_SIGNMASK = {-0.0F, -0.0F, -0.0F, -0.0F};

    class radians {
    public:
        inline constexpr explicit radians(const float angle) : m_angle(angle) {}

        inline static constexpr auto from_degrees(const float angle) {
            return radians(angle * (PI / 180.0F));
        }

        inline constexpr operator float() const { return m_angle; }

    private:
        float m_angle;
    };

    class degrees {
    public:
        inline constexpr explicit degrees(const float angle) : m_angle(angle) {}

        inline static constexpr auto from_radians(const float angle) {
            return degrees(angle * (180.0F / PI));
        }

        inline constexpr operator float() const { return m_angle; }

    private:
        float m_angle;
    };

    enum class simd_t : uint8_t { NONE, SSE2, SSE4, AVX };

    namespace detail {
#if defined(SM_AVX)
        constexpr simd_t default_simd = simd_t::AVX;
#elif defined(SM_SSE4)
        constexpr simd_t default_simd = simd_t::SSE4;
#elif defined(SM_SSE2)
        constexpr simd_t default_simd = simd_t::SSE2;
#else
        constexpr simd_t default_simd = simd_t::NONE;
#endif
    } // namespace detail

#ifdef SIMPLE_MATH_FORCE_ALIGNMENT
#    define SIMPLE_MATH_ALIGN(x) alignas(x)
#else
#    define SIMPLE_MATH_ALIGN(x)
#endif

#ifdef SIMPLE_MATH_ENABLE_ASSERTS
#    if defined(_WIN32)
#        define SIMPLE_MATH_BREAKPOINT __debugbreak();
#    elif defined(__linux__)
#        define SIMPLE_MATH_BREAKPOINT asm("int $3");
#    endif

#    define SIMPLE_MATH_ASSERT(x)                                    \
        {                                                            \
            if (!(x)) {                                              \
                std::cerr << "Assertion: " << #x                     \
                          << " failed in function: " << __FUNCTION__ \
                          << " at line: " << __LINE__                \
                          << " in file: " << __FILE__ << '\n';       \
                SIMPLE_MATH_BREAKPOINT                               \
            }                                                        \
        }
#else
#    define SIMPLE_MATH_ASSERT(x)
#endif
} // namespace sm