// Copyright(c) 2019-present, Anton Lilja.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once
#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"

namespace sm {
    // Other op funcs
    template <typename V>
    inline V inverse(const V& v) {
        return v.inverse();
    }

    template <typename V>
    inline float magnitude(const V& v) {
        return v.magnitude();
    }

    template <typename V>
    inline float square_magnitude(const V& v) {
        return v.square_magnitude();
    }

    template <typename V>
    inline V normalize(const V& v) {
        return v.normalize();
    }
} // namespace sm
