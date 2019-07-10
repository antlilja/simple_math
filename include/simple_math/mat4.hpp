// Copyright(c) 2019-present, Anton Lilja.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once
#include "common.hpp"
#include "vec.hpp"

#include <type_traits>

namespace sm {
    struct SIMPLE_MATH_ALIGN(32) mat4 {
        // Data
        union {
            float elements[16];
            vec4 columns[4];

            // SIMD
            __m128 xmm[4];
            __m256 ymm[2];
        };

        // Constructors
        inline constexpr mat4()
            : elements{0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
                       0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F} {}

        inline constexpr mat4(const float diagonal) : mat4() {
            elements[0 + 0 * 4] = diagonal;
            elements[1 + 1 * 4] = diagonal;
            elements[2 + 2 * 4] = diagonal;
            elements[3 + 3 * 4] = diagonal;
        }

        inline mat4(__m256 ymm0, __m256 ymm1)
            : ymm{std::move(ymm0), std::move(ymm1)} {}

        inline static constexpr mat4 identity() { return mat4(1.0F); }

        // Projection matrices
        static constexpr mat4 ortho(float left, float right, float bottom,
                                    float top, float _near,
                                    float _far /* Whereeeever you are */) {
            mat4 result;
            result.elements[0 + 0 * 4] = 2.0F / (right - left);
            result.elements[1 + 1 * 4] = 2.0F / (top - bottom);
            result.elements[2 + 2 * 4] = -2.0F / (_far - _near);
            result.elements[3 + 3 * 4] = 1.0F;

            result.elements[0 + 3 * 4] = (right + left) / (right - left);
            result.elements[1 + 3 * 4] = (top + bottom) / (top - bottom);
            result.elements[2 + 3 * 4] = (_near + _far) / (_far - _near);
            return result;
        }

        template <class Angle>
        static mat4 perspective(const Angle fov, float aspect_ratio,
                                float _near, float _far) {
            static_assert(std::is_same<Angle, radians>::value ||
                              std::is_same<Angle, degrees>::value,
                          "mat4 rotation requires parameter angle in an angle "
                          "type, i.e sm::radians or sm::degrees");

            const auto rad_fov = std::is_same<Angle, radians>::value
                                     ? fov
                                     : radians::from_degrees(fov);

            const float top = tanf(rad_fov / 2.0F) * _near;

            mat4 result;
            result.elements[0 + 0 * 4] = _near / (aspect_ratio * top);
            result.elements[1 + 1 * 4] = -(_near / top);
            result.elements[2 + 2 * 4] = -(_far + _near) / (_far - _near);
            result.elements[3 + 2 * 4] = -1.0F;
            result.elements[2 + 3 * 4] =
                -(2.0F * _far * _near) / (_far - _near);
            return result;
        }

        // Transform matrices //

        // Translation
        inline static constexpr mat4 translation(float x, float y, float z) {
            auto result = mat4::identity();
            result.elements[0 + 3 * 4] = x;
            result.elements[1 + 3 * 4] = y;
            result.elements[2 + 3 * 4] = z;

            return result;
        }

        inline static constexpr mat4 translation(const sm::vec3& t) {
            return translation(t.x, t.y, t.z);
        }

        // Rotation
        template <class Angle>
        static mat4 rotation(const Angle angle, const sm::vec3& axis) {
            static_assert(std::is_same<Angle, radians>::value ||
                              std::is_same<Angle, degrees>::value,
                          "mat4 rotation requires parameter angle in an angle "
                          "type, i.e sm::radians or sm::degrees");

            const auto rad = std::is_same<Angle, radians>::value
                                 ? angle
                                 : radians::from_degrees(angle);

            const float s = sinf(rad);
            const float c = cosf(rad);
            const float omc = 1.0F - c;

            auto result = mat4::identity();

            result.elements[0 + 0 * 4] = axis.x * omc + c;
            result.elements[1 + 0 * 4] = axis.y * axis.x * omc + axis.z * s;
            result.elements[2 + 0 * 4] = axis.x * axis.z * omc - axis.y * s;

            result.elements[0 + 1 * 4] = axis.x * axis.y * omc - axis.z * s;
            result.elements[1 + 1 * 4] = axis.y * omc + c;
            result.elements[2 + 1 * 4] = axis.y * axis.z * omc + axis.x * s;

            result.elements[0 + 2 * 4] = axis.x * axis.z * omc + axis.y * s;
            result.elements[1 + 2 * 4] = axis.y * axis.z * omc - axis.x * s;
            result.elements[2 + 2 * 4] = axis.z * omc + c;

            return result;
        }

