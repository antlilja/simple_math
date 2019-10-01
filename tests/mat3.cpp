// Copyright(c) 2019-present, Anton Lilja.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include "catch.hpp"
#include <simple_math/simple_math.hpp>

using namespace sm;

TEST_CASE("Identity mat3", "[mat3]") {
    const auto m = mat3::identity();

    REQUIRE(m.elements[0 + 0 * 3] == Approx(1.0f));
    REQUIRE(m.elements[1 + 1 * 3] == Approx(1.0f));
    REQUIRE(m.elements[2 + 2 * 3] == Approx(1.0f));
}

TEST_CASE("Translation mat3", "[mat3]") {
    const auto m = mat3::translation({1.0f, 2.0f});

    REQUIRE(m.elements[0 + 0 * 3] == Approx(1.0f));
    REQUIRE(m.elements[1 + 1 * 3] == Approx(1.0f));
    REQUIRE(m.elements[2 + 2 * 3] == Approx(1.0f));

    REQUIRE(m.elements[0 + 2 * 3] == Approx(1.0f));
    REQUIRE(m.elements[1 + 2 * 3] == Approx(2.0f));
}

TEST_CASE("Rotation mat3", "[mat3]") {
    constexpr radians angle(PI / 2.0f);
    const auto m = mat3::rotation(angle);

    REQUIRE(m.elements[2 + 2 * 3] == Approx(1.0f));

    REQUIRE(m.elements[0 + 0 * 3] == Approx(taylor_cosine(angle)));
    REQUIRE(m.elements[1 + 0 * 3] == Approx(taylor_sine(angle)));

    REQUIRE(m.elements[0 + 1 * 3] == Approx(-taylor_sine(angle)));
    REQUIRE(m.elements[1 + 1 * 3] == Approx(taylor_cosine(angle)));
}

TEST_CASE("Scale mat3", "[mat3]") {
    const auto m = mat3::scale({1.0f, 2.0f});

    REQUIRE(m.elements[2 + 2 * 3] == Approx(1.0f));

    REQUIRE(m.elements[0 + 0 * 3] == Approx(1.0f));
    REQUIRE(m.elements[1 + 1 * 3] == Approx(2.0f));
}

TEST_CASE("Comparison of 3x3 matrices", "[mat3]") {
    SECTION("Equal matrices") {
        const auto m0 = mat3::identity();
        const auto m1 = mat3::identity();

        REQUIRE(compare(m0, m1));
    }

    SECTION("Not equal matrices") {
        const auto m0 = mat3::identity();
        const auto m1 = mat3::translation({1.0f, 1.0f});

        REQUIRE(!compare(m0, m1));
    }
}