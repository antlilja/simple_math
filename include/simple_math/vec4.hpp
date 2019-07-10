// Copyright(c) 2019-present, Anton Lilja.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once
#include "common.hpp"
#include "utility.hpp"
#include "vec3.hpp"

namespace sm {
    struct SIMPLE_MATH_ALIGN(16) vec4 {
        // Data
        union {
            float elements[4];

            // Transform
            struct {
                float x, y, z, w;
            };

            // Color
            struct {
                float r, g, b, a;
            };

            // SIMD
            __m128 xmm;
        };

        // Constructors
        inline constexpr vec4() : elements{0.0F, 0.0F, 0.0F, 0.0F} {}

        inline constexpr vec4(float v) : elements{v, v, v, v} {}

        inline constexpr vec4(const vec3& v, float val = 0.0F)
            : elements{v.x, v.y, v.z, val} {}

        inline constexpr vec4(float _x, float _y, float _z, float _w)
            : elements{_x, _y, _z, _w} {}

        inline constexpr vec4(__m128 _xmm) : xmm(std::move(_xmm)) {}

        // Operations
        template <simd_t simd = intern::default_simd>
        inline vec4 inverse() const {
            return simd != simd_t::NONE ? vec4(_mm_xor_ps(xmm, M128_SIGNMASK))
                                        : vec4(-x, -y, -z, -w);
        }

        inline vec4 operator-() const { return inverse(); }

