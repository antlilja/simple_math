// Copyright(c) 2019-present, Anton Lilja.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once
#include "common.hpp"
#include "utility.hpp"

namespace sm {
    union SIMPLE_MATH_ALIGN(8) vec2 {
        // Data
        float elements[2];

        struct {
            float x, y;
        };

        // Constructors
        inline constexpr vec2() : elements{0.0F, 0.0F} {}

        inline constexpr vec2(float v) : elements{v, v} {}

        inline constexpr vec2(float _x, float _y) : elements{_x, _y} {}

        // Operations
        inline constexpr vec2 inverse() const { return {-x, -y}; }

        inline constexpr vec2 operator-() const { return inverse(); }

        constexpr float square_magnitude() const { return x * x + y * y; }
        inline float magnitude() const { return std::sqrt(square_magnitude()); }

        inline vec2 normalize() const {
            const auto mag = magnitude();

            SIMPLE_MATH_ASSERT(mag != 0.0F);
            return {x / mag, y / mag};
        }

        inline vec2 normalize_fast() const {
            const auto sqr_mag = square_magnitude();

            SIMPLE_MATH_ASSERT(sqr_mag != 0.0F);
            const auto inv_sqrt = fast_inverse_sqrt(sqr_mag);
            return {x * inv_sqrt, y * inv_sqrt};
        }
    };

    // Operations //

    // Add
    inline constexpr vec2 add(const vec2& lhs, const vec2& rhs) {
        return {lhs.x + rhs.x, lhs.y + rhs.y};
    }

    inline constexpr vec2 operator+(const vec2& lhs, const vec2& rhs) {
        return add(lhs, rhs);
    }

    inline constexpr vec2& operator+=(vec2& lhs, const vec2& rhs) {
        return lhs = add(lhs, rhs);
    }

    // Subtract
    inline constexpr vec2 subtract(const vec2& lhs, const vec2& rhs) {
        return {lhs.x - rhs.x, lhs.y - rhs.y};
    }

    inline constexpr vec2 operator-(const vec2& lhs, const vec2& rhs) {
        return subtract(lhs, rhs);
    }

    inline constexpr vec2& operator-=(vec2& lhs, const vec2& rhs) {
        return lhs = subtract(lhs, rhs);
    }

    // Multiply
    inline constexpr vec2 multiply(const vec2& v, const float scalar) {
        return {v.x * scalar, v.y * scalar};
    }

    inline constexpr vec2 operator*(const vec2& v, const float scalar) {
        return multiply(v, scalar);
    }

    inline constexpr vec2 operator*(const float scalar, const vec2& v) {
        return multiply(v, scalar);
    }

    inline constexpr vec2& operator*=(vec2& v, const float scalar) {
        return v = multiply(v, scalar);
    }

    // Divide
    inline vec2 divide(const vec2& v, const float scalar) {
        SIMPLE_MATH_ASSERT(scalar != 0.0F);
        return {v.x / scalar, v.y / scalar};
    }

    inline vec2 operator/(const vec2& v, const float scalar) {
        return divide(v, scalar);
    }

    inline vec2& operator/=(vec2& v, const float scalar) {
        return v = divide(v, scalar);
    }

    // Element-wise product
    inline constexpr vec2 element_wise(const vec2& lhs, const vec2& rhs) {
        return {lhs.x * rhs.x, lhs.y * rhs.y};
    }

    // Dot product
    inline constexpr float dot(const vec2& lhs, const vec2& rhs) {
        return lhs.x * rhs.x + lhs.y * rhs.y;
    }

    // Compare
    inline constexpr bool compare(const vec2& lhs, const vec2& rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }

    inline constexpr bool operator==(const vec2& lhs, const vec2& rhs) {
        return compare(lhs, rhs);
    }

    inline constexpr bool operator!=(const vec2& lhs, const vec2& rhs) {
        return !compare(lhs, rhs);
    }
} // namespace sm