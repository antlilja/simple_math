// Copyright(c) 2019-present, Anton Lilja.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include <benchmark/benchmark.h>
#include <simple_math/simple_math.hpp>

#include <random>

using namespace sm;

// sine
constexpr bool negative_pi_pi_bounded = true;
constexpr bool not_negative_pi_pi_bounded = false;
constexpr bool negative_hpi_hpi_bounded = true;
constexpr bool not_negative_hpi_hpi_bounded = false;

// cosine
constexpr bool zero_tau_bounded = true;
constexpr bool not_zero_tau_bounded = false;
constexpr bool zero_pi_bounded = true;
constexpr bool not_zero_pi_bounded = false;

// sine
template <bool b1, bool b2>
static void trig_taylor_sine(benchmark::State& state) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(-TAU, TAU);
    auto angle = radians(dist(mt));

    benchmark::DoNotOptimize(angle);
    for (auto _ : state) {
        auto s = taylor_sine<b1, b2>(angle);
        benchmark::DoNotOptimize(s);
    }
}

static void trig_sin(benchmark::State& state) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(-TAU, TAU);
    auto angle = radians(dist(mt));

    benchmark::DoNotOptimize(angle);
    for (auto _ : state) {
        auto s = std::sin(angle);
        benchmark::DoNotOptimize(s);
    }
}

// cosine
template <bool b1, bool b2>
static void trig_taylor_cosine(benchmark::State& state) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(-TAU, TAU);
    auto angle = radians(dist(mt));

    benchmark::DoNotOptimize(angle);
    for (auto _ : state) {
        auto s = taylor_cosine<b1, b2>(angle);
        benchmark::DoNotOptimize(s);
    }
}

static void trig_cos(benchmark::State& state) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(-TAU, TAU);
    auto angle = radians(dist(mt));

    benchmark::DoNotOptimize(angle);
    for (auto _ : state) {
        auto s = std::cos(angle);
        benchmark::DoNotOptimize(s);
    }
}

// sine
BENCHMARK_TEMPLATE(trig_taylor_sine, negative_pi_pi_bounded,
                   negative_hpi_hpi_bounded);
BENCHMARK_TEMPLATE(trig_taylor_sine, negative_pi_pi_bounded,
                   not_negative_hpi_hpi_bounded);
BENCHMARK_TEMPLATE(trig_taylor_sine, not_negative_pi_pi_bounded,
                   not_negative_hpi_hpi_bounded);
BENCHMARK(trig_sin);

// cosine
BENCHMARK_TEMPLATE(trig_taylor_cosine, zero_tau_bounded, zero_pi_bounded);
BENCHMARK_TEMPLATE(trig_taylor_cosine, zero_tau_bounded, not_zero_pi_bounded);
BENCHMARK_TEMPLATE(trig_taylor_cosine, not_zero_tau_bounded,
                   not_zero_pi_bounded);
BENCHMARK(trig_cos);