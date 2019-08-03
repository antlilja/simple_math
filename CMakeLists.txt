# Copyright(c) 2019-present, Anton Lilja.
# Distributed under the MIT License (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.2)

project(
	simple_math
	VERSION 1.0.0
    LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

# Options
option(SIMPLE_MATH_BUILD_TESTS "Build tests" FALSE)
option(SIMPLE_MATH_BUILD_BENCHMARKS "Build benchmarks" FALSE)

option(SIMPLE_MATH_FORCE_ALIGNMENT "Force alignment on all types" FALSE)
set(SIMPLE_MATH_SIMD_LEVEL "AVX" CACHE STRING "SIMD levels: NONE, SSE2, SSE4, AVX") 

# Targets
add_library(simple_math INTERFACE)
add_library(simple::math ALIAS simple_math)

# Target configuration
target_include_directories(simple_math INTERFACE include/)

target_compile_features(simple_math 
	INTERFACE
		cxx_std_17
)

target_compile_options(simple_math
	INTERFACE
		$<$<OR:$<OR:$<AND:$<CXX_COMPILER_ID:Clang>,$<NOT:$<STREQUAL:"x${CMAKE_CXX_SIMULATE_ID}","xMSVC">>>,$<CXX_COMPILER_ID:GNU>>,$<CXX_COMPILER_ID:AppleClang>>:
			$<$<NOT:$<STREQUAL:"${SIMPLE_MATH_SIMD_LEVEL}","NONE">>:
				$<$<STREQUAL:"${SIMPLE_MATH_SIMD_LEVEL}","SSE4">:
					-msse4.1
				>
				$<$<STREQUAL:"${SIMPLE_MATH_SIMD_LEVEL}","AVX">:
					-mavx2
				>
			>
        >
)

target_compile_definitions(simple_math 
	INTERFACE
		$<$<BOOL:${SIMPLE_MATH_FORCE_ALIGNMENT}>:SIMPLE_MATH_FORCE_ALIGNMENT>
		
		$<$<NOT:$<STREQUAL:"${SIMPLE_MATH_SIMD_LEVEL}","NONE">>:
			$<$<STREQUAL:"${SIMPLE_MATH_SIMD_LEVEL}","SSE2">:
				SIMPLE_MATH_SSE2
			>
			$<$<STREQUAL:"${SIMPLE_MATH_SIMD_LEVEL}","SSE4">:
				SIMPLE_MATH_SSE2
				SIMPLE_MATH_SSE4
			>
			$<$<STREQUAL:"${SIMPLE_MATH_SIMD_LEVEL}","AVX">:
				SIMPLE_MATH_SSE2
				SIMPLE_MATH_SSE4
				SIMPLE_MATH_AVX
			>
		>
		
)

# Testing
if(SIMPLE_MATH_BUILD_TESTS)
	include(CTest)
	enable_testing()
	add_subdirectory(tests)
endif()

# Benchmarking
if(SIMPLE_MATH_BUILD_BENCHMARKS)
	add_subdirectory(bench)
endif()