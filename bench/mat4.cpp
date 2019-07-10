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
static void mat4_multiply(benchmark::State& state) {
    constexpr auto angle = radians::from_degrees(90.0f);
    const auto m0 = mat4::translation(1.0f, 2.0f, 3.0f);
    const auto m1 = mat4::rotation(angle, {0.0f, 0.0f, 1.0f});

    benchmark::DoNotOptimize(m0);
    benchmark::DoNotOptimize(m1);
    for (auto _ : state) {
        benchmark::DoNotOptimize(multiply<simd>(m0, m1));
    }
}

template <simd_t simd>
static void mat4_compare(benchmark::State& state) {
    constexpr auto angle = radians::from_degrees(90.0f);
    const auto m0 = mat4::translation(1.0f, 2.0f, 3.0f);
    const auto m1 = mat4::rotation(angle, {0.0f, 0.0f, 1.0f});

    benchmark::DoNotOptimize(m0);
    benchmark::DoNotOptimize(m1);
    for (auto _ : state) {
        benchmark::DoNotOptimize(compare<simd>(m0, m1));
    }
}

template <simd_t simd>
static void mat4_vec4_transform(benchmark::State& state) {
    const auto m = mat4::translation(1.0f, 2.0f, 3.0f);
    const auto v = vec4(1.0f);

    benchmark::DoNotOptimize(m);
    benchmark::DoNotOptimize(v);
    for (auto _ : state) {
        benchmark::DoNotOptimize(transform_vec<simd>(v, m));
    }
}

// mat4 multiply
BENCHMARK_TEMPLATE(mat4_multiply, AVX);
BENCHMARK_TEMPLATE(mat4_multiply, SSE4);
BENCHMARK_TEMPLATE(mat4_multiply, NO_SIMD);

// mat4 compare
BENCHMARK_TEMPLATE(mat4_compare, SSE2);
BENCHMARK_TEMPLATE(mat4_compare, NO_SIMD);

// vec4 mat4 transform
BENCHMARK_TEMPLATE(mat4_vec4_transform, SSE2);
BENCHMARK_TEMPLATE(mat4_vec4_transform, NO_SIMD);