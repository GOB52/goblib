/*!
  Goblin Library

  @file   gob_utility.hpp
  @brief  Utilities.
*/
#pragma once
#ifndef GOBLIB_UTILITY_HPP
#define GOBLIB_UTILITY_HPP

#include <cassert>
#include <type_traits> // std::underlying_type, std::is_xxx
#include <bitset> // std::bitset
#include <limits> // std::numeric_limits
#if defined(GOBLIB_CPP17_OR_LATER)
#include <algorithm> // std::clamp
#endif
#if defined(GOBLIB_CPP14_OR_LATER)
#include <functional> // std::less
#endif

#include "gob_math.hpp" // goblib::math::fmin, fmax
#include "gob_macro.hpp"

namespace goblib
{
/// @name Delete object
/// @{

/*!
  @brief Delete object and set nullptr.
  @tparam T any class.
*/
template<typename T> GOBLIB_INLINE void safeDelete(T* &ptr)
{
    typedef char check_completed_type[sizeof(T) ? 1 : -1];
    (void)sizeof(check_completed_type);
    delete ptr;
    ptr = nullptr;
}

/*!
  @brief Delete array of object and set nullptr.
  @tparam T any class.
*/
template<typename T> GOBLIB_INLINE void safeDeleteArray(T* &ptr)
{
    typedef char check_completed_type[sizeof(T) ? 1 : -1];
    (void)sizeof(check_completed_type);
    delete[] ptr;
    ptr = nullptr;
}
/// @}

/*! 
  @brief Constrain value by range.(like std::clamp C++17 or later)
  @tparam value type
  @param v value
  @param low constraint value (low)
  @param high constraint value (high)
  @return constrainted value
  @note It can constexpr C++14 or later.
*/
#if defined(GOBLIB_CPP17_OR_LATER)
template<typename T> GOBLIB_INLINE constexpr const T& clamp(const T& v, const T& low, const T& high)
{
    return std::clamp(v, low, high);
}
#elif defined(GOBLIB_CPP14_OR_LATER)
template<typename T, typename std::enable_if<std::is_integral<T>::value, std::nullptr_t>::type = nullptr>
GOBLIB_INLINE constexpr const T& clamp(const T& v, const T& low, const T& high)
{
    assert(low <= high &&  "low must be lesser than high.");
    return std::min( std::max(v, low), high);
}
template<typename T, typename std::enable_if<std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
GOBLIB_INLINE constexpr const T& clamp(const T& v, const T& low, const T& high)
{
    // Don't check low <= high bacause allow input of NaN.
    return goblib::math::fmin( goblib::math::fmax(v, low), high);
}
#else
template<typename T, typename std::enable_if<std::is_integral<T>::value, std::nullptr_t>::type = nullptr>
GOBLIB_INLINE const T& clamp(const T& v, const T& low, const T& high)
{
    assert(low <= high &&  "low must be lesser than high.");
    return std::min( std::max(v, low), high);
}
template<typename T, typename std::enable_if<std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
GOBLIB_INLINE const T& clamp(const T& v, const T& low, const T& high)
{
    // Don't check low <= high bacause allow input of NaN.
    return goblib::math::fmin( goblib::math::fmax(v, low), high);
}
#endif

/// @name Alternative function
/// @{
/*! @brief Like std::size  C++17 or later.(for container) */
template <class C> constexpr auto size(const C& c) -> decltype(c.size()) { return c.size(); }
/*! @brief Like std::size C++17 or later.(for array) */
template<typename T, size_t N> constexpr auto size(const T(&)[N]) noexcept -> size_t { return N; }
/// @}

/*! 
  @brief Converts an enumeration to its underlying type.(Like std::to_underlying C++23 or later)
  @tparam E Type of enum
  @param e Enumeration value to convert
  @return The integer value of the underlying type of Enum, converted from e.
*/
template<typename E> constexpr GOBLIB_INLINE typename std::underlying_type<E>::type to_underlying(const E& e) noexcept
{
    return static_cast<typename std::underlying_type<E>::type>(e);
}

/*! 
  @brief Round trip counter.
  @tparam W round trip range(std::int32_t)
  @param c counter value.
  @return 0,1... ~ (W-1),(W-1),(W-2)... ~ 1,0,0,1 ~ ...
  @attention W must be power of 2.
  @attention The maximum value of the counter type must be greater than or equal to W.
*/
template <std::int32_t W, typename T> GOBLIB_INLINE T roundTrip(const T& c)
{
    static_assert(goblib::math::is_powerof2(W), "W must be power of 2.");
    static_assert(std::is_integral<T>::value, "The argument c is only an integer value.");
    static_assert(std::numeric_limits<T>::max() >= W, "The maximum value of the counter type must be greater than or equal to W.");

    return ((c & W) + ((c & (W-1)) ^ (0 - ((c >> (std::bitset<32>(W-1).count())) & 1))));
}

template <typename T> GOBLIB_INLINE T roundTrip(const T& c, const T& W)
{
    assert(goblib::math::is_powerof2(W) && "W must be power of 2.");
    static_assert(std::is_integral<T>::value, "The argument c is only an integer value.");
    assert(std::numeric_limits<T>::max() >= W &&  "The maximum value of the counter type must be greater than or equal to W.");

    return ((c & W) + ((c & (W-1)) ^ (0 - ((c >> (std::bitset<32>(W-1).count())) & 1))));
}

//
}
#endif
