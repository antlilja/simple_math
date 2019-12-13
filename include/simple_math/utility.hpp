// Copyright(c) 2019-present, Anton Lilja.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once
#include "common.hpp"
#include "constants.hpp"
#include "type_traits.hpp"

#include <type_traits>

namespace sm {
    template <typename To, typename From>
    inline constexpr To ceil_positive_to(const From f) {
        static_assert(std::is_arithmetic<To>::value,
                      "ceil_positive_to requires To to be an arithmetic type");

        static_assert(
            std::is_floating_point<From>::value,
            "ceil_positive_to requires From to be a floating point type");

        return static_cast<To>(f + static_cast<From>(0.5));
    }

    template <typename T>
    inline constexpr T square(const T x) {
        static_assert(std::is_arithmetic<T>::value,
                      "square requires arithmetic type");

        return x * x;
    }

    template <class T, typename F>
    inline constexpr T lerp(const T a, const T b, const F t) {
        static_assert(std::is_floating_point<F>::value,
                      "lerp requires t to be a floating point type");

        static_assert(detail::has_addition_v<T>,
                      "lerp requires a and b to have addition operator");

        static_assert(
            detail::has_multiplies_v<T, F>,
            "lerp requires a and b to have multiplication operators with t");

        return (a * (static_cast<F>(1.0) - t)) + (b * t);
    }

    template <typename T>
    inline constexpr T max_of(const T x) {
        static_assert(std::is_arithmetic<T>::value,
                      "max_of requires arithmetic type");

        return std::numeric_limits<T>::max(x);
    }

    template <typename F>
    inline constexpr auto to_radians(const F degrees) {
        static_assert(std::is_arithmetic<F>::value,
                      "to_radians requires arithmetic type");

        return degrees * (PI / static_cast<F>(180.0));
    }

    template <typename F>
    inline constexpr auto to_degrees(const F radians) {
        static_assert(std::is_arithmetic<F>::value,
                      "to_degrees requires arithmetic type");

        return radians * (static_cast<F>(180.0) / PI);
    }

    inline constexpr float fast_inverse_sqrt(const float x) {
        union {
            float f;
            uint32_t u;
        } y = {x};
        y.u = 0x5F3759DFul - (y.u >> 1);
        return 0.5f * y.f * (3.0f - x * y.f * y.f);
    }

    template <typename T>
    inline constexpr T factorial(T n) {
        static_assert(std::is_arithmetic<T>::value,
                      "factorial requires arithmetic an type");

        T fac = static_cast<T>(1);
        for (; n >= static_cast<T>(1); --n) {
            fac *= n;
        }

        return fac;
    }
} // namespace sm
