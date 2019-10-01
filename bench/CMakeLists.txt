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

    trig.cpp

    quaternion.cpp
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
        benchmark::benchmark 
        benchmark::benchmark_main
)