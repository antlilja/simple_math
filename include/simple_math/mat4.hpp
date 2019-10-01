// Copyright(c) 2019-present, Anton Lilja.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once
#include "common.hpp"
#include "vec.hpp"
#include "utility.hpp"
#include "quaternion.hpp"

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
        static constexpr mat4 rotation(const Angle angle,
                                       const sm::vec3& axis) {
            static_assert(std::is_same<Angle, radians>::value ||
                              std::is_same<Angle, degrees>::value,
                          "mat4 rotation requires parameter angle in an angle "
                          "type, i.e sm::radians or sm::degrees");

            const auto rad = std::is_same<Angle, radians>::value
                                 ? angle
                                 : radians::from_degrees(angle);

            const float s = taylor_sine(rad);
            const float c = taylor_cosine(rad);
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

        static constexpr mat4 rotation(quaternion q) {
            auto result = mat4::identity();

            const auto sqrw = square(q.w);
            const auto sqrx = square(q.x);
            const auto sqry = square(q.y);
            const auto sqrz = square(q.z);

            const auto invs = 1 / (sqrw + sqrx + sqry + sqrz);
            result.elements[0 + 0 * 4] = (sqrx - sqry - sqrz + sqrw) * invs;
            result.elements[1 + 1 * 4] = (-sqrx + sqry - sqrz + sqrw) * invs;
            result.elements[2 + 2 * 4] = (-sqrx - sqry + sqrz + sqrw) * invs;

            {
                const auto qxy = q.x * q.y;
                const auto qzw = q.z * q.w;
                result.elements[1 + 0 * 4] = 2.0F * (qxy + qzw) * invs;
                result.elements[0 + 1 * 4] = 2.0F * (qxy - qzw) * invs;
            }

            {
                const auto qxz = q.x * q.z;
                const auto qyw = q.y * q.w;
                result.elements[2 + 0 * 4] = 2.0F * (qxz - qyw) * invs;
                result.elements[0 + 2 * 4] = 2.0F * (qxz + qyw) * invs;
            }

            {
                const auto qyz = q.y * q.z;
                const auto qxw = q.x * q.w;
                result.elements[2 + 1 * 4] = 2.0F * (qyz + qxw) * invs;
                result.elements[1 + 2 * 4] = 2.0F * (qyz - qxw) * invs;
            }

            return result;
        }

        template <class Angle>
        static constexpr mat4 rotation_x(const Angle angle) {
            static_assert(std::is_same<Angle, radians>::value ||
                              std::is_same<Angle, degrees>::value,
                          "mat4 rotation requires parameter angle in an angle "
                          "type, i.e sm::radians or sm::degrees");

            const auto rad = std::is_same<Angle, radians>::value
                                 ? angle
                                 : radians::from_degrees(angle);

            auto result = mat4::identity();

            result.elements[1 + 1 * 4] = taylor_cosine(-rad);
            result.elements[2 + 1 * 4] = taylor_sine(-rad);

            result.elements[1 + 2 * 4] = -taylor_sine(-rad);
            result.elements[2 + 2 * 4] = taylor_cosine(-rad);

            return result;
        }

        template <class Angle>
        static constexpr mat4 rotation_y(const Angle angle) {
            static_assert(std::is_same<Angle, radians>::value ||
                              std::is_same<Angle, degrees>::value,
                          "mat4 rotation requires parameter angle in an angle "
                          "type, i.e sm::radians or sm::degrees");

            const auto rad = std::is_same<Angle, radians>::value
                                 ? angle
                                 : radians::from_degrees(angle);

            auto result = mat4::identity();

            result.elements[0 + 0 * 4] = taylor_cosine(-rad);
            result.elements[2 + 0 * 4] = -taylor_sine(-rad);

            result.elements[0 + 2 * 4] = taylor_sine(-rad);
            result.elements[2 + 2 * 4] = taylor_cosine(-rad);

            return result;
        }

        template <class Angle>
        static constexpr mat4 rotation_z(const Angle angle) {
            static_assert(std::is_same<Angle, radians>::value ||
                              std::is_same<Angle, degrees>::value,
                          "mat4 rotation requires parameter angle in an angle "
                          "type, i.e sm::radians or sm::degrees");

            const auto rad = std::is_same<Angle, radians>::value
                                 ? angle
                                 : radians::from_degrees(angle);

            auto result = mat4::identity();

            result.elements[0 + 0 * 4] = taylor_cosine(-rad);
            result.elements[1 + 0 * 4] = taylor_sine(-rad);

            result.elements[0 + 1 * 4] = -taylor_sine(-rad);
            result.elements[1 + 1 * 4] = taylor_cosine(-rad);

            return result;
        }

        template <simd_t simd_level = detail::default_simd>
        quaternion to_quaternion() const {
            const auto qx =
                elements[0 + 0 * 4] + elements[1 + 1 * 4] + elements[2 + 2 * 4];

            const auto qy =
                elements[0 + 0 * 4] - elements[1 + 1 * 4] - elements[2 + 2 * 4];

            const auto qz = -elements[0 + 0 * 4] + elements[1 + 1 * 4] -
                            elements[2 + 2 * 4];

            const auto qw = -elements[0 + 0 * 4] - elements[1 + 1 * 4] +
                            elements[2 + 2 * 4];

            quaternion q(qx, qy, qz, qw);

            if constexpr (simd_level != simd_t::NONE) {
                q.xmm = _mm_add_ps(q.xmm, _mm_set1_ps(1.0F));
                q.xmm = _mm_div_ps(q.xmm, _mm_set1_ps(4.0F));

                if (const auto res =
                        _mm_movemask_ps(_mm_cmplt_ps(q.xmm, _mm_setzero_ps()));
                    (res & 0x7) == 0x7) {
                    if (q.x < 0.0F) q.x = 0.0F;
                    if (q.y < 0.0F) q.y = 0.0F;
                    if (q.z < 0.0F) q.z = 0.0F;
                    if (q.w < 0.0F) q.w = 0.0F;
                }

                q.xmm = _mm_sqrt_ps(q.xmm);
            }
            else {
                q.x += 1.0F;
                q.y += 1.0F;
                q.z += 1.0F;
                q.w += 1.0F;

                q.x /= 4.0F;
                q.y /= 4.0F;
                q.z /= 4.0F;
                q.w /= 4.0F;

                if (q.x < 0.0F) q.x = 0.0F;
                if (q.y < 0.0F) q.y = 0.0F;
                if (q.z < 0.0F) q.z = 0.0F;
                if (q.w < 0.0F) q.w = 0.0F;

                q.x = sqrtf(q.x);
                q.y = sqrtf(q.y);
                q.z = sqrtf(q.z);
                q.w = sqrtf(q.w);
            }

            const auto l_sign = [](const float x) {
                return (x >= 0.0F) ? 1.0F : -1.0F;
            };

            if (q.x >= q.y && q.x >= q.z && q.x >= q.w) {
                if constexpr (simd_level != simd_t::NONE) {
                    __m128 tmp = {
                        1.0F, l_sign(elements[2 + 1 * 4] - elements[1 + 2 * 4]),
                        l_sign(elements[0 + 2 * 4] - elements[2 + 0 * 4]),
                        l_sign(elements[1 + 0 * 4] - elements[0 + 1 * 4])};

                    q.xmm = _mm_mul_ps(q.xmm, tmp);
                }
                else {
                    // q.x *= 1.0F;
                    q.y *= l_sign(elements[2 + 1 * 4] - elements[1 + 2 * 4]);
                    q.z *= l_sign(elements[0 + 2 * 4] - elements[2 + 0 * 4]);
                    q.w *= l_sign(elements[1 + 0 * 4] - elements[0 + 1 * 4]);
                }
            }
            else if (q.y >= q.x && q.y >= q.z && q.y >= q.w) {
                if constexpr (simd_level != simd_t::NONE) {
                    q.xmm = _mm_mul_ps(
                        q.xmm,
                        _mm_set_ps(
                            l_sign(elements[2 + 1 * 4] - elements[1 + 2 * 4]),
                            1.0F,
                            l_sign(elements[1 + 0 * 4] + elements[0 + 1 * 4]),
                            l_sign(elements[0 + 2 * 4] + elements[2 + 0 * 4])));
                }
                else {
                    q.x *= l_sign(elements[2 + 1 * 4] - elements[1 + 2 * 4]);
                    // q.y *= 1.0F;
                    q.z *= l_sign(elements[1 + 0 * 4] + elements[0 + 1 * 4]);
                    q.w *= l_sign(elements[0 + 2 * 4] + elements[2 + 0 * 4]);
                }
            }
            else if (q.z >= q.x && q.z >= q.y && q.z >= q.w) {
                if constexpr (simd_level != simd_t::NONE) {
                    q.xmm = _mm_mul_ps(
                        q.xmm,
                        _mm_set_ps(
                            l_sign(elements[0 + 2 * 4] - elements[2 + 0 * 4]),
                            l_sign(elements[1 + 0 * 4] + elements[0 + 1 * 4]),
                            1.0F,
                            l_sign(elements[2 + 1 * 4] + elements[1 + 2 * 4])));
                }
                else {
                    q.x *= l_sign(elements[0 + 2 * 4] - elements[2 + 0 * 4]);
                    q.y *= l_sign(elements[1 + 0 * 4] + elements[0 + 1 * 4]);
                    // q.z *= 1.0F
                    q.w *= l_sign(elements[2 + 1 * 4] + elements[1 + 2 * 4]);
                }
            }
            else if (q.w >= q.x && q.w >= q.y && q.w >= q.z) {
                if constexpr (simd_level != simd_t::NONE) {
                    q.xmm = _mm_mul_ps(
                        q.xmm,
                        _mm_set_ps(
                            l_sign(elements[1 + 0 * 4] - elements[0 + 1 * 4]),
                            l_sign(elements[2 + 0 * 4] + elements[0 + 2 * 4]),
                            l_sign(elements[2 + 1 * 4] + elements[1 + 2 * 4]),
                            1.0F));
                }
                else {
                    q.x *= l_sign(elements[1 + 0 * 4] - elements[0 + 1 * 4]);
                    q.y *= l_sign(elements[2 + 0 * 4] + elements[0 + 2 * 4]);
                    q.z *= l_sign(elements[2 + 1 * 4] + elements[1 + 2 * 4]);
                    // q.w *= 1.0F;
                }
            }

            if constexpr (simd_level >= simd_t::SSE4) {
                q.xmm = _mm_div_ps(q.xmm,
                                   _mm_sqrt_ps(_mm_dp_ps(q.xmm, q.xmm, 0xff)));
            }
            else if constexpr (simd_level != simd_t::NONE) {
                const auto x0 = _mm_mul_ps(q.xmm, q.xmm);
                const auto x1 = _mm_movehl_ps(x0, x0);
                const auto x2 = _mm_add_ps(x0, x1);
                const auto x3 = _mm_shuffle_ps(x2, x2, _MM_SHUFFLE(0, 0, 0, 1));
                const auto x4 = _mm_add_ps(x2, x3);
                const auto dp = _mm_sqrt_ps(x4);
                q.xmm = _mm_div_ps(q.xmm, dp);
            }
            else {
                const auto dp = sqrtf(square(q.x) + square(q.y) + square(q.z) +
                                      square(q.w));
                q.x /= dp;
                q.y /= dp;
                q.z /= dp;
                q.w /= dp;
            }

            return q;
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
    template <simd_t simd = detail::default_simd>
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
    template <simd_t simd = detail::default_simd>
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

    template <simd_t simd_level = detail::default_simd>
    inline quaternion to_quaternion(const mat4& mat) {
        return mat.to_quaternion();
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

    template <simd_t simd = detail::default_simd>
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
