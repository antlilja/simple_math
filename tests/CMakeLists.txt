# Copyright(c) 2019-present, Anton Lilja.
# Distributed under the MIT License (http://opensource.org/licenses/MIT)

include(FetchContent)
FetchContent_Declare(catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v2.11.1
)

FetchContent_MakeAvailable(catch2)

add_executable(simple_math_tests
	main.cpp

	vec2.cpp
	vec3.cpp
	vec4.cpp

	mat3.cpp
	mat4.cpp

	trig.cpp
)

target_link_libraries(simple_math_tests
	PRIVATE
		simple::math
		Catch2::Catch2
)

target_compile_definitions(simple_math_tests
	PRIVATE
		SIMPLE_MATH_ENABLE_ASSERTS
)