        template <class Angle>
        static mat4 rotation_x(const Angle angle) {
            static_assert(std::is_same<Angle, radians>::value ||
                              std::is_same<Angle, degrees>::value,
                          "mat4 rotation requires parameter angle in an angle "
                          "type, i.e sm::radians or sm::degrees");

            const auto rad = std::is_same<Angle, radians>::value
                                 ? angle
                                 : radians::from_degrees(angle);

            auto result = mat4::identity();

            result.elements[1 + 1 * 4] = cosf(-rad);
            result.elements[2 + 1 * 4] = sinf(-rad);

            result.elements[1 + 2 * 4] = -sinf(-rad);
            result.elements[2 + 2 * 4] = cosf(-rad);

            return result;
        }

        template <class Angle>
        static mat4 rotation_y(const Angle angle) {
            static_assert(std::is_same<Angle, radians>::value ||
                              std::is_same<Angle, degrees>::value,
                          "mat4 rotation requires parameter angle in an angle "
                          "type, i.e sm::radians or sm::degrees");

            const auto rad = std::is_same<Angle, radians>::value
                                 ? angle
                                 : radians::from_degrees(angle);

            auto result = mat4::identity();

            result.elements[0 + 0 * 4] = cosf(-rad);
            result.elements[2 + 0 * 4] = -sinf(-rad);

            result.elements[0 + 2 * 4] = sinf(-rad);
            result.elements[2 + 2 * 4] = cosf(-rad);

            return result;
        }

        template <class Angle>
        static mat4 rotation_z(const Angle angle) {
            static_assert(std::is_same<Angle, radians>::value ||
                              std::is_same<Angle, degrees>::value,
                          "mat4 rotation requires parameter angle in an angle "
                          "type, i.e sm::radians or sm::degrees");

            const auto rad = std::is_same<Angle, radians>::value
                                 ? angle
                                 : radians::from_degrees(angle);

            auto result = mat4::identity();

            result.elements[0 + 0 * 4] = cosf(-rad);
            result.elements[1 + 0 * 4] = sinf(-rad);

            result.elements[0 + 1 * 4] = -sinf(-rad);
            result.elements[1 + 1 * 4] = cosf(-rad);

            return result;
        }

        // Scale
        static constexpr mat4 scale(float x, float y, float z) {
            auto result = mat4::identity();
            result.elements[0 + 0 * 4] = x;
            result.elements[1 + 1 * 4] = y;
            result.elements[2 + 2 * 4] = z;

            return result;
        }

        inline static constexpr mat4 scale(const sm::vec3& s) {
            return scale(s.x, s.y, s.z);
        }

        // Look at (camera)
        static mat4 look_at(const vec3& eye, const vec3& target,
                            const vec3& up_dir) {
            const auto forward = normalize(target - eye);
            const auto side = normalize(cross(forward, up_dir));
            const auto up = cross(side, forward);

            auto result = mat4::identity();

            // Rotation
            result.elements[0 + 0 * 4] = side.x;
            result.elements[1 + 0 * 4] = side.y;
            result.elements[2 + 0 * 4] = side.z;
            result.elements[0 + 1 * 4] = up.x;
            result.elements[1 + 1 * 4] = up.y;
            result.elements[2 + 1 * 4] = up.z;
            result.elements[0 + 2 * 4] = -forward.x;
            result.elements[1 + 2 * 4] = -forward.y;
            result.elements[2 + 2 * 4] = -forward.z;

            // Translation
            result.elements[3 + 0 * 4] = -dot(side, eye);
            result.elements[3 + 1 * 4] = -dot(up, eye);
            result.elements[3 + 2 * 4] = dot(forward, eye);
            return result;
        }
    };

