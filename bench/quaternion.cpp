// Copyright(c) 2019-present, Anton Lilja.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <benchmark/benchmark.h>
#include <simple_math/simple_math.hpp>

using namespace sm;

static void quaternion_from_axis(benchmark::State& state) {
    auto angle = radians::from_degrees(90.0F);

    benchmark::ClobberMemory();
    for (auto _ : state) {
        auto q = quaternion::from_axis(angle, 0.0F, 0.0F, 1.0F);
        benchmark::DoNotOptimize(q);
    }
}

static void quaternion_to_axis(benchmark::State& state) {
    const auto q =
        quaternion::from_axis(radians::from_degrees(90.0F), 0.0F, 0.0F, 1.0F);

    for (auto _ : state) {
        const auto [angle, axis] = to_axis(q);
        benchmark::DoNotOptimize(angle);
        benchmark::DoNotOptimize(axis);
    }
}

BENCHMARK(quaternion_from_axis);
BENCHMARK(quaternion_to_axis);