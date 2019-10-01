// Copyright(c) 2019-present, Anton Lilja.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once
#include "common.hpp"
#include "vec.hpp"

#include <type_traits>

namespace sm {
    namespace detail {
        constexpr __m128 Q_VP_SIGNMASK = {-0.0F, -0.0F, -0.0F, 0.0F};
    }

    struct quaternion {
        // Data
        union {
            float elements[4];

            struct {
                float x, y, z, w;
            };

            __m128 xmm;
        };

        // Constructors
        inline constexpr quaternion() : x(0.0F), y(0.0F), z(0.0F), w(0.0F) {}

        inline constexpr quaternion(float x, float y, float z, float w)
            : x(x), y(y), z(z), w(w) {}

        inline constexpr quaternion(__m128 xmm) : xmm(std::move(xmm)) {}

        inline static constexpr quaternion identity() {
            return quaternion(0.0F, 0.0F, 0.0F, 1.0F);
        }

        template <typename Angle>
        inline static quaternion from_axis(const Angle angle, float x, float y,
                                           float z) {
            static_assert(std::is_same<Angle, radians>::value ||
                              std::is_same<Angle, degrees>::value,
                          "quaternion from_axis requires an angle "
                          "type, i.e radians or degrees");

            const auto rad = std::is_same<Angle, radians>::value
                                 ? angle
                                 : radians::from_degrees(angle);

            const auto srad = sinf(rad / 2);
            return {x * srad, y * srad, z * srad, cosf(rad / 2)};
        }

        template <typename Angle>
        inline static constexpr quaternion from_axis(const Angle angle,
                                                     const vec3& axis) {
            return from_axis(angle, axis.x, axis.y, axis.z);
        }

        template <typename Angle = radians>
        std::pair<Angle, vec3> to_axis() const {
            static_assert(std::is_same<Angle, radians>::value ||
                              std::is_same<Angle, degrees>::value,
                          "quaternion to_axis requires an angle "
                          "type, i.e radians or degrees");

            const auto half_angle = acosf(w);
            const auto s = sqrtf(1.0F - (w * w));

            // TODO: Research check for close to zero
            // if (s < 0.001F) {}

            return std::pair<Angle, vec3>(
                std::is_same<Angle, radians>::value
                    ? radians(half_angle * 2.0F)
                    : degrees::from_radians(half_angle * 2.0F),
                vec3(x / s, y / s, z / s));
        }

        template <simd_t simd_level = detail::default_simd>
        inline quaternion conjugate() const {
            return simd_level != simd_t::NONE
                       ? quaternion(_mm_xor_ps(xmm, detail::Q_VP_SIGNMASK))
                       : quaternion(-x, -y, -z, w);
        }
    };

    inline constexpr quaternion multiply(const quaternion& lhs,
                                         const quaternion& rhs) {
        auto result = quaternion::identity();

        const auto vlhs = vec3(lhs.x, lhs.y, lhs.z);
        const auto vrhs = vec3(rhs.x, rhs.y, rhs.z);

        result.w = lhs.w * rhs.w - dot(vlhs, vrhs);

        const auto v = cross(vlhs, vrhs) + vrhs * lhs.w + vlhs * rhs.w;
        result.x = v.x;
        result.y = v.y;
        result.z = v.z;

        return result;
    }

    inline constexpr quaternion operator*(const quaternion& lhs,
                                          const quaternion& rhs) {
        return multiply(lhs, rhs);
    }

    inline constexpr quaternion& operator*=(quaternion& lhs,
                                            const quaternion& rhs) {
        return lhs = multiply(lhs, rhs);
    }

    inline quaternion conjugate(const quaternion& q) { return q.conjugate(); }

    template <typename Angle = radians>
    inline std::pair<Angle, vec3> to_axis(const quaternion& q) {
        return q.to_axis<Angle>();
    }

    inline vec3 transform(const vec3& v, const quaternion& q) {
        quaternion qv(v.x, v.y, v.z, 0.0F);
        const auto qr = q * qv * q.conjugate();
        return vec3(qr.x, qr.y, qr.z);
    }
} // namespace sm