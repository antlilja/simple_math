# Copyright(c) 2019-present, Anton Lilja.
# Distributed under the MIT License (http://opensource.org/licenses/MIT)

find_package(benchmark CONFIG REQUIRED)

set(BENCHMARK_ENABLE_LTO true)

add_executable(simple_math_bench
    main.cpp

    vec2.cpp
    vec3.cpp
    vec4.cpp

    mat3.cpp
    mat4.cpp
)

target_link_libraries(simple_math_bench
    PRIVATE
        simple::math
        benchmark::benchmark 
        benchmark::benchmark_main
)