    // Multiply
    template <simd_t simd = intern::default_simd>
    inline mat4 multiply(const mat4& lhs, const mat4& rhs) {
        if constexpr (simd == simd_t::AVX) {
            __m256 a0, a1, b0, b1;
            __m256 c0, c1, c2, c3, c4, c5;

            const auto t0 = rhs.ymm[0];
            const auto t1 = rhs.ymm[1];
            const auto u0 = lhs.ymm[0];
            const auto u1 = lhs.ymm[1];

            a0 = _mm256_shuffle_ps(t0, t0, _MM_SHUFFLE(0, 0, 0, 0));
            a1 = _mm256_shuffle_ps(t1, t1, _MM_SHUFFLE(0, 0, 0, 0));
            b0 = _mm256_permute2f128_ps(u0, u0, 0x00);
            c0 = _mm256_mul_ps(a0, b0);
            c1 = _mm256_mul_ps(a1, b0);

            a0 = _mm256_shuffle_ps(t0, t0, _MM_SHUFFLE(1, 1, 1, 1));
            a1 = _mm256_shuffle_ps(t1, t1, _MM_SHUFFLE(1, 1, 1, 1));
            b0 = _mm256_permute2f128_ps(u0, u0, 0x11);
            c2 = _mm256_mul_ps(a0, b0);
            c3 = _mm256_mul_ps(a1, b0);

            a0 = _mm256_shuffle_ps(t0, t0, _MM_SHUFFLE(2, 2, 2, 2));
            a1 = _mm256_shuffle_ps(t1, t1, _MM_SHUFFLE(2, 2, 2, 2));
            b1 = _mm256_permute2f128_ps(u1, u1, 0x00);
            c4 = _mm256_mul_ps(a0, b1);
            c5 = _mm256_mul_ps(a1, b1);

            a0 = _mm256_shuffle_ps(t0, t0, _MM_SHUFFLE(3, 3, 3, 3));
            a1 = _mm256_shuffle_ps(t1, t1, _MM_SHUFFLE(3, 3, 3, 3));
            b1 = _mm256_permute2f128_ps(u1, u1, 0x11);
            const auto c6 = _mm256_mul_ps(a0, b1);
            const auto c7 = _mm256_mul_ps(a1, b1);

            c0 = _mm256_add_ps(c0, c2);
            c4 = _mm256_add_ps(c4, c6);
            c1 = _mm256_add_ps(c1, c3);
            c5 = _mm256_add_ps(c5, c7);

            return {_mm256_add_ps(c0, c4), _mm256_add_ps(c1, c5)};
        }
        else if constexpr (simd != simd_t::NONE) {
            mat4 result;
            const auto row1 = _mm_load_ps(&lhs.elements[0]);
            const auto row2 = _mm_load_ps(&lhs.elements[4]);
            const auto row3 = _mm_load_ps(&lhs.elements[8]);
            const auto row4 = _mm_load_ps(&lhs.elements[12]);
            for (int32_t i = 0; i < 4; ++i) {
                const auto brod1 = _mm_set1_ps(rhs.elements[4 * i + 0]);
                const auto brod2 = _mm_set1_ps(rhs.elements[4 * i + 1]);
                const auto brod3 = _mm_set1_ps(rhs.elements[4 * i + 2]);
                const auto brod4 = _mm_set1_ps(rhs.elements[4 * i + 3]);
                const auto row =
                    _mm_add_ps(_mm_add_ps(_mm_mul_ps(brod1, row1),
                                          _mm_mul_ps(brod2, row2)),
                               _mm_add_ps(_mm_mul_ps(brod3, row3),
                                          _mm_mul_ps(brod4, row4)));

                _mm_store_ps(&result.elements[4 * i], row);
            }
            return result;
        }
        else {
            mat4 result;
            for (int32_t y = 0; y < 4; ++y) {
                for (int32_t x = 0; x < 4; ++x) {
                    for (int32_t e = 0; e < 4; ++e) {
                        result.elements[x + y * 4] +=
                            lhs.elements[x + e * 4] * rhs.elements[e + y * 4];
                    }
                }
            }
            return result;
        }
    }

