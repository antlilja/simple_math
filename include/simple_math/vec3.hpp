// Copyright(c) 2019-present, Anton Lilja.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once
#include "common.hpp"
#include "utility.hpp"

namespace sm {
    union SIMPLE_MATH_ALIGN(16) vec3 {
        // Data
        float elements[3];

        struct {
            float x, y, z;
        };

        struct {
            float r, g, b;
        };

        // Constructors
        inline constexpr vec3() : elements{0.0F, 0.0F, 0.0F} {}

        inline constexpr vec3(float v) : elements{v, v, v} {}

        inline constexpr vec3(float _x, float _y, float _z)
            : elements{_x, _y, _z} {}

        // Operations
        inline constexpr vec3 inverse() const { return {-x, -y, -z}; }

        inline constexpr vec3 operator-() const { return inverse(); }

        constexpr float square_magnitude() const {
            return x * x + y * y + z * z;
        }

        inline float magnitude() const { return std::sqrt(square_magnitude()); }

        inline vec3 normalize() const {
            const auto mag = magnitude();

            SIMPLE_MATH_ASSERT(mag != 0.0F);
            return {x / mag, y / mag, z / mag};
        }

        inline vec3 normalize_fast() const {
            const auto sqr_mag = square_magnitude();

            SIMPLE_MATH_ASSERT(sqr_mag != 0.0F);
            const auto inv_sqrt = fast_inverse_sqrt(sqr_mag);
            return {x * inv_sqrt, y * inv_sqrt, z * inv_sqrt};
        }
    };

    // Operations //

    // Add
    inline constexpr vec3 add(const vec3& lhs, const vec3& rhs) {
        return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
    }

    inline constexpr vec3 operator+(const vec3& lhs, const vec3& rhs) {
        return add(lhs, rhs);
    }

    inline constexpr vec3& operator+=(vec3& lhs, const vec3& rhs) {
        return lhs = add(lhs, rhs);
    }

    // Subtract
    inline constexpr vec3 subtract(const vec3& lhs, const vec3& rhs) {
        return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
    }

    inline constexpr vec3 operator-(const vec3& lhs, const vec3& rhs) {
        return subtract(lhs, rhs);
    }

    inline constexpr vec3& operator-=(vec3& lhs, const vec3& rhs) {
        return lhs = subtract(lhs, rhs);
    }

    // Multiply
    inline constexpr vec3 multiply(const vec3& v, const float scalar) {
        return {v.x * scalar, v.y * scalar, v.z * scalar};
    }

    inline constexpr vec3 operator*(const vec3& v, const float scalar) {
        return multiply(v, scalar);
    }

    inline constexpr vec3 operator*(const float scalar, const vec3& v) {
        return multiply(v, scalar);
    }

    inline constexpr vec3& operator*=(vec3& v, const float scalar) {
        return v = multiply(v, scalar);
    }

    // Divide
    inline vec3 divide(const vec3& v, const float scalar) {
        SIMPLE_MATH_ASSERT(scalar != 0.0F);
        return {v.x / scalar, v.y / scalar, v.z / scalar};
    }

    inline vec3 operator/(const vec3& v, const float scalar) {
        return divide(v, scalar);
    }

    inline vec3& operator/=(vec3& v, const float scalar) {
        return v = divide(v, scalar);
    }

    // Element-wise product
    inline constexpr vec3 element_wise(const vec3& lhs, const vec3& rhs) {
        return {lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z};
    }

    // Dot product
    inline constexpr float dot(const vec3& lhs, const vec3& rhs) {
        return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
    }

    // Cross product
    inline constexpr vec3 cross(const vec3& lhs, const vec3& rhs) {
        const auto x = lhs.y * rhs.z - lhs.z * rhs.y;
        const auto y = lhs.z * rhs.x - lhs.x * rhs.z;
        const auto z = lhs.x * rhs.y - lhs.y * rhs.x;
        return {x, y, z};
    }

    // Compare
    inline constexpr bool compare(const vec3& lhs, const vec3& rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
    }

    inline constexpr bool operator==(const vec3& lhs, const vec3& rhs) {
        return compare(lhs, rhs);
    }

    inline constexpr bool operator!=(const vec3& lhs, const vec3& rhs) {
        return !compare(lhs, rhs);
    }
} // namespace sm