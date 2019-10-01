// Copyright(c) 2019-present, Anton Lilja.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once
#include "common.hpp"
#include "type_traits.hpp"

#include <type_traits>

namespace sm {
    template <typename T, typename F>
    inline constexpr T round_up_to(const F f) {
        static_assert(std::is_arithmetic<T>::value,
                      "round_up_to requires arithmetic type");

        static_assert(std::is_floating_point<F>::value,
                      "round_up_to requires f to be a floating point type");

        return static_cast<T>(f + static_cast<F>(0.5));
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
                      "round_up_to requires arithmetic type");

        return degrees * (PI / static_cast<F>(180.0));
    }

    template <typename F>
    inline constexpr auto to_degrees(const F radians) {
        static_assert(std::is_arithmetic<F>::value,
                      "round_up_to requires arithmetic type");

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

    namespace detail {
        inline constexpr float taylor_sine_impl(const float angle) {
            // Taylor polynomial for sin(x) centered around 0 (n=4):
            // x - ((x^3)/3!) + ((x^5)/5!)
            const auto x2 = angle * angle;
            const auto x3 = x2 * angle;
            const auto x5 = x3 * x2;

            return (angle - (x3 / factorial<float>(3)) +
                    (x5 / factorial<float>(5)));
        }

        inline constexpr float taylor_cosine_impl(const float angle) {
            // Taylor polynomial for cos(x) centered around π/2 (n=4):
            // -(x-π/2) + ((x-π/2)^3)/3! - ((x-π/2)^5)/5!
            const auto x = angle - HALF_PI;
            const auto x2 = x * x;
            const auto x3 = x2 * x;
            const auto x5 = x3 * x2;

            return (-x + (x3 / factorial<float>(3)) -
                    (x5 / factorial<float>(5)));
        }
    } // namespace detail

    template <bool negative_pi_pi_bounded = false,
              bool negative_hpi_hpi_bounded = false, typename Angle>
    inline constexpr float taylor_sine(const Angle angle) {
        static_assert(std::is_same<Angle, radians>::value ||
                          std::is_same<Angle, degrees>::value,
                      "taylor_sine requires an angle "
                      "type, i.e sm::radians or sm::degrees");

        auto a = static_cast<float>(std::is_same<Angle, radians>::value
                                        ? angle
                                        : radians::from_degrees(angle));

        // Is angle bounded between -π and π?
        if constexpr (!negative_pi_pi_bounded) {
            // Constrain a between -π and π
            a -= TAU * std::floor((a + PI) * (1.0F / TAU));
        }

        // Is angle bounded between -π/2 and π/2?
        if constexpr (!negative_hpi_hpi_bounded) {
            // Is angle between π/2 and π?
            if (a > HALF_PI) {
                return detail::taylor_sine_impl(PI - a);
            }
            // Is angle between -π/2 and -π?
            else if (a < -HALF_PI) {
                return -detail::taylor_sine_impl(PI + a);
            }
        }

        return detail::taylor_sine_impl(a);
    }

    template <bool zero_tau_bounded = false, bool zero_pi_bounded = false,
              typename Angle>
    inline constexpr float taylor_cosine(const Angle angle) {
        static_assert(std::is_same<Angle, radians>::value ||
                          std::is_same<Angle, degrees>::value,
                      "taylor_cosine requires an angle "
                      "type, i.e sm::radians or sm::degrees");

        auto a = static_cast<float>(std::is_same<Angle, radians>::value
                                        ? angle
                                        : radians::from_degrees(angle));

        // Is angle bounded between 0 and τ?
        if constexpr (!zero_tau_bounded) {
            // Constrain a between 0 and 2π
            a -= TAU * std::floor(a * (1.0F / TAU));
        }

        // Is angle between 0 and π?
        if constexpr (!zero_pi_bounded) {
            // Is a is between π and 2π?
            if (a > PI) {
                return -detail::taylor_cosine_impl(a - PI);
            }
        }

        return detail::taylor_cosine_impl(a);
    }
} // namespace sm
