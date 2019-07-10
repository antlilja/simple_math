// Copyright(c) 2019-present, Anton Lilja.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include "catch.hpp"
#include <simple_math/simple_math.hpp>

using namespace sm;

TEST_CASE("Initialization of vec3", "[vec3]") {
    SECTION("Default constructor") {
        vec3 v;
        REQUIRE(v.x == Approx(0.0f));
        REQUIRE(v.y == Approx(0.0f));
        REQUIRE(v.z == Approx(0.0f));
    }

    SECTION("One value constructor") {
        vec3 v(1.0f);
        REQUIRE(v.x == Approx(1.0f));
        REQUIRE(v.y == Approx(1.0f));
        REQUIRE(v.z == Approx(1.0f));
    }

    SECTION("Full constructor") {
        vec3 v(1.0f, 2.0f, 3.0f);
        REQUIRE(v.x == Approx(1.0f));
        REQUIRE(v.y == Approx(2.0f));
        REQUIRE(v.z == Approx(3.0f));
    }
}

TEST_CASE("Inverse of vec3", "[vec3]") {

    vec3 v(2.0f);

    constexpr auto ans = -2.0f;
    const auto calc = v.inverse();
    REQUIRE(calc.x == Approx(ans));
    REQUIRE(calc.y == Approx(ans));
    REQUIRE(calc.z == Approx(ans));
}

TEST_CASE("Magnitude and normilization of vec3", "[vec3]") {

    vec3 v(2.0f);

    SECTION("Magnitude works") {
        REQUIRE(v.magnitude() == Approx(sqrtf(12.0f)));
    }

    SECTION("Normalization works") {
        const float ans = 2.0f / sqrtf(12.0f);
        const auto calc = v.normalize();
        REQUIRE(calc.x == Approx(ans));
        REQUIRE(calc.y == Approx(ans));
        REQUIRE(calc.z == Approx(ans));
    }

    SECTION("Fast normalization works") {
        const float ans = 2.0f / sqrtf(12.0f);
        const auto calc = v.normalize_fast();
        REQUIRE(calc.x == Approx(ans).epsilon(0.01F));
        REQUIRE(calc.y == Approx(ans).epsilon(0.01F));
        REQUIRE(calc.z == Approx(ans).epsilon(0.01F));
    }
}

TEST_CASE("Addition and subtraction of vec3", "[vec3]") {

    SECTION("Uniform vectors") {
        vec3 v0(1.0f);
        vec3 v1(2.0f);

        SECTION("Addition works") {
            constexpr auto ans = 3.0f;
            const auto calc = add(v0, v1);
            REQUIRE(calc.x == Approx(ans));
            REQUIRE(calc.y == Approx(ans));
            REQUIRE(calc.z == Approx(ans));
        }

        SECTION("Subtraction works") {
            constexpr auto ans = 1.0f;
            const auto calc = subtract(v1, v0);
            REQUIRE(calc.x == Approx(ans));
            REQUIRE(calc.y == Approx(ans));
            REQUIRE(calc.z == Approx(ans));
        }
    }

    SECTION("Non-uniform vectors") {
        vec3 v0(1.0f, 2.0f, 3.0f);
        vec3 v1(5.0f, 6.0f, 7.0f);

        SECTION("Addition works") {
            const auto calc = add(v0, v1);
            REQUIRE(calc.x == Approx(6.0f));
            REQUIRE(calc.y == Approx(8.0f));
            REQUIRE(calc.z == Approx(10.0f));
        }

        SECTION("Subtraction works") {
            const auto calc = subtract(v1, v0);
            REQUIRE(calc.x == Approx(4.0f));
            REQUIRE(calc.y == Approx(4.0f));
            REQUIRE(calc.z == Approx(4.0f));
        }
    }
}

TEST_CASE("Multiplication and division by scalars of vec3", "[vec3]") {

    SECTION("Uniform vectors") {
        vec3 v(10.0f);
        constexpr auto s = 5.0f;

        SECTION("Multiplication works") {
            constexpr auto ans = 50.0f;
            const auto calc = multiply(v, s);
            REQUIRE(calc.x == Approx(ans));
            REQUIRE(calc.y == Approx(ans));
            REQUIRE(calc.z == Approx(ans));
        }

        SECTION("Division works") {
            constexpr auto ans = 2.0f;
            const auto calc = divide(v, s);
            REQUIRE(calc.x == Approx(ans));
            REQUIRE(calc.y == Approx(ans));
            REQUIRE(calc.z == Approx(ans));
        }
    }

    SECTION("Non-uniform vectors") {
        vec3 v(1.0f, 2.0f, 3.0f);
        constexpr auto s = 5.0f;

        SECTION("Multiplication works") {
            const auto calc = multiply(v, s);
            REQUIRE(calc.x == Approx(5.0f));
            REQUIRE(calc.y == Approx(10.0f));
            REQUIRE(calc.z == Approx(15.0f));
        }

        SECTION("Division works") {
            const auto calc = divide(v, s);
            REQUIRE(calc.x == Approx(0.2f));
            REQUIRE(calc.y == Approx(0.4f));
            REQUIRE(calc.z == Approx(0.6f));
        }
    }
}

TEST_CASE("Dot, cross and element-wise product of vec3", "[vec3]") {
    vec3 v0(1.0f);
    vec3 v1(2.0f);

    SECTION("Dot product works") { REQUIRE(dot(v0, v1) == Approx(6.0f)); }

    SECTION("Cross product works") {
        const auto calc = cross(v0, v1);
        REQUIRE(calc.x == Approx(0.0f));
        REQUIRE(calc.y == Approx(0.0f));
        REQUIRE(calc.z == Approx(0.0f));
    }

    SECTION("Element-wise product works") {
        const float ans = 2.0f;
        const auto calc = element_wise(v0, v1);
        REQUIRE(calc.x == Approx(ans));
        REQUIRE(calc.y == Approx(ans));
        REQUIRE(calc.z == Approx(ans));
    }
}

TEST_CASE("Comparison of vec3", "[vec3]") {

    SECTION("Equal vectors") {
        vec3 v0(1.0f);
        vec3 v1(1.0f);

        REQUIRE(compare(v0, v1));
    }

    SECTION("Not equal vectors") {
        vec3 v0(1.0f);
        vec3 v1(2.0f);

        REQUIRE(!compare(v0, v1));
    }
}