    inline mat4 operator*(const mat4& lhs, const mat4& rhs) {
        return multiply(lhs, rhs);
    }

    inline mat4& operator*=(mat4& lhs, const mat4& rhs) {
        return lhs = multiply(lhs, rhs);
    }

    // Compare
    template <simd_t simd = intern::default_simd>
    inline bool compare(const mat4& lhs, const mat4& rhs) {
        if constexpr (simd != simd_t::NONE) {
            const auto eq0 = _mm_cmpeq_ps(lhs.xmm[0], rhs.xmm[0]);
            const auto eq1 = _mm_cmpeq_ps(lhs.xmm[1], rhs.xmm[1]);
            const auto eq2 = _mm_cmpeq_ps(lhs.xmm[2], rhs.xmm[2]);
            const auto eq3 = _mm_cmpeq_ps(lhs.xmm[3], rhs.xmm[3]);
            return ((_mm_movemask_ps(eq0) & 0x7) == 0x7) &&
                   ((_mm_movemask_ps(eq1) & 0x7) == 0x7) &&
                   ((_mm_movemask_ps(eq2) & 0x7) == 0x7) &&
                   ((_mm_movemask_ps(eq3) & 0x7) == 0x7);
        }
        else {
            for (int32_t i = 0; i < 16; ++i) {
                if (lhs.elements[i] != rhs.elements[i]) {
                    return false;
                }
            }
            return true;
        }
    }

    // Vector matrix multiplication
    inline vec3 transform_vec(const vec3& v, const mat4& m) {
        vec4 result;
        for (int32_t i = 0; i < 4; ++i) {
            result.elements[i] = v.x * m.elements[0 + i * 4] +
                                 v.y * m.elements[1 + i * 4] +
                                 v.z * m.elements[2 + i * 4];
        }
        return {result.elements[0] / result.elements[3],
                result.elements[1] / result.elements[3],
                result.elements[2] / result.elements[3]};
    }

    template <simd_t simd = intern::default_simd>
    inline vec4 transform_vec(const vec4& v, const mat4& m) {
        if constexpr (simd != simd_t::NONE) {
            const auto m0x = _mm_mul_ps(_mm_set1_ps(v.x), m.xmm[0]);
            const auto m1y = _mm_mul_ps(_mm_set1_ps(v.y), m.xmm[1]);
            const auto m2z = _mm_mul_ps(_mm_set1_ps(v.z), m.xmm[2]);
            return {_mm_add_ps(_mm_add_ps(m0x, m1y), m2z)};
        }
        else {
            vec4 result;
            for (int32_t i = 0; i < 4; ++i) {
                result.elements[i] = v.x * m.elements[0 + i * 4] +
                                     v.y * m.elements[1 + i * 4] +
                                     v.z * m.elements[2 + i * 4];
            }
            return {result.elements[0] / result.elements[3],
                    result.elements[1] / result.elements[3],
                    result.elements[2] / result.elements[3], v.w};
        }
    }

    inline vec3 operator*(const vec3& v, const mat4& m) {
        return transform_vec(v, m);
    }
    inline vec4 operator*(const vec4& v, const mat4& m) {
        return transform_vec(v, m);
    }

    inline vec3& operator*=(vec3& v, const mat4& m) {
        return v = transform_vec(v, m);
    }
    inline vec4& operator*=(vec4& v, const mat4& m) {
        return v = transform_vec(v, m);
    }
} // namespace sm
