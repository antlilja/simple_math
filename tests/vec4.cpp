// Copyright(c) 2019-present, Anton Lilja.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include "catch.hpp"
#include <simple_math/simple_math.hpp>

using namespace sm;

TEST_CASE("Initialization of vec4", "[vec4]") {
    SECTION("Default constructor") {
        vec4 v;
        REQUIRE(v.x == Approx(0.0f));
        REQUIRE(v.y == Approx(0.0f));
        REQUIRE(v.z == Approx(0.0f));
        REQUIRE(v.w == Approx(0.0f));
    }

    SECTION("One value constructor") {
        vec4 v(1.0f);
        REQUIRE(v.x == Approx(1.0f));
        REQUIRE(v.y == Approx(1.0f));
        REQUIRE(v.z == Approx(1.0f));
        REQUIRE(v.w == Approx(1.0f));
    }

    SECTION("Full constructor") {
        vec4 v(1.0f, 2.0f, 3.0f, 4.0f);
        REQUIRE(v.x == Approx(1.0f));
        REQUIRE(v.y == Approx(2.0f));
        REQUIRE(v.z == Approx(3.0f));
        REQUIRE(v.w == Approx(4.0f));
    }
}

TEMPLATE_TEST_CASE_SIG("Inverse of vec4", "[vec4],[simd]",
                       ((typename T, simd_t S), T, S), (simd_t, simd_t::NONE),
                       (simd_t, simd_t::SSE2)) {

    vec4 v(2.0f);

    constexpr auto ans = -2.0f;
    const auto calc = v.inverse<S>();
    REQUIRE(calc.x == Approx(ans));
    REQUIRE(calc.y == Approx(ans));
    REQUIRE(calc.z == Approx(ans));
    REQUIRE(calc.w == Approx(ans));
}

TEMPLATE_TEST_CASE_SIG("Magnitude and normilization of vec4", "[vec4],[simd]",
                       ((typename T, simd_t S), T, S), (simd_t, simd_t::NONE),
                       (simd_t, simd_t::SSE2), (simd_t, simd_t::SSE4)) {

    vec4 v(2.0f);

    SECTION("Magnitude works") { REQUIRE(v.magnitude<S>() == Approx(4.0f)); }
    SECTION("Square magnitude works") {
        REQUIRE(v.square_magnitude<S>() == Approx(16.0f));
    }

    SECTION("Normalization works") {
        const float ans = 0.5f;
        const auto calc = v.normalize<S>();
        REQUIRE(calc.x == Approx(ans).epsilon(0.01F));
        REQUIRE(calc.y == Approx(ans).epsilon(0.01F));
        REQUIRE(calc.z == Approx(ans).epsilon(0.01F));
        REQUIRE(calc.w == Approx(ans).epsilon(0.01F));
    }

    SECTION("Fast normalization works") {
        const float ans = 0.5F;
        const auto calc = v.normalize_fast<S>();
        REQUIRE(calc.x == Approx(ans).epsilon(0.01F));
        REQUIRE(calc.y == Approx(ans).epsilon(0.01F));
        REQUIRE(calc.z == Approx(ans).epsilon(0.01F));
        REQUIRE(calc.w == Approx(ans).epsilon(0.01F));
    }
}

