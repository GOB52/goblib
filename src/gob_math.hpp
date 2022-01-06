/*!
  Goblin Library

  @file   gob_math.hpp
  @brief  Mathematics
*/
#pragma once
#ifndef GOBLIB_MATH_HPP
#define GOBLIB_MATH_HPP

#include <cmath>
#ifdef __GNUG__
#include <bits/c++config.h> // _GLIBCXX_USE_C99_MATH_TR1
#endif
#include <cstdint>
#include <type_traits> // std::enable_if
#include <limits> // std::numeric_limits::epsi;on
#include <algorithm> // std::min,max
#if defined(GOBLIB_CPP20_OR_LATER)
#include <numbers> // std::numbers
#endif
#include <cassert>
#include "gob_macro.hpp"

namespace goblib
{

/*!
  @brief Mathematics
*/
namespace math
{
/*! 
  constants
  @brief Constants for math
*/
namespace constants
{
#if defined(GOBLIB_CPP20_OR_LATER)
/*! PI */
template<typename T> constexpr auto pi = std::numbers::pi_v<T>;
constexpr float pi_f = pi<float>;
constexpr double pi_d = pi<double>;
constexpr long double pi_ld = pi<long double>;
/*! HalfPI */
template<typename T> constexpr auto half_pi = pi<T> * static_cast<T>(0.5f);
constexpr float half_pi_f = half_pi<float>;
constexpr double half_pi_d = half_pi<double>;
constexpr long double half_pi_ld = half_pi<long double>;
/*! 2PI */
template<typename T> constexpr auto pi2 = pi<T> * static_cast<T>(2.0f);
constexpr float pi2_f = pi2<float>;
constexpr double pi2_d = pi2<double>;
constexpr long double pi2_ld = pi2<long double>;

#elif defined(GOBLIB_CPP14_OR_LATER)
/*! PI */
template<typename T> constexpr T pi = std::acos(static_cast<T>(-1.0));
constexpr float pi_f = pi<float>;
constexpr double pi_d = pi<double>;
constexpr long double pi_ld = pi<long double>;
/*! HalfPI */
template<typename T> constexpr auto half_pi = pi<T> * static_cast<T>(0.5f);
constexpr float half_pi_f = half_pi<float>;
constexpr double half_pi_d = half_pi<double>;
constexpr long double half_pi_ld = half_pi<long double>;
/*! 2PI */
template<typename T> constexpr auto pi2 = pi<T> * static_cast<T>(2.0f);
constexpr float pi2_f = pi2<float>;
constexpr double pi2_d = pi2<double>;
constexpr long double pi2_ld = pi2<long double>;

#else // C++11

/*! PI */
template<typename T> constexpr T pi() { return std::acos(static_cast<T>(-1.0)); }
constexpr float pi_f = pi<float>();
constexpr double pi_d = pi<double>();
constexpr long double pi_ld = pi<long double>();
/*! HalfPI */
template<typename T> constexpr T half_pi() { return pi<T>() * static_cast<T>(0.5f); }
constexpr float half_pi_f = half_pi<float>();
constexpr double half_pi_d = half_pi<double>();
constexpr long double half_pi_ld = half_pi<long double>();
/* 2PI */
template<typename T> constexpr T pi2() { return pi<T>() * static_cast<T>(2.0f); }
constexpr float pi2_f = pi2<float>();
constexpr double pi2_d = pi2<double>();
constexpr long double pi2_ld = pi2<long double>();
#endif
//
}


/// @name Alternative function
/// @brief Some functions cannot be used depending on whether or not _GLIBCXX_USE_C99_MATH_TR is defeined.
/// @{
/*!
  @brief alternative of std::fmin
*/
template<typename T, typename std::enable_if<std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
GOBLIB_INLINE constexpr const T& fmin(const T& left, const T& right)
{
    return (std::islessequal(left, right) || std::isnan(right)) ? left : right;
}

/*!
  @brief alternative of std::fmax
*/
template<typename T, typename std::enable_if<std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
GOBLIB_INLINE constexpr const T& fmax(const T& left, const T& right)
{
    return (std::isgreaterequal(left, right) || std::isnan(right)) ? left : right;
}

/*!
  @brief alternative of std::round
*/
template<typename T> GOBLIB_INLINE constexpr auto round(const T v)
        -> typename std::enable_if<std::is_floating_point<T>::value, T>::type
{
#if defined(__GNUG__) && !defined(_GLIBCXX_USE_C99_MATH_TR1)
    return static_cast<std::int64_t>(v + (v >= static_cast<T>(0) ? static_cast<T>(0.5f) : static_cast<T>(-0.5f)) );
#else
    return std::round(v);
#endif
}

/*!
  @brief alternative of std::floor
  @note Can constexpr
*/
template<typename T> GOBLIB_INLINE constexpr auto floor(const T v)
        -> typename std::enable_if<std::is_floating_point<T>::value, T>::type
{
    return (v >= static_cast<T>(0)) ? static_cast<std::int64_t>(v) :
            (v == static_cast<T>(static_cast<std::int64_t>(v))) ? v :
            static_cast<std::int64_t>(v) - 1;
}

/*!
  @brief alternative of std::ceil
  @note Can constexpr
*/
template<typename T> GOBLIB_INLINE constexpr auto ceil(const T v)
        -> typename std::enable_if<std::is_floating_point<T>::value, T>::type
{
    return (static_cast<T>(static_cast<std::int64_t>(v)) == v)
            ? static_cast<std::int64_t>(v)
            : static_cast<std::int64_t>(v) + ((v > 0) ? 1 : 0);
}

/*!
  @brief alternative of std::fabs for float
*/
template<typename T> GOBLIB_INLINE constexpr auto fabs(const T v)
        -> typename std::enable_if<std::is_same<T, float>::value, T>::type
{
#if defined(__GNUG__) && !defined(_GLIBCXX_USE_C99_MATH_TR1)
    return ::fabsf(v);
#else
    return std::fabs(v);
#endif
}

/*!
  @brief alternative of std::fabs for double
*/
template<typename T> GOBLIB_INLINE constexpr auto fabs(const T v)
        -> typename std::enable_if<std::is_same<T, double>::value, T>::type
{
#if defined(__GNUG__) && !defined(_GLIBCXX_USE_C99_MATH_TR1)
    return ::fabs(v);
#else
    return std::fabs(v);
#endif
}

/*!
  @brief alternative of std::fabs for long double
*/
template<typename T> GOBLIB_INLINE constexpr auto fabs(const T v)
        -> typename std::enable_if<std::is_same<T, long double>::value, T>::type
{
#if defined(__GNUG__) && !defined(_GLIBCXX_USE_C99_MATH_TR1)
    return ::fabsl(v);
#else
    return std::fabs(v);
#endif
}

/*!
  @brief alternative of std::abs for integral and return Integral
  @warning return double if using std::abs(Integral). [double std::abs(Integral x);]
*/
template<typename T> GOBLIB_INLINE constexpr auto abs(const T v)
        -> typename std::enable_if<std::is_integral<T>::value, T>::type
{
    return (v < T(0)) ? -v : v;
}

/*!
  @brief alternative of std::abs for floating point number.
*/
template<typename T> GOBLIB_INLINE constexpr auto abs(const T v)
        -> typename std::enable_if<std::is_floating_point<T>::value, T>::type
{
    return goblib::math::fabs(v);
}
/// @}


/// @name Compare
/// @{
/*! @brief equal for arithmetic type */
template<typename T, typename std::enable_if< std::is_integral<T>::value, std::nullptr_t>::type = nullptr>
GOBLIB_INLINE constexpr bool equal(const T x, const T y) { return x == y; }

template<typename T, typename std::enable_if< std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
GOBLIB_INLINE constexpr bool equal(const T x, const T y)
{
    return goblib::math::fabs(x - y) < std::numeric_limits<T>::epsilon();
}
/*! @brief equal with tolerance 
  @param x Value
  @param y Value
  @param tolerance Tolerance value
*/
template<typename T, typename std::enable_if< std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
GOBLIB_INLINE constexpr bool equal_tolerance(const T x, const T y, const T tolerance = static_cast<T>(0.00001f))
{
    return goblib::math::fabs(x - y) < tolerance;
}

/*! @brief equal with tolerance for integral
  @param x Value
  @param y Value
  @param tolerance Tolerance value(Ignored)
*/
template<typename T, typename std::enable_if< std::is_integral<T>::value, std::nullptr_t>::type = nullptr>
GOBLIB_INLINE constexpr bool equal_tolerance(const T x, const T y, const T tolerance = 0)
{
    return x == y; // ignore tolerance (T is integral)
}

/*! @brief Not equal for arithmetic type */
template<typename T, typename std::enable_if< std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr>
GOBLIB_INLINE constexpr bool not_equal(const T x, const T y) { return !(x == y); }
/// @}


/// @name Radian, Degree
// @{
/*!
  Wrapping degree [0, +360)
  @tparam T floating point number type
  @param deg degree
  @param wrapped value that between 0 and less than 360.
*/
template<typename T, typename std::enable_if< std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
GOBLIB_INLINE constexpr T wrapDeg360(const T deg)
{
    return deg - static_cast<T>(360.f) * goblib::math::floor( deg * (static_cast<T>(1.0f) / static_cast<T>(360.f)) );
}

/*!
  Wrapping degree [-180, +180)
  @tparam T floating point number type
  @param deg degree
  @return wrapped value that between -180 and less than +180.
*/
template<typename T, typename std::enable_if< std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
GOBLIB_INLINE constexpr T wrapDeg180(const T deg)
{
    return deg - static_cast<T>(360.f) * goblib::math::floor( (deg + static_cast<T>(180.f)) * (static_cast<T>(1.0f) / static_cast<T>(360.f)) );
}

/*!
  Wrapping radian [-PI ~ PI)
  @tparam T floating point number type
  @param deg degree
  @return wrapped value that between -180 and less than +180.
*/
template<typename T, typename std::enable_if< std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
GOBLIB_INLINE constexpr T wrapRad(const T rad)
{
#if defined(GOBLIB_CPP14_OR_LATER)
    return rad - constants::pi2<T> * goblib::math::floor( (rad + constants::pi<T> ) / constants::pi2<T>);
#else
    return rad - constants::pi2<T>() * goblib::math::floor( (rad + constants::pi<T>() ) / constants::pi2<T>());
#endif
}

/*! 
  degree to radian.
  @tparam T floating point number type
  @param deg degree.
  @return radian.
*/
template<typename T, typename std::enable_if< std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
GOBLIB_INLINE constexpr T deg2rad(const T deg)
{
#if defined(GOBLIB_CPP14_OR_LATER)
    return deg * constants::pi<T> / static_cast<T>(180.0f);
#else
    return deg * constants::pi<T>() / static_cast<T>(180.0f);
#endif
}

/*!
  radian to degree.
  @tparam T floating point number type
  @param rad radian
  @return degree
*/
template<typename T, typename std::enable_if< std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
GOBLIB_INLINE constexpr T rad2deg(const T rad)
{
#if defined(GOBLIB_CPP14_OR_LATER)
    return rad * static_cast<T>(180.0f) / constants::pi<T>;
#else
    return rad * static_cast<T>(180.0f) / constants::pi<T>();
#endif
}
// @}

/// @name Normalize
/// @{
/*!  @brief Normalize between 0 and 1 */
template<typename T> constexpr GOBLIB_INLINE auto normalize01(const T& val, const T& min_v, const T& max_v) ->
        typename std::common_type<T,float>::type
{
    using ctype = typename std::common_type<T,float>::type;
    return (static_cast<ctype>(val) - static_cast<ctype>(min_v)) / (static_cast<ctype>(max_v) - static_cast<ctype>(min_v));
}

/*!  @brief Normalize between -1 and 1 */
template<typename T> constexpr GOBLIB_INLINE auto normalize11(const T& val, const T& min_v, const T& max_v) ->
        typename std::common_type<T,float>::type
{
    using ctype = typename std::common_type<T,float>::type;
    return (static_cast<ctype>(val) - ((static_cast<ctype>(min_v) + static_cast<ctype>(max_v)) * 0.5f)) /
            ((static_cast<ctype>(max_v) - static_cast<ctype>(min_v)) * 0.5f);
}
///@}

/*! 
  @brief Is value power of 2?
  @param v integer value
  @retval true Power of 2.
  @retval false Not power of 2.
*/
template<typename T> constexpr GOBLIB_INLINE bool is_powerof2(const T& v)
{
    static_assert(std::is_integral<T>::value, "The argument v is only an integer value.");
    return v > 0 && ((v & (v - 1)) == 0);
}

/*! 
  @brief Is value positive, negative, or zero?
  @retval T(1) value is positive.
  @retval T(-1) value is negative.
  @retval T(0) value is zero.
 */
template<typename T, typename std::enable_if< std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr>
constexpr GOBLIB_INLINE T sign(const T& v)
{
    return static_cast<T>((T(0) < v) - (v < T(0)));
}
//
}}
#endif
