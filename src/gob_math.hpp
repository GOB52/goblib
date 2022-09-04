/*!
  Goblin Library

  @file   gob_math.hpp
  @brief  Mathematics
*/
#pragma once
#ifndef GOBLIB_MATH_HPP
#define GOBLIB_MATH_HPP

#include <gob_macro.hpp>
#include <cmath>

#if defined(GOBLIB_COMPILER_GCC)
#include <bits/c++config.h> // _GLIBCXX_USE_C99_MATH_TR1
#endif

#include <cstdint>
#include <type_traits> // std::enable_if, common_type
#include <limits> // std::numeric_limits::epsilon
#include <climits> // CHAR_BIT
#include <algorithm> // std::min,max
#if defined(GOBLIB_CPP20_OR_LATER)
#include <numbers> // std::numbers
#endif
#include <cassert>
#include "gob_macro.hpp"
#include "gob_debug.hpp"

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
#  if defined(GOBLIB_COMPILER_GCC)
template<typename T> constexpr T pi = std::acos(static_cast<T>(-1.0));
#  else
template<typename T> constexpr T pi = static_cast<T>(M_PI);
#  endif
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
#  if defined(GOBLIB_COMPILER_GCC)
template<typename T> constexpr T pi() { return std::acos(static_cast<T>(-1.0)); }
#  else
template<typename T> constexpr T pi() { return static_cast<T>(M_PI); }
#  endif
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
  @note Can be constexpr when compiled with GCC.
*/
template<typename T, typename std::enable_if<std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
GOBLIB_INLINE GOBLIB_CONSTEXPR_GCC const T& fmin(const T& left, const T& right)
{
    return (std::islessequal(left, right) || std::isnan(right)) ? left : right;
}

/*!
  @brief alternative of std::fmax
  @note Can be constexpr when compiled with GCC.
*/
template<typename T, typename std::enable_if<std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
GOBLIB_INLINE GOBLIB_CONSTEXPR_GCC const T& fmax(const T& left, const T& right)
{
    return (std::isgreaterequal(left, right) || std::isnan(right)) ? left : right;
}

/*!
  @brief alternative of std::round
  @note Can be constexpr
*/
template<typename T> GOBLIB_INLINE constexpr auto round(const T v)
        -> typename std::enable_if<std::is_floating_point<T>::value, T>::type
{
#if defined(GOBLIB_COMPILER_GCC) && !defined(_GLIBCXX_USE_C99_MATH_TR1)
    return std::isnan(v) ? v : ::round(v); // using GCC builtin
#else
    return (v != v) ? v // unordered (may be v is NaN)
            : (v == std::numeric_limits<T>::infinity()) ? v
            : (v == -std::numeric_limits<T>::infinity()) ? v
            : static_cast<std::int64_t>(v + (v >= static_cast<T>(0) ? static_cast<T>(0.5f) : static_cast<T>(-0.5f)) );
#endif
}

/*!
  @brief alternative of std::floor
  @note Can be constexpr
*/
template<typename T> GOBLIB_INLINE constexpr auto floor(const T v)
        -> typename std::enable_if<std::is_floating_point<T>::value, T>::type
{
    return (v != v) ? v // unordered (may be v is NaN)
            : (v == std::numeric_limits<T>::infinity()) ? v
            : (v == -std::numeric_limits<T>::infinity()) ? v
            : (v >= static_cast<T>(0)) ? static_cast<std::int64_t>(v)
            : (v == static_cast<T>(static_cast<std::int64_t>(v))) ? v
            : static_cast<std::int64_t>(v) - 1;
}

/*!
  @brief alternative of std::ceil
  @note Can be constexpr
*/
template<typename T> GOBLIB_INLINE constexpr auto ceil(const T v)
        -> typename std::enable_if<std::is_floating_point<T>::value, T>::type
{
    return (v != v) ? v // unordered (may be v is NaN)
            : (v == std::numeric_limits<T>::infinity()) ? v
            : (v == -std::numeric_limits<T>::infinity()) ? v
            : (static_cast<T>(static_cast<std::int64_t>(v)) == v) ? static_cast<std::int64_t>(v)
            : static_cast<std::int64_t>(v) + ((v > 0) ? 1 : 0);
}

/*!
  @brief alternative of std::fabs for float
  @note Can be constexpr
*/
template<typename T> GOBLIB_INLINE constexpr auto fabs(const T v)
        -> typename std::enable_if<std::is_same<T, float>::value, T>::type
{
#if defined(GOBLIB_COMPILER_GCC) && !defined(_GLIBCXX_USE_C99_MATH_TR1)
    return ::fabsf(v); // using GCC bultin
#else
    return v < 0.0f ? -v : v;
#endif
}

