// Copyright(c) 2019-present, Anton Lilja.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <catch2/catch.hpp>
#include <simple_math/simple_math.hpp>

#include <cmath>

using namespace sm;

constexpr float max_diff = 0.01F;

// sine
TEST_CASE("taylor_sine test", "[trig]") {
    for (float angle = -2.0F * TAU; angle < 2.0F * TAU; angle += PI / 8.0F) {
        const auto diff = taylor_sine(radians(angle)) - std::sin(angle);
        REQUIRE(diff < max_diff);
        REQUIRE(diff > -max_diff);
    }
}

// cosine
TEST_CASE("taylor_cosine test", "[trig]") {
    for (float angle = -2.0F * TAU; angle < 2.0F * TAU; angle += PI / 8.0F) {
        const auto diff = taylor_cosine(radians(angle)) - std::cos(angle);
        REQUIRE(diff < max_diff);
        REQUIRE(diff > -max_diff);
    }
}