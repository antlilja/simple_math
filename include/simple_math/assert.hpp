#pragma once
namespace sm {
#ifdef SIMPLE_MATH_ENABLE_ASSERTS
#    include <exception>
#    if defined(_WIN32)
#        define SIMPLE_MATH_BREAKPOINT __debugbreak();
#    elif defined(__linux__) || defined(__APPLE__)
#        define SIMPLE_MATH_BREAKPOINT asm("int $3");
#    endif

#    define SIMPLE_MATH_ASSERT(x)  \
        if (!(x)) {                \
            SIMPLE_MATH_BREAKPOINT \
            std::terminate();      \
        }

#else
#    define SIMPLE_MATH_ASSERT(x)
#endif
} // namespace sm