/*!
  @brief alternative of std::fabs for double
  @note Can be constexpr
*/
template<typename T> GOBLIB_INLINE constexpr auto fabs(const T v)
        -> typename std::enable_if<std::is_same<T, double>::value, T>::type
{
#if defined(GOBLIB_COMPILER_GCC) && !defined(_GLIBCXX_USE_C99_MATH_TR1)
    return ::fabs(v); // using GCC bultin
#else
    return v < 0.0 ? -v : v;
#endif
}

/*!
  @brief alternative of std::fabs for long double
  @note Can be constexpr
*/
template<typename T> GOBLIB_INLINE constexpr auto fabs(const T v)
        -> typename std::enable_if<std::is_same<T, long double>::value, T>::type
{
#if defined(GOBLIB_COMPILER_GCC) && !defined(_GLIBCXX_USE_C99_MATH_TR1)
    return ::fabsl(v); // using GCC bultin
#else
    return v < 0.0 ? -v : v;
#endif
}

/*!
  @brief alternative of std::abs for integral and return Integral
  @warning return double if using std::abs(Integral). [double std::abs(Integral x);]
  @note Can be constexpr
*/
template<typename T, typename std::enable_if< std::is_integral<T>::value, std::nullptr_t>::type = nullptr>
GOBLIB_INLINE constexpr auto abs(const T v)
        -> typename std::enable_if<std::is_integral<T>::value, typename std::make_unsigned<T>::type>::type
{
    //    return (v < T(0)) ? -v : v;
    return v < 0 ? -static_cast<uintmax_t>(v) : v;

}

/*!
  @brief alternative of std::abs for floating point number.
  @note Can be constexpr
*/
template<typename T> GOBLIB_INLINE constexpr auto abs(const T v)
        -> typename std::enable_if<std::is_floating_point<T>::value, T>::type
{
    return goblib::math::fabs(v);
}
/// @}


/// @name Compare
/// @{
/*! @brief equal for Integral type */
template<typename T, typename std::enable_if< std::is_integral<T>::value, std::nullptr_t>::type = nullptr>
GOBLIB_INLINE constexpr bool equal(const T x, const T y) { return x == y; }

/*! @brief equal for floating point type */
template<typename T, typename std::enable_if< std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
GOBLIB_INLINE constexpr bool equal(const T x, const T y)
{
    //In comparison operations, positive infinity is larger than all values except itself and NaN, and negative infinity is smaller than all values except itself and NaN. 
    return (!(x < y) != (x >= y)) ? false // unordered (may be x or y is NaN)
            : (std::numeric_limits<T>::infinity() == x && std::numeric_limits<T>::infinity() == y) ? true
            : (-std::numeric_limits<T>::infinity() == x && -std::numeric_limits<T>::infinity() == y) ? true
            : goblib::math::fabs(x - y) < std::numeric_limits<T>::epsilon();
}

/*! @brief Not equal for arithmetic type */
template<typename T, typename std::enable_if< std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr>
GOBLIB_INLINE constexpr bool not_equal(const T x, const T y) { return !(x == y); }

/*! @brief equal with tolerance for floating point number
  @param x Value
  @param y Value
  @param tolerance Tolerance value
*/
template<typename T, typename std::enable_if< std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
GOBLIB_INLINE constexpr bool equal_tolerance(const T x, const T y, const T tolerance = static_cast<T>(0.00001f))
{
    return (!(x < y) != (x >= y)) || (tolerance != tolerance) ? false // unordered (may be x or y or tolelance is NaN)
            : (std::numeric_limits<T>::infinity() == x && std::numeric_limits<T>::infinity() == y) ? true
            : (-std::numeric_limits<T>::infinity() == x && -std::numeric_limits<T>::infinity() == y) ? true
            : (std::numeric_limits<T>::infinity() == tolerance) ? true
            : goblib::math::fabs(x - y) <= tolerance;
}

