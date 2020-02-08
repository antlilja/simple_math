# Copyright(c) 2019-present, Anton Lilja.
# Distributed under the MIT License (http://opensource.org/licenses/MIT)

set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "")
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "")

include(FetchContent)
FetchContent_Declare(benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.5.0
)

FetchContent_MakeAvailable(benchmark)

add_executable(simple_math_bench
    main.cpp

    vec2.cpp
    vec3.cpp
    vec4.cpp

    mat3.cpp
    mat4.cpp

    trig.cpp
)

target_compile_options(simple_math_bench
    PRIVATE
        $<$<OR:$<AND:$<CXX_COMPILER_ID:Clang>,$<NOT:$<STREQUAL:"x${CMAKE_CXX_SIMULATE_ID}","xMSVC">>>,$<CXX_COMPILER_ID:GNU>>:
			-O2
        >
        $<$<OR:$<CXX_COMPILER_ID:MSVC>,$<AND:$<STREQUAL:"x${CMAKE_CXX_SIMULATE_ID}","xMSVC">,$<STREQUAL:"${CMAKE_CXX_COMPILER_ID}","Clang">>>:
            /O2
		>
)

target_link_libraries(simple_math_bench
    PRIVATE
        simple::math
        benchmark 
)