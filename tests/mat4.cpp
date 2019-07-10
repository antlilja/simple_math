// Copyright(c) 2019-present, Anton Lilja.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include "catch.hpp"
#include <simple_math/simple_math.hpp>

using namespace sm;

TEST_CASE("Identity mat4", "[mat4]") {
    const auto m = mat4::identity();

    REQUIRE(m.elements[0 + 0 * 4] == Approx(1.0f));
    REQUIRE(m.elements[1 + 1 * 4] == Approx(1.0f));
    REQUIRE(m.elements[2 + 2 * 4] == Approx(1.0f));
    REQUIRE(m.elements[3 + 3 * 4] == Approx(1.0f));
}

TEST_CASE("Translation mat4", "[mat4]") {
    const auto m = mat4::translation(1.0f, 2.0f, 3.0f);

    REQUIRE(m.elements[0 + 0 * 4] == Approx(1.0f));
    REQUIRE(m.elements[1 + 1 * 4] == Approx(1.0f));
    REQUIRE(m.elements[2 + 2 * 4] == Approx(1.0f));
    REQUIRE(m.elements[3 + 3 * 4] == Approx(1.0f));

    REQUIRE(m.elements[0 + (3 * 4)] == Approx(1.0f));
    REQUIRE(m.elements[1 + (3 * 4)] == Approx(2.0f));
    REQUIRE(m.elements[2 + (3 * 4)] == Approx(3.0f));
}

TEST_CASE("Rotation mat4", "[mat4]") {
    constexpr radians angle(PI / 2.0f);
    const auto m = mat4::rotation(angle, {0.0f, 0.0f, 1.0f});

    REQUIRE(m.elements[2 + 2 * 4] == Approx(1.0f));
    REQUIRE(m.elements[3 + 3 * 4] == Approx(1.0f));

    REQUIRE(m.elements[0 + (0 * 4)] == Approx(cosf(angle)));
    REQUIRE(m.elements[1 + (0 * 4)] == Approx(sinf(angle)));

    REQUIRE(m.elements[0 + (1 * 4)] == Approx(-sinf(angle)));
    REQUIRE(m.elements[1 + (1 * 4)] == Approx(cosf(angle)));
}

TEST_CASE("Scale mat4", "[mat4]") {
    const auto m = mat4::scale(1.0f, 2.0f, 3.0f);

    REQUIRE(m.elements[3 + 3 * 4] == Approx(1.0f));

    REQUIRE(m.elements[0 + 0 * 4] == Approx(1.0f));
    REQUIRE(m.elements[1 + 1 * 4] == Approx(2.0f));
    REQUIRE(m.elements[2 + 2 * 4] == Approx(3.0f));
}

TEST_CASE("Ortho mat4", "[mat4]") {
    const auto m = mat4::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, 0.1f, 1000.0f);

    REQUIRE(m.elements[0 + 0 * 4] == Approx((2.0f / 1920.0f)));
    REQUIRE(m.elements[1 + 1 * 4] == Approx((2.0f / 1080.0f)));
    REQUIRE(m.elements[2 + 2 * 4] == Approx((-2.0f / (1000.0f - 0.1))));
    REQUIRE(m.elements[3 + 3 * 4] == Approx(1.0f));

    REQUIRE(m.elements[0 + 3 * 4] == Approx(1.0f));
    REQUIRE(m.elements[1 + 3 * 4] == Approx(1.0f));
    REQUIRE(m.elements[2 + 3 * 4] ==
            Approx(((0.1f + 1000.0f) / (1000.0f - 0.1f))));
}

TEST_CASE("Perspective mat4", "[mat4]") {
    const auto aspect = 1920.0f / 1080.0f;
    const auto angle = radians(PI / 2.0f);
    const auto m = mat4::perspective(angle, aspect, 0.1f, 10.0f);

    const auto top = tanf(angle / 2.0f) * 0.1f;

    REQUIRE(m.elements[0 + 0 * 4] == Approx(0.1f / (aspect * top)));
    REQUIRE(m.elements[1 + 1 * 4] == Approx(-(0.1f / top)));
    REQUIRE(m.elements[2 + 2 * 4] ==
            Approx((-(10.0f + 0.1f) / (10.0f - 0.1f))));
    REQUIRE(m.elements[3 + 2 * 4] == Approx((-1.0f)));
    REQUIRE(m.elements[2 + 3 * 4] ==
            Approx((-(2.0f * 10.0f * 0.1f) / (10.0f - 0.1f))));
}

TEMPLATE_TEST_CASE_SIG("Multiplication of mat4", "[mat4],[simd]",
                       ((typename T, simd_t S), T, S), (simd_t, simd_t::NONE),
                       (simd_t, simd_t::SSE2), (simd_t, simd_t::AVX)) {

    const auto angle = radians(PI / 2.0f);
    const auto m0 = mat4::translation(1.0f, 0.0f, 1.0f);
    const auto m1 = mat4::rotation(angle, {0.0f, 0.0f, 1.0f});

    const auto calc = multiply<S>(m0, m1);

    mat4 ans;
    for (int32_t y = 0; y < 4; ++y) {
        for (int32_t x = 0; x < 4; ++x) {
            for (int32_t e = 0; e < 4; ++e) {
                ans.elements[x + y * 4] +=
                    m0.elements[x + e * 4] * m1.elements[e + y * 4];
            }
        }
    }

    // Test
    for (int32_t i = 0; i < 16; ++i) {
        REQUIRE(calc.elements[i] == Approx(ans.elements[i]));
    }
}

TEMPLATE_TEST_CASE_SIG("Comparison of mat4", "[mat4],[simd]",
                       ((typename T, simd_t S), T, S), (simd_t, simd_t::NONE),
                       (simd_t, simd_t::SSE2)) {

    SECTION("Equal matrices") {
        const auto m0 = mat4::identity();
        const auto m1 = mat4::identity();

        REQUIRE(compare(m0, m1));
    }

    SECTION("Not equal matrices") {
        const auto m0 = mat4::identity();
        const auto m1 = mat4::translation(1.0f, 2.0f, 3.0f);

        REQUIRE(!compare(m0, m1));
    }
}
