// Copyright(c) 2019-present, Anton Lilja.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <benchmark/benchmark.h>
#include <simple_math/simple_math.hpp>

using namespace sm;

static void vec2_magnitude(benchmark::State& state) {
    const auto v = vec2(2.0f);

    benchmark::DoNotOptimize(v);
    for (auto _ : state) {
        benchmark::DoNotOptimize(magnitude(v));
    }
}

static void vec2_normalize(benchmark::State& state) {
    const auto v = vec2(2.0f);

    benchmark::DoNotOptimize(v);
    for (auto _ : state) {
        benchmark::DoNotOptimize(v.normalize());
    }
}

static void vec2_normalize_fast(benchmark::State& state) {
    const auto v = vec2(2.0f);

    benchmark::DoNotOptimize(v);
    for (auto _ : state) {
        benchmark::DoNotOptimize(v.normalize_fast());
    }
}

static void vec2_add(benchmark::State& state) {
    const auto v0 = vec2(1.0f);
    const auto v1 = vec2(1.0f);

    benchmark::DoNotOptimize(v0);
    benchmark::DoNotOptimize(v1);
    for (auto _ : state) {
        benchmark::DoNotOptimize(add(v0, v1));
    }
}

static void vec2_subtract(benchmark::State& state) {
    const auto v0 = vec2(1.0f);
    const auto v1 = vec2(1.0f);

    benchmark::DoNotOptimize(v0);
    benchmark::DoNotOptimize(v1);
    for (auto _ : state) {
        benchmark::DoNotOptimize(subtract(v0, v1));
    }
}

static void vec2_multiply(benchmark::State& state) {
    const auto v = vec2(1.0f);
    const auto s = 2.0f;

    benchmark::DoNotOptimize(v);
    benchmark::DoNotOptimize(s);
    for (auto _ : state) {
        benchmark::DoNotOptimize(multiply(v, s));
    }
}

static void vec2_divide(benchmark::State& state) {
    const auto v = vec2(1.0f);
    const auto s = 2.0f;

    benchmark::DoNotOptimize(v);
    benchmark::DoNotOptimize(s);
    for (auto _ : state) {
        benchmark::DoNotOptimize(divide(v, s));
    }
}

static void vec2_element_wise(benchmark::State& state) {
    const auto v0 = vec2(1.0f);
    const auto v1 = vec2(2.0f);

    benchmark::DoNotOptimize(v0);
    benchmark::DoNotOptimize(v1);
    for (auto _ : state) {
        benchmark::DoNotOptimize(element_wise(v0, v1));
    }
}

static void vec2_dot(benchmark::State& state) {
    auto v0 = vec2(1.0f);
    auto v1 = vec2(2.0f);

    benchmark::DoNotOptimize(v0);
    benchmark::DoNotOptimize(v1);
    for (auto _ : state) {
        benchmark::DoNotOptimize(dot(v0, v1));
    }
}

static void vec2_compare(benchmark::State& state) {
    const auto v0 = vec2(1.0f);
    const auto v1 = vec2(2.0f);

    benchmark::DoNotOptimize(v0);
    benchmark::DoNotOptimize(v1);
    for (auto _ : state) {
        benchmark::DoNotOptimize(compare(v0, v1));
    }
}

BENCHMARK(vec2_magnitude);
BENCHMARK(vec2_normalize);
BENCHMARK(vec2_normalize_fast);
BENCHMARK(vec2_add);
BENCHMARK(vec2_subtract);
BENCHMARK(vec2_multiply);
BENCHMARK(vec2_divide);
BENCHMARK(vec2_element_wise);
BENCHMARK(vec2_dot);
BENCHMARK(vec2_compare);