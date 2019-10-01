# simple_math
A header-only C++ math library for games and rendering that conforms to the vulkan API.

## Features
- SIMD (SSE to AVX)
- Vector types 
     - [vec2](include/simple_math/vec2.hpp) 
     - [vec3](include/simple_math/vec3.hpp) 
     - [vec4](include/simple_math/vec4.hpp) SIMD
- Column major matrix types 
     - [mat3](include/simple_math/mat3.hpp)
     - [mat4](include/simple_math/mat4.hpp) SIMD
- [Color functions](include/simple_math/color.hpp)
- [Angle types for radians and degrees](include/simple_math/common.hpp)
- [Templated ostream operators for all types](include/simple_math/io.hpp)
- [Constexpr trigonometric functions (sine and cosine)](include/simple_math/utility.hpp)
     - Approximations implemented using taylor polynomials

## Todo
- Quaternions
- Constexpr tangent function

## Using
- CMake
- [Catch2](https://github.com/catchorg/Catch2) Required for tests (catch.hpp included in project)
- [Google benchmark](https://github.com/google/benchmark) Required for benchmarks (external)

## Examples
#### Vector 
```c++
#include <simple_math/simple_math.hpp>
#include <iostream>

int main() {
     sm::vec3 v0(1.0f);
     sm::vec3 v1(2.3f, 4.5f, 3.0f);

     // Addition operator
     auto v3 = v0 + v1;

     // Subtraction operator
     auto v3 = v0 - v1;
    
     // Streams
     std::cout << v3 << '\n';
}
```

#### Matrix
```c++
#include <simple_math/simple_math.hpp>
#include <iostream>

int main() {
     auto m = sm::mat4::rotation_z(sm::degrees(time * 90.0f)) * sm::mat4::translation(2.0f, 3.0f, 4.0f);
     auto v = sm::mat4::look_at(sm::v3(2.0f), sm::v3(0.0f), sm::v3(0.0f, 0.0f, 1.0f));
     auto p = sm::mat4::perspective(45.0f, 1920.0f / 1080.0f, 0.1f, 10.0f);

     // Multiplication operator
     auto mvp = p * v * m;

     std::cout << mvp << '\n';
}
```
