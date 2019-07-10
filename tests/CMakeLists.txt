# Copyright(c) 2019-present, Anton Lilja.
# Distributed under the MIT License (http://opensource.org/licenses/MIT)

add_executable(simple_math_tests
	main.cpp

	vec2.cpp
	vec3.cpp
	vec4.cpp

	mat3.cpp
	mat4.cpp
)

target_link_libraries(simple_math_tests
	PRIVATE
		simple::math
)

include(Catch)
catch_discover_tests(simple_math_tests)