/*! @brief not equal with tolerance for floating point number */
template<typename T, typename std::enable_if< std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
GOBLIB_INLINE constexpr bool not_equal_tolerance(const T x, const T y, const T tolerance = static_cast<T>(0.00001f))
{
    return !goblib::math::equal_tolerance(x, y, tolerance);
}
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
template<typename T, typename std::enable_if< std::is_floating_point<T>::value, std::nullptr_t>::type = nullptr>
constexpr GOBLIB_INLINE auto normalize01(const T val, const T min_v, const T max_v) ->
        typename std::common_type<T,float>::type
{
    using ctype = typename std::common_type<T,float>::type;
    return  GOBLIB_ASSERT(val >= min_v),
            GOBLIB_ASSERT(val <= max_v),
            (val != val) || !((min_v < max_v) != (min_v >= max_v)) ? std::numeric_limits<T>::quiet_NaN() // unordered (may be min_v or max_v or val is NaN)
            : (max_v - min_v) == -std::numeric_limits<T>::infinity() || (max_v - min_v) == std::numeric_limits<T>::infinity() ? std::numeric_limits<T>::quiet_NaN()
            : (static_cast<ctype>(val) - static_cast<ctype>(min_v)) / (static_cast<ctype>(max_v) - static_cast<ctype>(min_v));
}

template<typename T, typename std::enable_if< std::is_integral<T>::value, std::nullptr_t>::type = nullptr>
constexpr GOBLIB_INLINE auto normalize01(const T val, const T min_v, const T max_v) ->
        typename std::common_type<T,float>::type
{
    using ctype = typename std::common_type<T,float>::type;
    static_assert(std::is_floating_point<ctype>::value, "Invalid ctype");
    return  GOBLIB_ASSERT(val >= min_v),
            GOBLIB_ASSERT(val <= max_v),
            (static_cast<ctype>(val) - static_cast<ctype>(min_v)) / (static_cast<ctype>(max_v) - static_cast<ctype>(min_v));
}

/*!  @brief Normalize between -1 and 1 */
template<typename T> constexpr GOBLIB_INLINE auto normalize11(const T val, const T min_v, const T max_v) ->
        typename std::common_type<T,float>::type
{
    using ctype = typename std::common_type<T,float>::type;
    return  GOBLIB_ASSERT(val >= min_v),
            GOBLIB_ASSERT(val <= max_v),
            (static_cast<ctype>(val) - ((static_cast<ctype>(min_v) + static_cast<ctype>(max_v)) * 0.5f)) /
            ((static_cast<ctype>(max_v) - static_cast<ctype>(min_v)) * 0.5f);
}
///@}

/*! 
  @brief Is value power of 2?
  @param v integer value
  @retval true Power of 2.
  @retval false Not power of 2.
*/
template<typename T> constexpr GOBLIB_INLINE bool is_powerof2(const T v)
{
    static_assert(std::is_integral<T>::value, "The argument v is only an integer value.");
    return v > 0 && ((v & (v - 1)) == 0);
}

/*! 
  @brief Is value positive, negative, or zero?
  @retval T(1) value is positive.
  @retval T(-1) value is negative.
  @retval T(0) value is zero or valiable is NaN.
 */
template<typename T, typename std::enable_if< std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr>
constexpr GOBLIB_INLINE T sign(const T v)
{
    return (v != v) ? 0 // unordered (may be v is NaN)
            : static_cast<T>((T(0) < v) - (v < T(0)));
}

/// @cond
template <typename U, typename std::enable_if<std::is_integral<U>::value && std::is_unsigned<U>::value, std::nullptr_t>::type = nullptr>
constexpr U roundUpPow2_(const U v, const unsigned i = 1)
{ return (i < sizeof(U) * CHAR_BIT) ? roundUpPow2_(U(v | (v >> i)), i << 1) : v; }

template <typename T, typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value, std::nullptr_t>::type = nullptr>
constexpr auto roundUpPow2_(const T v) -> typename std::make_unsigned<T>::type
{
    using UType = typename std::make_unsigned<T>::type;
    return roundUpPow2_(static_cast<UType>(v <= 0 ? 0 : v) );
}
/// @endcond

/*! @brief roundup to the power of 2 */
template <typename T> constexpr auto roundUpPow2(const T v) -> typename std::make_unsigned<T>::type
{
    static_assert(std::is_integral<T>::value, "Value must be integral.");
    using UType = typename std::make_unsigned<T>::type;
    return roundUpPow2_(static_cast<UType>(v - 1)) + 1;
}

/*! @brief roundup to the next power of 2 */
template <typename T> constexpr auto nextPow2(const T v) -> typename std::make_unsigned<T>::type
{
    static_assert(std::is_integral<T>::value, "Value must be integral.");
    using UType = typename std::make_unsigned<T>::type;
    return roundUpPow2_(static_cast<UType>(v)) + 1;
}

//
}}
#endif