TEMPLATE_TEST_CASE_SIG("Addition and subtraction of vec4", "[vec4],[simd]",
                       ((typename T, simd_t S), T, S), (simd_t, simd_t::NONE),
                       (simd_t, simd_t::SSE2)) {

    SECTION("Uniform vectors") {
        vec4 v0(1.0f);
        vec4 v1(2.0f);

        SECTION("Addition works") {
            constexpr auto ans = 3.0f;
            const auto calc = add<S>(v0, v1);
            REQUIRE(calc.x == Approx(ans));
            REQUIRE(calc.y == Approx(ans));
            REQUIRE(calc.z == Approx(ans));
            REQUIRE(calc.w == Approx(ans));
        }

        SECTION("Subtraction works") {
            constexpr auto ans = 1.0f;
            const auto calc = subtract<S>(v1, v0);
            REQUIRE(calc.x == Approx(ans));
            REQUIRE(calc.y == Approx(ans));
            REQUIRE(calc.z == Approx(ans));
            REQUIRE(calc.w == Approx(ans));
        }
    }

    SECTION("Non-uniform vectors") {
        vec4 v0(1.0f, 2.0f, 3.0f, 4.0f);
        vec4 v1(5.0f, 6.0f, 7.0f, 8.0f);

        SECTION("Addition works") {
            const auto calc = add<S>(v0, v1);
            REQUIRE(calc.x == Approx(6.0f));
            REQUIRE(calc.y == Approx(8.0f));
            REQUIRE(calc.z == Approx(10.0f));
            REQUIRE(calc.w == Approx(12.0f));
        }

        SECTION("Subtraction works") {
            const auto calc = subtract<S>(v1, v0);
            REQUIRE(calc.x == Approx(4.0f));
            REQUIRE(calc.y == Approx(4.0f));
            REQUIRE(calc.z == Approx(4.0f));
            REQUIRE(calc.w == Approx(4.0f));
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Multiplication and division by scalars of vec4",
                       "[vec4],[simd]", ((typename T, simd_t S), T, S),
                       (simd_t, simd_t::NONE), (simd_t, simd_t::SSE2)) {

    SECTION("Uniform vectors") {
        vec4 v(10.0f);
        constexpr auto s = 5.0f;

        SECTION("Multiplication works") {
            constexpr auto ans = 50.0f;
            const auto calc = multiply<S>(v, s);
            REQUIRE(calc.x == Approx(ans));
            REQUIRE(calc.y == Approx(ans));
            REQUIRE(calc.z == Approx(ans));
            REQUIRE(calc.w == Approx(ans));
        }

        SECTION("Division works") {
            constexpr auto ans = 2.0f;
            const auto calc = divide<S>(v, s);
            REQUIRE(calc.x == Approx(ans));
            REQUIRE(calc.y == Approx(ans));
            REQUIRE(calc.z == Approx(ans));
            REQUIRE(calc.w == Approx(ans));
        }
    }

    SECTION("Non-uniform vectors") {
        vec4 v(1.0f, 2.0f, 3.0f, 4.0f);
        constexpr auto s = 5.0f;

        SECTION("Multiplication works") {
            const auto calc = multiply<S>(v, s);
            REQUIRE(calc.x == Approx(5.0f));
            REQUIRE(calc.y == Approx(10.0f));
            REQUIRE(calc.z == Approx(15.0f));
            REQUIRE(calc.w == Approx(20.0f));
        }

        SECTION("Division works") {
            const auto calc = divide<S>(v, s);
            REQUIRE(calc.x == Approx(0.2f));
            REQUIRE(calc.y == Approx(0.4f));
            REQUIRE(calc.z == Approx(0.6f));
            REQUIRE(calc.w == Approx(0.8f));
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Dot and element-wise product of vec4", "[vec4],[simd]",
                       ((typename T, simd_t S), T, S), (simd_t, simd_t::NONE),
                       (simd_t, simd_t::SSE2), (simd_t, simd_t::SSE4)) {

    vec4 v0(1.0f);
    vec4 v1(2.0f);

    SECTION("Dot product works") { REQUIRE(dot<S>(v0, v1) == Approx(8.0f)); }

    SECTION("Element-wise product works") {
        const float ans = 2.0f;
        const auto calc = element_wise<S>(v0, v1);
        REQUIRE(calc.x == Approx(ans));
        REQUIRE(calc.y == Approx(ans));
        REQUIRE(calc.z == Approx(ans));
        REQUIRE(calc.w == Approx(ans));
    }
}

TEMPLATE_TEST_CASE_SIG("Comparison of vec4", "[vec4],[simd]",
                       ((typename T, simd_t S), T, S), (simd_t, simd_t::NONE),
                       (simd_t, simd_t::SSE2)) {

    SECTION("Equal vectors") {
        vec4 v0(1.0f);
        vec4 v1(1.0f);

        REQUIRE(compare<S>(v0, v1));
    }

    SECTION("Not equal vectors") {
        vec4 v0(1.0f);
        vec4 v1(2.0f);

        REQUIRE(!compare<S>(v0, v1));
    }
}