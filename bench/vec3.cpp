// Copyright(c) 2019-present, Anton Lilja.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <benchmark/benchmark.h>
#include <simple_math/simple_math.hpp>

using namespace sm;

static void vec3_magnitude(benchmark::State& state) {
    const auto v = vec3(2.0f);

    benchmark::DoNotOptimize(v);
    for (auto _ : state) {
        benchmark::DoNotOptimize(magnitude(v));
    }
}

static void vec3_normalize(benchmark::State& state) {
    const auto v = vec3(2.0f);

    benchmark::DoNotOptimize(v);
    for (auto _ : state) {
        benchmark::DoNotOptimize(v.normalize());
    }
}

static void vec3_normalize_fast(benchmark::State& state) {
    const auto v = vec3(2.0f);

    benchmark::DoNotOptimize(v);
    for (auto _ : state) {
        benchmark::DoNotOptimize(v.normalize_fast());
    }
}

static void vec3_add(benchmark::State& state) {
    const auto v0 = vec3(1.0f);
    const auto v1 = vec3(1.0f);

    benchmark::DoNotOptimize(v0);
    benchmark::DoNotOptimize(v1);
    for (auto _ : state) {
        benchmark::DoNotOptimize(add(v0, v1));
    }
}

static void vec3_subtract(benchmark::State& state) {
    const auto v0 = vec3(1.0f);
    const auto v1 = vec3(1.0f);

    benchmark::DoNotOptimize(v0);
    benchmark::DoNotOptimize(v1);
    for (auto _ : state) {
        benchmark::DoNotOptimize(subtract(v0, v1));
    }
}

static void vec3_multiply(benchmark::State& state) {
    const auto v = vec3(1.0f);
    const auto s = 2.0f;

    benchmark::DoNotOptimize(v);
    benchmark::DoNotOptimize(s);
    for (auto _ : state) {
        benchmark::DoNotOptimize(multiply(v, s));
    }
}

static void vec3_divide(benchmark::State& state) {
    const auto v = vec3(1.0f);
    const auto s = 2.0f;

    benchmark::DoNotOptimize(v);
    benchmark::DoNotOptimize(s);
    for (auto _ : state) {
        benchmark::DoNotOptimize(divide(v, s));
    }
}

static void vec3_element_wise(benchmark::State& state) {
    const auto v0 = vec3(1.0f);
    const auto v1 = vec3(2.0f);

    benchmark::DoNotOptimize(v0);
    benchmark::DoNotOptimize(v1);
    for (auto _ : state) {
        benchmark::DoNotOptimize(element_wise(v0, v1));
    }
}

static void vec3_dot(benchmark::State& state) {
    auto v0 = vec3(1.0f);
    auto v1 = vec3(2.0f);

    benchmark::DoNotOptimize(v0);
    benchmark::DoNotOptimize(v1);
    for (auto _ : state) {
        benchmark::DoNotOptimize(dot(v0, v1));
    }
}

static void vec3_cross(benchmark::State& state) {
    auto v0 = vec3(1.0f);
    auto v1 = vec3(2.0f);

    benchmark::DoNotOptimize(v0);
    benchmark::DoNotOptimize(v1);
    for (auto _ : state) {
        benchmark::DoNotOptimize(cross(v0, v1));
    }
}

static void vec3_compare(benchmark::State& state) {
    const auto v0 = vec3(1.0f);
    const auto v1 = vec3(2.0f);

    benchmark::DoNotOptimize(v0);
    benchmark::DoNotOptimize(v1);
    for (auto _ : state) {
        benchmark::DoNotOptimize(compare(v0, v1));
    }
}

BENCHMARK(vec3_magnitude);
BENCHMARK(vec3_normalize);
BENCHMARK(vec3_normalize_fast);
BENCHMARK(vec3_add);
BENCHMARK(vec3_subtract);
BENCHMARK(vec3_multiply);
BENCHMARK(vec3_divide);
BENCHMARK(vec3_element_wise);
BENCHMARK(vec3_dot);
BENCHMARK(vec3_cross);
BENCHMARK(vec3_compare);