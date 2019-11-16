// Copyright(c) 2019-present, Anton Lilja.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once
#include "common.hpp"
#include "utility.hpp"
#include "vec.hpp"

namespace sm {
    inline float exact_linear_to_srgb(float l) {
        l = l < 0.0f ? 0.0f : l;
        l = l > 1.0f ? 1.0f : l;

        return l <= 0.0031308f ? l * 12.92f
                               : 1.055f * powf(l, 1.0f / 2.4f) - 0.055f;
    }

    inline vec3 exact_linear_to_srgb(const vec3& L) {
        return vec3(exact_linear_to_srgb(L.x), exact_linear_to_srgb(L.y),
                    exact_linear_to_srgb(L.z));
    }

    // RGB

    inline constexpr uint32_t pack_rgb_to_u32(const vec3& c) {
        return (static_cast<uint32_t>(1) << 24) |
               (ceil_positive_to<uint32_t>(c.b) << 16) |
               (ceil_positive_to<uint32_t>(c.g) << 8) |
               (ceil_positive_to<uint32_t>(c.r) << 0);
    }

    inline constexpr uint32_t pack_rgba_to_u32(const vec4& c) {
        return ((ceil_positive_to<uint32_t>(c.a) << 24) |
                (ceil_positive_to<uint32_t>(c.b) << 16) |
                (ceil_positive_to<uint32_t>(c.g) << 8) |
                (ceil_positive_to<uint32_t>(c.r) << 0));
    }

    inline constexpr vec3 unpack_rgb_to_u32(uint32_t c) {
        return {static_cast<float>((c >> 8) & 0xFF),
                static_cast<float>((c >> 16) & 0xFF),
                static_cast<float>((c >> 24) & 0xFF)};
    }

    inline constexpr vec4 unpack_rgba_to_u32(uint32_t c) {
        return {static_cast<float>((c >> 0) & 0xFF),
                static_cast<float>((c >> 8) & 0xFF),
                static_cast<float>((c >> 16) & 0xFF),
                static_cast<float>((c >> 24) & 0xFF)};
    }

    // BGR

    inline constexpr uint32_t pack_bgr_to_u32(const vec3& c) {
        return (static_cast<uint32_t>(1) << 24) |
               (ceil_positive_to<uint32_t>(c.r) << 16) |
               (ceil_positive_to<uint32_t>(c.g) << 8) |
               (ceil_positive_to<uint32_t>(c.b) << 0);
    }

    inline constexpr uint32_t pack_bgra_to_u32(const vec4& c) {
        return ((ceil_positive_to<uint32_t>(c.a) << 24) |
                (ceil_positive_to<uint32_t>(c.r) << 16) |
                (ceil_positive_to<uint32_t>(c.g) << 8) |
                (ceil_positive_to<uint32_t>(c.b) << 0));
    }

    inline constexpr vec3 unpack_bgr_to_u32(const uint32_t c) {
        return {static_cast<float>((c >> 16) & 0xFF),
                static_cast<float>((c >> 8) & 0xFF),
                static_cast<float>((c >> 0) & 0xFF)};
    }

    inline constexpr vec4 unpack_bgra_to_u32(const uint32_t c) {
        return {static_cast<float>((c >> 16) & 0xFF),
                static_cast<float>((c >> 8) & 0xFF),
                static_cast<float>((c >> 0) & 0xFF),
                static_cast<float>((c >> 24) & 0xFF)};
    }

    // SRGB
    inline constexpr vec4 srgb_to_linear(const vec4& c) {
        return {square((static_cast<float>(1) / static_cast<float>(255)) * c.r),
                square((static_cast<float>(1) / static_cast<float>(255)) * c.g),
                square((static_cast<float>(1) / static_cast<float>(255)) * c.b),
                (static_cast<float>(1) / static_cast<float>(255)) * c.a};
    }

    inline vec4 linear_to_srgb(const vec4& c) {
        return {static_cast<float>(255) * sqrtf(c.r),
                static_cast<float>(255) * sqrtf(c.g),
                static_cast<float>(255) * sqrtf(c.b),
                static_cast<float>(255) * c.a};
    }
} // namespace sm
