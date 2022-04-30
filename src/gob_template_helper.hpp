/*!
  Goblin Library

  @file  gob_template_helper.hpp
  @brief utilities for template.
*/
#pragma once
#ifndef GOBLIB_TEMPLATE_HELPER_HPP
#define GOBLIB_TEMPLATE_HELPER_HPP

#include <type_traits>
#include <utility>
#include <array>
#include "gob_macro.hpp"

namespace goblib
{

/*!
  @brief utilities for template.
*/
namespace template_helper
{
// Like std::void_t
#if defined(GOBLIB_CPP14_OR_EARLIER)
/// @cond
template <class... Types>  struct Tester { using type = void;  };
/// @endcond
/*! @brief Like std::void_t (C++17) */
template <class... Types> using void_t = typename Tester<Types...>::type;
/*! @brief detection */ 
template<class, template<class> class, class = void_t<>> struct detect : std::false_type {};
/*! @brief detection */ 
template<class T, template<class> class Check> struct detect<T, Check, void_t<Check<T>>> : std::true_type{};
#else
template<class, template<class> class, class = std::void_t<>> struct detect : std::false_type {};
template<class T, template<class> class Check> struct detect<T, Check, std::void_t<Check<T>>> : std::true_type{};
#endif

/// @cond
template<class T, class T2>
struct can_determine_less_impl
{
    template<class U, class V> static auto test(U*) -> decltype(std::declval<U>() < std::declval<V>());
    template<typename, typename> static auto test(...) -> std::false_type;
    using type = typename std::is_same<bool, decltype(test<T, T2>(0))>::type;
};
/// @endcond
/*!
  @brief Can determine that T is less than T2 ?
  @tparam T type of left.
  @tparam T2 type of right. (T as default)
  @remark Check T has operator<(T2) or exists operator<(T1,T2)
*/
template<class T, class T2 = T> struct can_determine_less : can_determine_less_impl<T, T2>::type {};

/// @cond
template<class T, class T2>
struct can_determine_equal_impl
{
    template<class U, class V> static auto test(U*) -> decltype(std::declval<U>() == std::declval<V>());
    template<typename, typename> static auto test(...) -> std::false_type;
    using type = typename std::is_same<bool, decltype(test<T, T2>(0))>::type;
};
/// @endcond
/*!
  @brief Can determine that T equal T2 ?
  @tparam T type of left.
  @tparam T2 type of right. (T as default)
  @remark Check T has operator==(T2) or exists operator==(T1,T2)
*/
template<class T, class T2 = T> struct can_determine_equal : can_determine_equal_impl<T, T2>::type {};


/// @cond
template <class... Args> struct is_callable_impl
{
    template <class Func> static std::true_type check(decltype(std::declval<Func>()(std::declval<Args>()...), (void)0)*);
    template <class Func> static std::false_type check(...);
};
/// @endcond
/*!
  @brief Determine if a function can be called with a specific argument.
  @tparam F Function
  @tparam Args Arguments of function
  @sa https://cpprefjp.github.io/lang/cpp11/sfinae_expressions.html
*/
template <class Func, class... Args> struct is_callable : decltype(is_callable_impl<Args...>::template check<Func>(nullptr)) {};

/*!
  @brief Check function return type.
  @tparam Expected return type
  @tparam Func Function
  @tparam Args Arguments of function
*/
template<typename Expected, typename Func, typename... Args>
#if defined(GOBLIB_CPP17_OR_LATER)
// Deprecated std::result_of C++17 , Deleted std::result_of C++20
using is_return_type = std::is_same<Expected, typename std::invoke_result<Func, Args...>::type>;
#else
using is_return_type = std::is_same<Expected, typename std::result_of<Func(Args...)>::type>;
#endif

#if defined(GOBLIB_CPP17_OR_EARLIER)
/// @cond
template<class T> struct is_clock_impl
{
    template<class U> using has_rep_impl = typename U::rep;
    template<class U> using has_rep = detect<U, has_rep_impl>;

    template<class U> using has_period_impl = typename U::period;
    template<class U> using has_period = detect<U, has_period_impl>;

    template<class U> using has_duration_impl = typename U::duration;
    template<class U> using has_duration = detect<U, has_duration_impl>;

    template<class U> using has_time_point_impl = typename U::time_point;
    template<class U> using has_time_point = detect<U, has_time_point_impl>;

    template<typename U,
             typename std::enable_if<has_rep<U>::value, std::nullptr_t>::type = nullptr,
             typename std::enable_if<has_period<U>::value, std::nullptr_t>::type = nullptr,
             typename std::enable_if<has_duration<U>::value, std::nullptr_t>::type = nullptr,
             typename std::enable_if<has_time_point<U>::value, std::nullptr_t>::type = nullptr>
    static auto test(U*) ->
            decltype(&U::is_steady, &U::now, std::true_type{});
    template<typename> static auto test(...) -> std::false_type;

    using type = decltype( test<T>(0) );
};
/// @endcond

/*! @brief Check T satisfies the Clock requirements. (like is_clock C++20 or later)
  @tparam T check target type
*/
template<class T> struct is_clock : is_clock_impl<T>::type {};
#endif

/// @cond
template<class T> struct is_rng_impl
{
    template<class U> using has_result_type_impl = typename U::result_type;
    template<class U> using has_result_type = detect<U, has_result_type_impl>;

    template<class U> using has_seed_impl = decltype(std::declval<U>().seed());
    template<class U> using has_seed = detect<U, has_seed_impl>;
    
    template<class U,
             typename std::enable_if<has_result_type<U>::value, std::nullptr_t>::type = nullptr,
             typename std::enable_if<has_seed<U>::value, std::nullptr_t>::type = nullptr>
    static auto test(U*) ->
            decltype(&U::operator(), &U::discard, &U::min, &U::max, std::true_type{});
    template<typename> static auto test(...) -> std::false_type;
    using type = decltype( test<T>(0) );
};
/// @endcond
/*! @brief Check T satisfies the RNG(Random Number Generator class in std) requirements.
  @tparam T check target type
*/
template<class T> struct is_rng : is_rng_impl<T>::type {};


#if defined(GOBLIB_CPP11_OR_EARLIER) || defined(DOXYGEN_PROCESS)
/// @name Like std::index_sequence,etc (C++14 or later)
/// @{
template<typename T, T... I> struct integer_sequence
{
    using value_type = T;
    static constexpr std::size_t size() { return sizeof...(I); }
};
template<std::size_t... I> using index_sequence = integer_sequence<std::size_t, I...>;
template<typename T, std::size_t N, T... I> struct make_integer_sequence : make_integer_sequence<T, N-1, N-1, I...> {};
template<typename T, T... I> struct make_integer_sequence<T, 0, I...> : integer_sequence<T, I...> {};
template<std::size_t N> using make_index_sequence = make_integer_sequence<std::size_t, N>;
template<typename... T> using index_sequence_for = make_index_sequence<sizeof...(T)>;
/// @}

#else

template<typename T, T... I> using integer_sequence = std::integer_sequence<T, I...>;
template<std::size_t... I> using index_sequence = std::index_sequence<I...>;
template<typename T, std::size_t N> using make_integer_sequence =  std::make_integer_sequence<T, N>;
template<std::size_t N> using make_index_sequence = std::make_index_sequence<N>;
template<typename... T> using index_sequence_for = std::index_sequence_for<T...>;

#endif

#if defined(GOBLIB_CPP17_OR_EARLIER) || defined(DOXYGEN_PROCESS)
/*! @brief Like std::identity (C++20 or later) */
struct identity
{
    template<class T> constexpr T&& operator()(T&& t) const noexcept;
    //    using is_transparent = bool;
};
#else
using identity = std::identity;
#endif

/*! @brief table generator */
namespace table
{
/// @cond
template<class Fn, std::size_t... I>
constexpr auto generate_helper(Fn f, goblib::template_helper::index_sequence<I...>)
        -> std::array<decltype(f(std::size_t{})), sizeof...(I)>
{
        return {{ f(I)... }};
}
/// @endcond

/*! @brief Table generator on compile.
@code
// Example 32-division sin table.
constexpr float tsin(int x) { return std::sin(goblib::math::deg2rad(x*360.0f/32.0f)); }
constexpr auto sin_table = goblib::template_helper::table::generator<32>(tsin); // std::array<float,32>
@endcode
 */
template<std::size_t N, class Fn>
constexpr auto generator(Fn f) -> decltype(generate_helper(f, goblib::template_helper::make_index_sequence<N>{}))
{
    return generate_helper(f, goblib::template_helper::make_index_sequence<N>{});
}
//
}
//
}}
#endif
