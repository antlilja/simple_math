#pragma once
#include "constants.hpp"

#include <type_traits>

namespace sm {
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
} // namespace sm