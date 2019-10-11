#pragma once
#include "utility.hpp"

namespace sm {
    // Actual implementation without bounds checking
    namespace detail {
        inline constexpr float taylor_sine_impl(const float angle) {
            // Taylor polynomial for sin(x) centered around 0 (n=4):
            // x - ((x^3)/3!) + ((x^5)/5!)
            const auto x2 = angle * angle;
            const auto x3 = x2 * angle;
            const auto x5 = x3 * x2;

            return (angle - (x3 * (1.0F / factorial<float>(3))) +
                    (x5 * (1.0F / factorial<float>(5))));
        }

        inline constexpr float taylor_cosine_impl(const float angle) {
            // Taylor polynomial for cos(x) centered around π/2 (n=4):
            // -(x-π/2) + ((x-π/2)^3)/3! - ((x-π/2)^5)/5!
            const auto x = angle - HALF_PI;
            const auto x2 = x * x;
            const auto x3 = x2 * x;
            const auto x5 = x3 * x2;

            return (-x + (x3 * (1.0F / factorial<float>(3))) -
                    (x5 * (1.0F / factorial<float>(5))));
        }
    } // namespace detail

    // Bounds checked interface
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