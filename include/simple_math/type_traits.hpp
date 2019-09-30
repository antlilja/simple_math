// Copyright(c) 2019-present, Anton Lilja.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once
#include <functional>

namespace sm::detail {
    // Boiler plate for operator checking
    template <class T, class R, class... Args>
    std::is_convertible<std::invoke_result_t<T, Args...>, R>
    is_invokable_test(int);

    template <class T, class R, class... Args>
    std::false_type is_invokable_test(...);

    template <class T, class R, class... Args>
    using is_invokable = decltype(is_invokable_test<T, R, Args...>(0));

    template <class T, class R, class... Args>
    constexpr auto is_invokable_v = is_invokable<T, R, Args...>::value;

    // Addition
    template <class L, class R = L>
    using has_addition = is_invokable<std::plus<>, L, L, R>;
    template <class L, class R = L>
    constexpr auto has_addition_v = has_addition<L, R>::value;

    // Multiplication
    template <class L, class R = L>
    using has_multiplies = is_invokable<std::multiplies<>, L, L, R>;
    template <class L, class R = L>
    constexpr auto has_multiplies_v = has_multiplies<L, R>::value;
} // namespace sm::detail