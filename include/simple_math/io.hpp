// Copyright(c) 2019-present, Anton Lilja.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once
#include "vec.hpp"
#include "mat.hpp"

template <typename OStream>
inline OStream& operator<<(OStream& os, const sm::vec2& v) {
    return os << '[' << v.x << ", " << v.y << ']';
}

template <typename OStream>
inline OStream& operator<<(OStream& os, const sm::vec3& v) {
    return os << '[' << v.x << ", " << v.y << ", " << v.z << ']';
}

template <typename OStream>
inline OStream& operator<<(OStream& os, const sm::vec4& v) {
    return os << '[' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ']';
}

template <typename OStream>
inline OStream& operator<<(OStream& os, const sm::mat3& m) {
    return os << '[' << m.elements[0 + 0 * 3] << ", " << m.elements[0 + 1 * 3]
              << ", " << m.elements[0 + 2 * 3] << "]\n["

              << m.elements[1 + 0 * 3] << ", " << m.elements[1 + 1 * 3] << ", "
              << m.elements[1 + 2 * 3] << "]\n["

              << m.elements[2 + 0 * 3] << ", " << m.elements[2 + 1 * 3] << ", "
              << m.elements[2 + 2 * 3] << "]\n";
}

template <typename OStream>
inline OStream& operator<<(OStream& os, const sm::mat4& m) {
    return os << '[' << m.elements[0 + 0 * 4] << ", " << m.elements[0 + 1 * 4]
              << ", " << m.elements[0 + 2 * 4] << ", " << m.elements[0 + 3 * 4]
              << "]\n["

              << m.elements[1 + 0 * 4] << ", " << m.elements[1 + 1 * 4] << ", "
              << m.elements[1 + 2 * 4] << ", " << m.elements[1 + 3 * 4]
              << "]\n["

              << m.elements[2 + 0 * 4] << ", " << m.elements[2 + 1 * 4] << ", "
              << m.elements[2 + 2 * 4] << ", " << m.elements[2 + 3 * 4]
              << "]\n["

              << m.elements[3 + 0 * 4] << ", " << m.elements[3 + 1 * 4] << ", "
              << m.elements[3 + 2 * 4] << ", " << m.elements[3 + 3 * 4]
              << "]\n";
}
