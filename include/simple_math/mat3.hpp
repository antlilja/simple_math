// Copyright(c) 2019-present, Anton Lilja.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once
#include "common.hpp"
#include "vec2.hpp"
#include "vec3.hpp"

#include <type_traits>

namespace sm {
    struct mat3 {
        // Data
        union {
            float elements[9];
            vec3 columns[3];
        };

        // Constructors
        inline constexpr mat3()
            : elements{0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F} {}

        inline constexpr mat3(float diagonal) : mat3() {
            elements[0 + 0 * 3] = diagonal;
            elements[1 + 1 * 3] = diagonal;
            elements[2 + 2 * 3] = diagonal;
        }

        inline static constexpr mat3 identity() { return mat3(1.0F); }

        // Transform matrices //

        // Translation
        inline static constexpr mat3 translation(float x, float y) {
            mat3 result = mat3::identity();

            result.elements[0 + 2 * 3] = x;
            result.elements[1 + 2 * 3] = y;

            return result;
        }

        inline static constexpr mat3 translation(const sm::vec2& t) {
            return mat3::translation(t.x, t.y);
        }

        // Rotation
        template <class Angle>
        inline static mat3 rotation(const Angle angle) {
            static_assert(std::is_same<Angle, radians>::value ||
                              std::is_same<Angle, degrees>::value,
                          "mat3 rotation requires parameter angle in an angle "
                          "type, i.e sm::radians or sm::degrees");

            const auto rad = std::is_same<Angle, radians>::value
                                 ? angle
                                 : radians::from_degrees(angle);

            mat3 result(1.0F);
            result.elements[0 + 0 * 3] = cosf(rad);
            result.elements[1 + 0 * 3] = sinf(rad);

            result.elements[0 + 1 * 3] = -sinf(rad);
            result.elements[1 + 1 * 3] = cosf(rad);

            return result;
        }

        // Scale
        inline static constexpr mat3 scale(float x, float y) {
            mat3 result = mat3::identity();

            result.elements[0 + 0 * 3] = x;
            result.elements[1 + 1 * 3] = y;

            return result;
        }

        inline static constexpr mat3 scale(const sm::vec2& s) {
            return mat3::scale(s.x, s.y);
        }
    };

    // Multiply
    inline constexpr mat3 multiply(const mat3& lhs, const mat3& rhs) {
        mat3 result;
        for (int32_t y = 0; y < 3; ++y) {
            for (int32_t x = 0; x < 3; ++x) {
                for (int32_t e = 0; e < 3; ++e) {
                    result.elements[x + y * 3] +=
                        lhs.elements[x + e * 3] * rhs.elements[e + y * 3];
                }
            }
        }
        return result;
    }

    inline mat3 operator*(const mat3& lhs, const mat3& rhs) {
        return multiply(lhs, rhs);
    }

    inline mat3& operator*=(mat3& lhs, const mat3& rhs) {
        return lhs = multiply(lhs, rhs);
    }

    // Compare
    inline constexpr bool compare(const mat3& lhs, const mat3& rhs) {
        for (int32_t i = 0; i < 9; ++i) {
            if (lhs.elements[i] != rhs.elements[i]) {
                return false;
            }
        }
        return true;
    }
} // namespace sm
