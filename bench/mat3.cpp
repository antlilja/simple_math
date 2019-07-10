// Copyright(c) 2019-present, Anton Lilja.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <benchmark/benchmark.h>
#include <simple_math/simple_math.hpp>

using namespace sm;

static void mat3_multiply(benchmark::State& state) {
    const auto angle = radians::from_degrees(90.0f);
    const auto m0 = mat3::translation(1.0f, 2.0f);
    const auto m1 = mat3::rotation(angle);

    benchmark::DoNotOptimize(m0);
    benchmark::DoNotOptimize(m1);
    for (auto _ : state) {
        benchmark::DoNotOptimize(multiply(m0, m1));
    }
}

static void mat3_compare(benchmark::State& state) {
    const auto angle = radians::from_degrees(90.0f);
    const auto m0 = mat3::translation(1.0f, 2.0f);
    const auto m1 = mat3::rotation(angle);

    benchmark::DoNotOptimize(m0);
    benchmark::DoNotOptimize(m1);
    for (auto _ : state) {
        benchmark::DoNotOptimize(compare(m0, m1));
    }
}

BENCHMARK(mat3_multiply);
BENCHMARK(mat3_compare);