        template <simd_t simd = intern::default_simd>
        inline float magnitude() const {
            if constexpr (simd >= simd_t::SSE4) {
                return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(xmm, xmm, 0xff)));
            }
            else if constexpr (simd != simd_t::NONE) {
                // x0 = x^2, y^2, z^2, w^2
                // x1 = x^2, y^2, x^2, y^2
                // x2 = 2(x^2), 2(y^2), x^2 + z^2, y^2 + w^2
                // x3 = x^2 + z^2, y^2 + w^2, y^2 + w^2, y^2 + w^2
                // x4 = x^2 + y^2 + z^2 + w^2, ??
                // x5 = sqrt(x^2 + y^2 + z^2 + w^2)
                // ret sqrt(x^2 + y^2 + z^2 + w^2)

                const auto x0 = _mm_mul_ps(xmm, xmm);
                const auto x1 = _mm_movehl_ps(x0, x0);
                const auto x2 = _mm_add_ps(x0, x1);
                const auto x3 = _mm_shuffle_ps(x2, x2, _MM_SHUFFLE(0, 0, 0, 1));
                const auto x4 = _mm_add_ss(x2, x3);
                const auto x5 = _mm_sqrt_ss(x4);
                return _mm_cvtss_f32(x5);
            }
            else {
                return sqrtf(x * x + y * y + z * z + w * w);
            }
        }

        template <simd_t simd = intern::default_simd>
        inline vec4 normalize() const {
            if constexpr (simd >= simd_t::SSE4) {
                const auto x = _mm_sqrt_ps(_mm_dp_ps(xmm, xmm, 0xff));

                SIMPLE_MATH_ASSERT(_mm_cvtss_f32(x) != 0.0F);
                return _mm_div_ps(xmm, x);
            }
            else if constexpr (simd != simd_t::NONE) {
                // x0 = x^2, y^2, z^2, w^2
                // x1 = x^2, y^2, x^2, y^2
                // x2 = 2(x^2), 2(y^2), x^2 + z^2, y^2 + w^2
                // x3 = x^2 + z^2, y^2 + w^2, x^2 + z^2, y^2 + w^2
                // x4 = y^2 + w^2, x^2 + z^2, y^2 + w^2, x^2 + z^2
                // x5 = x^2 + y^2 + z^2 + w^2, -||-
                // x6 = sqrt(x^2 + y^2 + z^2 + w^2), -||-
                // ret x / x6, y / x6, z / x6, w / x6

                const auto x0 = _mm_mul_ps(xmm, xmm);
                const auto x1 = _mm_movehl_ps(x0, x0);

                const auto x2 = _mm_add_ps(x0, x1);

                const auto x3 = _mm_shuffle_ps(x2, x2, _MM_SHUFFLE(0, 1, 0, 1));
                const auto x4 = _mm_shuffle_ps(x2, x2, _MM_SHUFFLE(1, 0, 1, 0));
                const auto x5 = _mm_add_ps(x3, x4);
                const auto x6 = _mm_sqrt_ps(x5);

                SIMPLE_MATH_ASSERT(_mm_cvtss_f32(x6) != 0.0F);
                return _mm_div_ps(xmm, x6);
            }
            else {
                const auto mag = magnitude<simd>();

                SIMPLE_MATH_ASSERT(mag != 0.0F);
                return {x / mag, y / mag, z / mag, z / mag};
            }
        }

        template <simd_t simd = intern::default_simd>
        inline vec4 normalize_fast() const {
            if constexpr (simd >= simd_t::SSE4) {
                const auto x = _mm_rsqrt_ps(_mm_dp_ps(xmm, xmm, 0xff));

                SIMPLE_MATH_ASSERT(_mm_cvtss_f32(x) != 0.0F);
                return _mm_mul_ps(xmm, x);
            }
            else if constexpr (simd != simd_t::NONE) {
                // x0 = x^2, y^2, z^2, w^2
                // x1 = x^2, y^2, x^2, y^2
                // x2 = 2(x^2), 2(y^2), x^2 + z^2, y^2 + w^2
                // x3 = x^2 + z^2, y^2 + w^2, x^2 + z^2, y^2 + w^2
                // x4 = y^2 + w^2, x^2 + z^2, y^2 + w^2, x^2 + z^2
                // x5 = x^2 + y^2 + z^2 + w^2, -||-
                // x6 = rsqrt(x^2 + y^2 + z^2 + w^2), -||-
                // ret x * x6, y * x6, z * x6, w * x6

                const auto x0 = _mm_mul_ps(xmm, xmm);
                const auto x1 = _mm_movehl_ps(x0, x0);

                const auto x2 = _mm_add_ps(x0, x1);

                const auto x3 = _mm_shuffle_ps(x2, x2, _MM_SHUFFLE(0, 1, 0, 1));
                const auto x4 = _mm_shuffle_ps(x2, x2, _MM_SHUFFLE(1, 0, 1, 0));
                const auto x5 = _mm_add_ps(x3, x4);

                SIMPLE_MATH_ASSERT(_mm_cvtss_f32(x5) != 0.0F);
                const auto x6 = _mm_rsqrt_ps(x5);
                return _mm_mul_ps(xmm, x6);
            }
            else {
                const auto sqr_mag = x * x + y * y + z * z + w * w;

                SIMPLE_MATH_ASSERT(sqr_mag != 0.0F);
                const auto is = fast_inverse_sqrt(sqr_mag);
                return {x * is, y * is, z * is, w * is};
            }
        }
    };

    // Operations //

    // Add
    template <simd_t simd = intern::default_simd>
    inline vec4 add(const vec4& lhs, const vec4& rhs) {
        return simd != simd_t::NONE ? vec4(_mm_add_ps(lhs.xmm, rhs.xmm))
                                    : vec4(lhs.x + rhs.x, lhs.y + rhs.y,
                                           lhs.z + rhs.z, lhs.w + rhs.w);
    }

    inline vec4 operator+(const vec4& lhs, const vec4& rhs) {
        return add(lhs, rhs);
    }

    inline vec4& operator+=(vec4& lhs, const vec4& rhs) {
        return lhs = add(lhs, rhs);
    }

    // Subtract
    template <simd_t simd = intern::default_simd>
    inline vec4 subtract(const vec4& lhs, const vec4& rhs) {
        return simd != simd_t::NONE ? vec4(_mm_sub_ps(lhs.xmm, rhs.xmm))
                                    : vec4(lhs.x - rhs.x, lhs.y - rhs.y,
                                           lhs.z - rhs.z, lhs.w - rhs.w);
    }

    inline vec4 operator-(const vec4& lhs, const vec4& rhs) {
        return subtract(lhs, rhs);
    }

    inline vec4& operator-=(vec4& lhs, const vec4& rhs) {
        return lhs = subtract(lhs, rhs);
    }

    // Multiply
    template <simd_t simd = intern::default_simd>
    inline vec4 multiply(const vec4& v, const float scalar) {
        return simd != simd_t::NONE
                   ? vec4(_mm_mul_ps(v.xmm, _mm_set1_ps(scalar)))
                   : vec4(v.x * scalar, v.y * scalar, v.z * scalar,
                          v.w * scalar);
    }

    inline vec4 operator*(const vec4& v, const float scalar) {
        return multiply(v, scalar);
    }

    inline vec4& operator*=(vec4& v, const float scalar) {
        return v = multiply(v, scalar);
    }

    // Divide
    template <simd_t simd = intern::default_simd>
    inline vec4 divide(const vec4& v, const float scalar) {
        SIMPLE_MATH_ASSERT(scalar != 0.0F);
        return simd != simd_t::NONE
                   ? vec4(_mm_div_ps(v.xmm, _mm_set1_ps(scalar)))
                   : vec4(v.x / scalar, v.y / scalar, v.z / scalar,
                          v.w / scalar);
    }

    inline vec4 operator/(const vec4& v, const float scalar) {
        return divide(v, scalar);
    }

    inline vec4& operator/=(vec4& v, const float scalar) {
        return v = divide(v, scalar);
    }

    // Element-wise product
    template <simd_t simd = intern::default_simd>
    inline vec4 element_wise(const vec4& lhs, const vec4& rhs) {
        return simd != simd_t::NONE ? vec4(_mm_mul_ps(lhs.xmm, rhs.xmm))
                                    : vec4(lhs.x * rhs.x, lhs.y * rhs.y,
                                           lhs.z * rhs.z, lhs.w * rhs.w);
    }

    // Dot product
    template <simd_t simd = intern::default_simd>
    inline float dot(const vec4& lhs, const vec4& rhs) {
        if constexpr (simd != simd_t::NONE) {
            // x0 = l.x * r.x, l.y * r.y, l.z * r.z, l.w * r.w
            // x1 = l.x * r.x, l.y * r.y, l.x * r.x, l.y * r.y
            // x2 = 2(l.x * r.x), 2(l.y * r.y), l.x * r.x + l.z * r.z, l.y * r.y
            // + l.w * r.w x3 = x^2 + z^2, y^2 + w^2, y^2 + w^2, y^2 + w^2 x4 =
            // x^2 + y^2 + z^2 + w^2, ?? x5 = sqrt(x^2 + y^2 + z^2 + w^2) ret
            // sqrt(x^2 + y^2 + z^2 + w^2)

            const auto x0 = _mm_mul_ps(lhs.xmm, rhs.xmm);
            const auto x1 = _mm_movehl_ps(x0, x0);

            const auto x2 = _mm_add_ps(x1, x0);
            const auto x3 = _mm_shuffle_ps(x2, x2, _MM_SHUFFLE(0, 0, 0, 1));
            return _mm_cvtss_f32(_mm_add_ss(x3, x2));
        }
        else {
            return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z +
                   lhs.w * rhs.w;
        }
    }

    // Compare
    template <simd_t simd = intern::default_simd>
    inline bool compare(const vec4& lhs, const vec4& rhs) {
        if constexpr (simd != simd_t::NONE) {
            const auto eq = _mm_cmpeq_ps(lhs.xmm, rhs.xmm);
            return ((_mm_movemask_ps(eq) & 0x7) == 0x7);
        }
        else {
            return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z &&
                   lhs.w == rhs.w;
        }
    }

    inline bool operator==(const vec4& lhs, const vec4& rhs) {
        return compare(lhs, rhs);
    }

    inline bool operator!=(const vec4& lhs, const vec4& rhs) {
        return !compare(lhs, rhs);
    }
} // namespace sm