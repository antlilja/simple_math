// Copyright(c) 2019-present, Anton Lilja.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <catch2/catch.hpp>
#include <simple_math/simple_math.hpp>

using namespace sm;

TEST_CASE("Initialization of vec2", "[vec2]") {
    SECTION("Default constructor") {
        vec2 v;
        REQUIRE(v.x == Approx(0.0f));
        REQUIRE(v.y == Approx(0.0f));
    }

    SECTION("One value constructor") {
        vec2 v(1.0f);
        REQUIRE(v.x == Approx(1.0f));
        REQUIRE(v.y == Approx(1.0f));
    }

    SECTION("Full constructor") {
        vec2 v(1.0f, 2.0f);
        REQUIRE(v.x == Approx(1.0f));
        REQUIRE(v.y == Approx(2.0f));
    }
}

TEST_CASE("Inverse of vec2", "[vec2]") {

    vec2 v(2.0f);

    constexpr auto ans = -2.0f;
    const auto calc = v.inverse();
    REQUIRE(calc.x == Approx(ans));
    REQUIRE(calc.y == Approx(ans));
}

TEST_CASE("Magnitude and normilization of vec2", "[vec2]") {

    vec2 v(2.0f);

    SECTION("Magnitude works") {
        REQUIRE(v.magnitude() == Approx(sqrtf(8.0f)));
    }

    SECTION("Square magnitude works") {
        REQUIRE(v.square_magnitude() == Approx(8.0f));
    }

    SECTION("Normalization works") {
        const float ans = 2.0f / sqrtf(8.0f);
        const auto calc = v.normalize();
        REQUIRE(calc.x == Approx(ans));
        REQUIRE(calc.y == Approx(ans));
    }

    SECTION("Fast normalization works") {
        const float ans = 2.0f / sqrtf(8.0f);
        const auto calc = v.normalize_fast();
        REQUIRE(calc.x == Approx(ans).epsilon(0.01F));
        REQUIRE(calc.y == Approx(ans).epsilon(0.01F));
    }
}

TEST_CASE("Addition and subtraction of vec2", "[vec2]") {

    SECTION("Uniform vectors") {
        vec2 v0(1.0f);
        vec2 v1(2.0f);

        SECTION("Addition works") {
            constexpr auto ans = 3.0f;
            const auto calc = add(v0, v1);
            REQUIRE(calc.x == Approx(ans));
            REQUIRE(calc.y == Approx(ans));
        }

        SECTION("Subtraction works") {
            constexpr auto ans = 1.0f;
            const auto calc = subtract(v1, v0);
            REQUIRE(calc.x == Approx(ans));
            REQUIRE(calc.y == Approx(ans));
        }
    }

    SECTION("Non-uniform vectors") {
        vec2 v0(1.0f, 2.0f);
        vec2 v1(5.0f, 6.0f);

        SECTION("Addition works") {
            const auto calc = add(v0, v1);
            REQUIRE(calc.x == Approx(6.0f));
            REQUIRE(calc.y == Approx(8.0f));
        }

        SECTION("Subtraction works") {
            const auto calc = subtract(v1, v0);
            REQUIRE(calc.x == Approx(4.0f));
            REQUIRE(calc.y == Approx(4.0f));
        }
    }
}

TEST_CASE("Multiplication and division by scalars of vec2", "[vec2]") {

    SECTION("Uniform vectors") {
        vec2 v(10.0f);
        constexpr auto s = 5.0f;

        SECTION("Multiplication works") {
            constexpr auto ans = 50.0f;
            const auto calc = multiply(v, s);
            REQUIRE(calc.x == Approx(ans));
            REQUIRE(calc.y == Approx(ans));
        }

        SECTION("Division works") {
            constexpr auto ans = 2.0f;
            const auto calc = divide(v, s);
            REQUIRE(calc.x == Approx(ans));
            REQUIRE(calc.y == Approx(ans));
        }
    }

    SECTION("Non-uniform vectors") {
        vec2 v(1.0f, 2.0f);
        constexpr auto s = 5.0f;

        SECTION("Multiplication works") {
            const auto calc = multiply(v, s);
            REQUIRE(calc.x == Approx(5.0f));
            REQUIRE(calc.y == Approx(10.0f));
        }

        SECTION("Division works") {
            const auto calc = divide(v, s);
            REQUIRE(calc.x == Approx(0.2f));
            REQUIRE(calc.y == Approx(0.4f));
        }
    }
}

TEST_CASE("Dot and element-wise product of vec2", "[vec2]") {
    vec2 v0(1.0f);
    vec2 v1(2.0f);

    SECTION("Dot product works") { REQUIRE(dot(v0, v1) == Approx(4.0f)); }

    SECTION("Element-wise product works") {
        const float ans = 2.0f;
        const auto calc = element_wise(v0, v1);
        REQUIRE(calc.x == Approx(ans));
        REQUIRE(calc.y == Approx(ans));
    }
}

TEST_CASE("Comparison of vec2", "[vec2]") {

    SECTION("Equal vectors") {
        vec2 v0(1.0f);
        vec2 v1(1.0f);

        REQUIRE(compare(v0, v1));
    }

    SECTION("Not equal vectors") {
        vec2 v0(1.0f);
        vec2 v1(2.0f);

        REQUIRE(!compare(v0, v1));
    }
}