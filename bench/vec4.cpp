// Copyright(c) 2019-present, Anton Lilja.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <benchmark/benchmark.h>
#include <simple_math/simple_math.hpp>

using namespace sm;

constexpr simd_t NO_SIMD = simd_t::NONE;
constexpr simd_t SSE2 = simd_t::SSE2;
constexpr simd_t SSE4 = simd_t::SSE4;
constexpr simd_t AVX = simd_t::AVX;

template <simd_t simd>
static void vec4_magnitude(benchmark::State& state) {
    const auto v = vec4(2.0f);

    benchmark::DoNotOptimize(v);
    for (auto _ : state) {
        benchmark::DoNotOptimize(v.magnitude<simd>());
    }
}

template <simd_t simd>
static void vec4_normalize(benchmark::State& state) {
    const auto v = vec4(2.0f);

    benchmark::DoNotOptimize(v);
    for (auto _ : state) {
        benchmark::DoNotOptimize(v.normalize<simd>());
    }
}

template <simd_t simd>
static void vec4_normalize_fast(benchmark::State& state) {
    const auto v = vec4(2.0f);

    benchmark::DoNotOptimize(v);
    for (auto _ : state) {
        benchmark::DoNotOptimize(v.normalize_fast<simd>());
    }
}

template <simd_t simd>
static void vec4_add(benchmark::State& state) {
    const auto v0 = vec4(1.0f);
    const auto v1 = vec4(1.0f);

    benchmark::DoNotOptimize(v0);
    benchmark::DoNotOptimize(v1);
    for (auto _ : state) {
        benchmark::DoNotOptimize(add<simd>(v0, v1));
    }
}

template <simd_t simd>
static void vec4_subtract(benchmark::State& state) {
    const auto v0 = vec4(1.0f);
    const auto v1 = vec4(1.0f);

    benchmark::DoNotOptimize(v0);
    benchmark::DoNotOptimize(v1);
    for (auto _ : state) {
        benchmark::DoNotOptimize(subtract<simd>(v0, v1));
    }
}

template <simd_t simd>
static void vec4_multiply(benchmark::State& state) {
    const auto v = vec4(1.0f);
    const auto s = 2.0f;

    benchmark::DoNotOptimize(v);
    benchmark::DoNotOptimize(s);
    for (auto _ : state) {
        benchmark::DoNotOptimize(multiply<simd>(v, s));
    }
}

template <simd_t simd>
static void vec4_divide(benchmark::State& state) {
    const auto v = vec4(1.0f);
    const auto s = 2.0f;

    benchmark::DoNotOptimize(v);
    benchmark::DoNotOptimize(s);
    for (auto _ : state) {
        benchmark::DoNotOptimize(divide<simd>(v, s));
    }
}

template <simd_t simd>
static void vec4_element_wise(benchmark::State& state) {
    const auto v0 = vec4(1.0f);
    const auto v1 = vec4(2.0f);

    benchmark::DoNotOptimize(v0);
    benchmark::DoNotOptimize(v1);
    for (auto _ : state) {
        benchmark::DoNotOptimize(element_wise<simd>(v0, v1));
    }
}

template <simd_t simd>
static void vec4_dot(benchmark::State& state) {
    auto v0 = vec4(1.0f);
    auto v1 = vec4(2.0f);

    benchmark::DoNotOptimize(v0);
    benchmark::DoNotOptimize(v1);
    for (auto _ : state) {
        benchmark::DoNotOptimize(dot<simd>(v0, v1));
    }
}

template <simd_t simd>
static void vec4_compare(benchmark::State& state) {
    const auto v0 = vec4(1.0f);
    const auto v1 = vec4(2.0f);

    benchmark::DoNotOptimize(v0);
    benchmark::DoNotOptimize(v1);
    for (auto _ : state) {
        benchmark::DoNotOptimize(compare<simd>(v0, v1));
    }
}

// vec4 magnitude
BENCHMARK_TEMPLATE(vec4_magnitude, SSE4);
BENCHMARK_TEMPLATE(vec4_magnitude, SSE2);
BENCHMARK_TEMPLATE(vec4_magnitude, NO_SIMD);

// vec4 normalize
BENCHMARK_TEMPLATE(vec4_normalize, SSE4);
BENCHMARK_TEMPLATE(vec4_normalize, SSE2);
BENCHMARK_TEMPLATE(vec4_normalize, NO_SIMD);

// vec4 normalize_fast
BENCHMARK_TEMPLATE(vec4_normalize_fast, SSE4);
BENCHMARK_TEMPLATE(vec4_normalize_fast, SSE2);
BENCHMARK_TEMPLATE(vec4_normalize_fast, NO_SIMD);

// vec4 add
BENCHMARK_TEMPLATE(vec4_add, SSE2);
BENCHMARK_TEMPLATE(vec4_add, NO_SIMD);

// vec4 subtract
BENCHMARK_TEMPLATE(vec4_subtract, SSE2);
BENCHMARK_TEMPLATE(vec4_subtract, NO_SIMD);

// vec4 multiply
BENCHMARK_TEMPLATE(vec4_multiply, SSE2);
BENCHMARK_TEMPLATE(vec4_multiply, NO_SIMD);

// vec4 divide
BENCHMARK_TEMPLATE(vec4_divide, SSE2);
BENCHMARK_TEMPLATE(vec4_divide, NO_SIMD);

// vec4 element-wise
BENCHMARK_TEMPLATE(vec4_element_wise, SSE2);
BENCHMARK_TEMPLATE(vec4_element_wise, NO_SIMD);

// vec4 dot
BENCHMARK_TEMPLATE(vec4_dot, SSE2);
BENCHMARK_TEMPLATE(vec4_dot, NO_SIMD);

// vec4 compare
BENCHMARK_TEMPLATE(vec4_compare, SSE2);
BENCHMARK_TEMPLATE(vec4_compare, NO_SIMD);