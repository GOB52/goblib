/*!
  Goblin Library

  @file   gob_fixed_point_number.hpp
  @brief  Fixed point number.
*/
#pragma once
#ifndef GOBLIB_FIXED_POINT_NUMBER_HPP
#define GOBLIB_FIXED_POINT_NUMBER_HPP

#include <cstdint>
#include <utility>
#include <limits>
#include <algorithm>
#include <type_traits>
#include <cassert>
#include "gob_math.hpp"
#include "gob_macro.hpp"

namespace goblib
{

/*!
  @brief Fixed-point number
  @tparam BT Type of base value.
  @tparam Fraction Number of bits for fraction.
 */
template<typename BT, std::size_t Fraction> class FixedPointNumber
{
    static_assert(std::is_integral<BT>::value, "BT must be an integral");
    static_assert(Fraction > 0, "Fraction must be greater than zero");
    static_assert(Fraction <= sizeof(BT) * 8, "Fractional bits must be less than or equal to the number of bits in T");

    template<typename T> struct BitsBeyondSpecification
    {
        template<typename U, typename std::enable_if< std::is_signed<U>::value && (std::numeric_limits<int16_t>::digits > std::numeric_limits<U>::digits), std::nullptr_t>::type = nullptr> int16_t static foo() { return 0; }
        template<typename U, typename std::enable_if< std::is_unsigned<U>::value && (std::numeric_limits<uint16_t>::digits > std::numeric_limits<U>::digits), std::nullptr_t>::type = nullptr> static  uint16_t foo() { return 0; }
        template<typename U, typename std::enable_if< std::is_signed<U>::value && (std::numeric_limits<int32_t>::digits > std::numeric_limits<U>::digits) &&  (std::numeric_limits<U>::digits > std::numeric_limits<std::int8_t>::digits) , std::nullptr_t>::type = nullptr> static int32_t foo() { return 0; }
        template<typename U, typename std::enable_if< std::is_unsigned<U>::value && (std::numeric_limits<uint32_t>::digits > std::numeric_limits<U>::digits) &&  (std::numeric_limits<U>::digits > std::numeric_limits<std::uint8_t>::digits) , std::nullptr_t>::type = nullptr> static uint32_t foo() { return 0; }
        template<typename U, typename std::enable_if< std::is_signed<U>::value && (std::numeric_limits<int64_t>::digits > std::numeric_limits<U>::digits) &&  (std::numeric_limits<U>::digits > std::numeric_limits<std::int16_t>::digits) , std::nullptr_t>::type = nullptr> static int64_t foo() { return 0; }
        template<typename U, typename std::enable_if< std::is_unsigned<U>::value && (std::numeric_limits<uint64_t>::digits > std::numeric_limits<U>::digits) &&  (std::numeric_limits<U>::digits > std::numeric_limits<std::uint16_t>::digits) , std::nullptr_t>::type = nullptr> static uint64_t foo() { return 0; }
        using type = decltype(foo<T>());
    };
    /*! @brief Type for used to store intermediate results in calculation */
    using CalculationType = typename BitsBeyondSpecification<BT>::type;

    static_assert(std::numeric_limits<CalculationType>::digits > std::numeric_limits<BT>::digits, "Insufficient number of bits in CalculationType");
    
  public:
    using base_type = BT;
    constexpr static std::size_t FRACTION = Fraction;
    constexpr static BT FACTOR = BT(1) << Fraction;
    
    /// @name Constructor
    /// @{
    GOBLIB_INLINE constexpr FixedPointNumber() : _value(0) {}
    template<typename U, typename std::enable_if<std::is_integral<U>::value, std::nullptr_t>::type = nullptr>
    GOBLIB_INLINE constexpr explicit  FixedPointNumber(const U v)
            : _value(static_cast<BT>(v * FACTOR))
    {
#if defined(GOBLIB_CPP17_OR_LATER)
        assert(v <= static_cast<U>(std::numeric_limits<FixedPointNumber>::max()) && "Overflow");
        assert(v >= static_cast<U>(std::numeric_limits<FixedPointNumber>::min()) && "Underflow");
#endif
    }
    template<typename U, typename std::enable_if<std::is_floating_point<U>::value, std::nullptr_t>::type = nullptr>
    GOBLIB_INLINE constexpr explicit FixedPointNumber(const U v)
            : _value(static_cast<BT>( (v >= 0) ?
                                      (v * FACTOR + static_cast<BT>(0.5f)) :
                                      (v * FACTOR - static_cast<BT>(0.5f))
                                      ))
    {
#if defined(GOBLIB_CPP17_OR_LATER)
        assert(v <= static_cast<U>(std::numeric_limits<FixedPointNumber>::max()) && "Overflow");
        assert(v >= static_cast<U>(std::numeric_limits<FixedPointNumber>::min()) && "Underflow");
#endif        
    }
    GOBLIB_INLINE constexpr FixedPointNumber(const FixedPointNumber& f) : _value(f._value) {}
    template<typename U, std::size_t Frac>
    GOBLIB_INLINE constexpr FixedPointNumber(const FixedPointNumber<U, Frac>& f) : _value(convert<Frac>(f.raw()).raw()) {}
    /// @}

    /// @name Assignment
    /// @{
    GOBLIB_INLINE FixedPointNumber& operator=(const FixedPointNumber& f)
    {
        _value = f._value;
        return *this;
    }
    template<typename U, std::size_t Frac>
    GOBLIB_INLINE FixedPointNumber& operator=(const FixedPointNumber<U, Frac>& f)
    {
        _value = convert<Frac>(f.raw()).raw();
        return *this;
    }
    /// @}
    
    /// @name Cast
    /// @{
    constexpr GOBLIB_INLINE explicit operator bool() const { return _value != 0; }

    template<typename U, typename std::enable_if<std::is_integral<U>::value, std::nullptr_t>::type = nullptr>
    constexpr GOBLIB_INLINE explicit operator U() const { return static_cast<U>(_value / FACTOR); }
    template<typename U, typename std::enable_if<std::is_floating_point<U>::value, std::nullptr_t>::type = nullptr>
    constexpr GOBLIB_INLINE explicit operator U() const { return static_cast<U>(_value) / FACTOR; }

    template<typename U, typename std::enable_if<std::is_arithmetic<U>::value, std::nullptr_t>::type = nullptr>
    constexpr GOBLIB_INLINE U value() const { return static_cast<U>(*this); }
    /// @}

    /// @name Raw value
    /// @{
    constexpr GOBLIB_INLINE BT raw() const { return _value; }
    /// @}

    /// @name Convert
    /// @{
    template<std::size_t Frac, typename U, typename std::enable_if< (Frac > Fraction), std::nullptr_t >::type = nullptr>
    constexpr static GOBLIB_INLINE FixedPointNumber convert(const U v)
    {
        return FixedPointNumber(static_cast<BT>(v / (BT(1) << (Frac - Fraction)) +
                                               (v / (BT(1) << (Frac - Fraction - 1)) & 1) ), nullptr);
    }
    template<std::size_t Frac, typename U, typename std::enable_if< (Frac <= Fraction), std::nullptr_t >::type = nullptr>
    constexpr static GOBLIB_INLINE FixedPointNumber convert(const U v)
    {
        return FixedPointNumber(static_cast<BT>(v * (BT(1) << (Fraction - Frac))), nullptr);
    }
    constexpr static FixedPointNumber from_raw(const BT v) { return FixedPointNumber(v, nullptr); }
    /// @}

    /// @name Unary Negation/Plus
    /// @{
    GOBLIB_INLINE FixedPointNumber operator+() const { return FixedPointNumber( _value, nullptr); }
    GOBLIB_INLINE FixedPointNumber operator-() const { return FixedPointNumber(-_value, nullptr); }
    /// @}

    /// @name Compound Assignment
    /// @{
    GOBLIB_INLINE FixedPointNumber operator+=(const FixedPointNumber& f)
    {
        _value += f._value;
        return *this;
    }
    template<typename U, typename std::enable_if<std::is_integral<U>::value, std::nullptr_t>::type = nullptr>
    GOBLIB_INLINE FixedPointNumber operator+=(const U v)
    {
        _value += v * FACTOR;
        return *this;
    }
    template<typename U, typename std::enable_if<std::is_floating_point<U>::value, std::nullptr_t>::type = nullptr>
    GOBLIB_INLINE FixedPointNumber operator+=(const U v)
    {
        return *this += FixedPointNumber<BT, Fraction>(v);
    }
    
    GOBLIB_INLINE FixedPointNumber operator-=(const FixedPointNumber& f)
    {
        _value -= f._value;
        return *this;
    }
    template<typename U, typename std::enable_if<std::is_integral<U>::value, std::nullptr_t>::type = nullptr>
    GOBLIB_INLINE FixedPointNumber operator-=(const U v)
    {
        _value -= v * FACTOR;
        return *this;
    }
    template<typename U, typename std::enable_if<std::is_floating_point<U>::value, std::nullptr_t>::type = nullptr>
    GOBLIB_INLINE FixedPointNumber operator-=(const U v)
    {
        return *this -= FixedPointNumber<BT, Fraction>(v);
    }

    GOBLIB_INLINE FixedPointNumber operator*=(const FixedPointNumber& f)
    {
        auto iv = (static_cast<CalculationType>(_value) * f._value) / (FACTOR / 2);
        _value = static_cast<BT>((iv / 2) + (iv & 1));
        return *this;
    }
    template<typename U, typename std::enable_if<std::is_integral<U>::value, std::nullptr_t>::type = nullptr>
    GOBLIB_INLINE FixedPointNumber operator*=(const U v)
    {
        _value *= v;
        return *this;
    }
    template<typename U, typename std::enable_if<std::is_floating_point<U>::value, std::nullptr_t>::type = nullptr>
    GOBLIB_INLINE FixedPointNumber operator*=(const U v)
    {
        //        return *this *= FixedPointNumber<BT, Fraction>(v);
        _value *= v;
        return *this;
    }

    GOBLIB_INLINE FixedPointNumber operator/=(const FixedPointNumber& f)
    {
        assert(f._value != 0 && "Divide by zero");
        auto iv = (static_cast<CalculationType>(_value) * (FACTOR * 2)) / f._value;
        _value = static_cast<BT>((iv / 2) + (iv & 1));
        return *this;
    }
    template<typename U, typename std::enable_if<std::is_integral<U>::value, std::nullptr_t>::type = nullptr>
    GOBLIB_INLINE FixedPointNumber operator/=(const U v)
    {
        assert(v != 0 && "Divide by zero");
        _value /= v;
        return *this;
    }
    template<typename U, typename std::enable_if<std::is_floating_point<U>::value, std::nullptr_t>::type = nullptr>
    GOBLIB_INLINE FixedPointNumber operator/=(const U v)
    {
        //        return *this /= FixedPointNumber<BT, Fraction>(v);
        _value /= v;
        return *this;
    }
    /// @}
    
#ifndef NDEBUG
    const char* str() const
    {
        static char sbuf[sizeof(BT)*8+1];
        auto p = sbuf + sizeof(sbuf) - 1;
        *p-- = '\0';
        typename std::make_unsigned<BT>::type v = _value;
        while(v)
        {
            *p-- = '0' + (v&1);
            v /= 2;
        }
        assert(p + 1 >= sbuf);
        return p + 1;
    }
#endif
    
  protected:
    /*! @brief Create directly from value type of BT*/
    constexpr FixedPointNumber(const BT v, const void*) : _value(v) {}
    
  private:
    BT _value;
};

/// @name Compare
/// @{
template<typename BT, std::size_t Fraction>
constexpr GOBLIB_INLINE bool operator<(const FixedPointNumber<BT, Fraction>& a, const FixedPointNumber<BT, Fraction>& b)
{
    return a.raw() < b.raw();
}
template<typename BT, std::size_t Fraction,
         typename U, typename std::enable_if<std::is_arithmetic<U>::value, std::nullptr_t>::type = nullptr>
constexpr GOBLIB_INLINE bool operator<(const FixedPointNumber<BT, Fraction>& a, const U& b)
{
    return a < FixedPointNumber<BT,Fraction>(b);
}
template<typename BT, std::size_t Fraction,
         typename U, typename std::enable_if<std::is_arithmetic<U>::value, std::nullptr_t>::type = nullptr>
constexpr GOBLIB_INLINE bool operator<(const U& a, const FixedPointNumber<BT, Fraction>& b)
{
    return FixedPointNumber<BT,Fraction>(a) < b;
}

template<typename BT, std::size_t Fraction>
constexpr GOBLIB_INLINE bool operator<=(const FixedPointNumber<BT, Fraction>& a, const FixedPointNumber<BT, Fraction>& b)
{
    return std::rel_ops::operator<=(a, b);
}
template<typename BT, std::size_t Fraction,
         typename U, typename std::enable_if<std::is_arithmetic<U>::value, std::nullptr_t>::type = nullptr>
constexpr GOBLIB_INLINE bool operator<=(const FixedPointNumber<BT, Fraction>& a, const U& b)
{
    return a <= FixedPointNumber<BT,Fraction>(b);
}
template<typename BT, std::size_t Fraction,
         typename U, typename std::enable_if<std::is_arithmetic<U>::value, std::nullptr_t>::type = nullptr>
constexpr GOBLIB_INLINE bool operator<=(const U& a, const FixedPointNumber<BT, Fraction>& b)
{
    return FixedPointNumber<BT,Fraction>(a) <= b;
}

template<typename BT, std::size_t Fraction>
constexpr GOBLIB_INLINE bool operator>(const FixedPointNumber<BT, Fraction>& a, const FixedPointNumber<BT, Fraction>& b)
{
    return std::rel_ops::operator>(a, b);
}
template<typename BT, std::size_t Fraction,
         typename U, typename std::enable_if<std::is_arithmetic<U>::value, std::nullptr_t>::type = nullptr>
constexpr GOBLIB_INLINE bool operator>(const FixedPointNumber<BT, Fraction>& a, const U& b)
{
    return a > FixedPointNumber<BT,Fraction>(b);
}
template<typename BT, std::size_t Fraction,
         typename U, typename std::enable_if<std::is_arithmetic<U>::value, std::nullptr_t>::type = nullptr>
constexpr GOBLIB_INLINE bool operator>(const U& a, const FixedPointNumber<BT, Fraction>& b)
{
    return FixedPointNumber<BT,Fraction>(a) > b;
}

template<typename BT, std::size_t Fraction>
constexpr GOBLIB_INLINE bool operator>=(const FixedPointNumber<BT, Fraction>& a, const FixedPointNumber<BT, Fraction>& b)
{
    return std::rel_ops::operator>=(a, b);
}
template<typename BT, std::size_t Fraction,
         typename U, typename std::enable_if<std::is_arithmetic<U>::value, std::nullptr_t>::type = nullptr>
constexpr GOBLIB_INLINE bool operator>=(const FixedPointNumber<BT, Fraction>& a, const U& b)
{
    return a >= FixedPointNumber<BT,Fraction>(b);
}
template<typename BT, std::size_t Fraction,
         typename U, typename std::enable_if<std::is_arithmetic<U>::value, std::nullptr_t>::type = nullptr>
constexpr GOBLIB_INLINE bool operator>=(const U& a, const FixedPointNumber<BT, Fraction>& b)
{
    return FixedPointNumber<BT,Fraction>(a) >= b;
}
/// @}

/// @name Equality Compare
/// @{
template<typename BT, std::size_t Fraction>
constexpr GOBLIB_INLINE bool operator==(const FixedPointNumber<BT, Fraction>& a, const FixedPointNumber<BT, Fraction>& b)
{
    return a.raw() == b.raw();
}
template<typename BT, std::size_t Fraction,
         typename U, typename std::enable_if<std::is_arithmetic<U>::value, std::nullptr_t>::type = nullptr>
constexpr GOBLIB_INLINE bool operator==(const FixedPointNumber<BT, Fraction>& a, const U& b)
{
    return a == FixedPointNumber<BT,Fraction>(b);
}
template<typename BT, std::size_t Fraction,
         typename U, typename std::enable_if<std::is_arithmetic<U>::value, std::nullptr_t>::type = nullptr>
constexpr GOBLIB_INLINE bool operator==(const U& a, const FixedPointNumber<BT, Fraction>& b)
{
    return FixedPointNumber<BT,Fraction>(a) == b;
}

template<typename BT, std::size_t Fraction>
constexpr GOBLIB_INLINE bool operator!=(const FixedPointNumber<BT, Fraction>& a, const FixedPointNumber<BT, Fraction>& b)
{
    return std::rel_ops::operator!=(a, b);
}
template<typename BT, std::size_t Fraction,
         typename U, typename std::enable_if<std::is_arithmetic<U>::value, std::nullptr_t>::type = nullptr>
constexpr GOBLIB_INLINE bool operator!=(const FixedPointNumber<BT, Fraction>& a, const U& b)
{
    return a != FixedPointNumber<BT,Fraction>(b);
}
template<typename BT, std::size_t Fraction,
         typename U, typename std::enable_if<std::is_arithmetic<U>::value, std::nullptr_t>::type = nullptr>
constexpr GOBLIB_INLINE bool operator!=(const U& a, const FixedPointNumber<BT, Fraction>& b)
{
    return FixedPointNumber<BT,Fraction>(a) != b;
}

/// @}

/// @name Arithmetic
/// @{
template<typename BT, std::size_t Fraction>
constexpr GOBLIB_INLINE FixedPointNumber<BT, Fraction> operator+(const FixedPointNumber<BT, Fraction>& a, const FixedPointNumber<BT, Fraction>& b)
{
    return FixedPointNumber<BT, Fraction>(a) += b;

}
template<typename BT, std::size_t Fraction, typename U, typename std::enable_if<std::is_arithmetic<U>::value, std::nullptr_t>::type = nullptr>
constexpr GOBLIB_INLINE FixedPointNumber<BT, Fraction> operator+(const FixedPointNumber<BT, Fraction>& a, const U b)
{
    return FixedPointNumber<BT, Fraction>(a) += b;
}
template<typename BT, std::size_t Fraction, typename U, typename std::enable_if<std::is_arithmetic<U>::value, std::nullptr_t>::type = nullptr>
constexpr GOBLIB_INLINE FixedPointNumber<BT, Fraction> operator+(const U a, const FixedPointNumber<BT, Fraction>& b)
{
    return FixedPointNumber<BT, Fraction>(b) += a;
}

template<typename BT, std::size_t Fraction>
constexpr GOBLIB_INLINE FixedPointNumber<BT, Fraction> operator-(const FixedPointNumber<BT, Fraction>& a, const FixedPointNumber<BT, Fraction>& b)
{
    return FixedPointNumber<BT, Fraction>(a) -= b;

}
template<typename BT, std::size_t Fraction, typename U, typename std::enable_if<std::is_arithmetic<U>::value, std::nullptr_t>::type = nullptr>
constexpr GOBLIB_INLINE FixedPointNumber<BT, Fraction> operator-(const FixedPointNumber<BT, Fraction>& a, const U b)
{
    return FixedPointNumber<BT, Fraction>(a) -= b;
}
template<typename BT, std::size_t Fraction, typename U, typename std::enable_if<std::is_arithmetic<U>::value, std::nullptr_t>::type = nullptr>
constexpr GOBLIB_INLINE FixedPointNumber<BT, Fraction> operator-(const U a, const FixedPointNumber<BT, Fraction>& b)
{
    return FixedPointNumber<BT, Fraction>(a) -= b;
}

template<typename BT, std::size_t Fraction>
constexpr GOBLIB_INLINE FixedPointNumber<BT, Fraction> operator*(const FixedPointNumber<BT, Fraction>& a, const FixedPointNumber<BT, Fraction>& b)
{
    return FixedPointNumber<BT, Fraction>(a) *= b;

}
template<typename BT, std::size_t Fraction, typename U, typename std::enable_if<std::is_arithmetic<U>::value, std::nullptr_t>::type = nullptr>
constexpr GOBLIB_INLINE FixedPointNumber<BT, Fraction> operator*(const FixedPointNumber<BT, Fraction>& a, const U b)
{
    return FixedPointNumber<BT, Fraction>(a) *= b;
}
template<typename BT, std::size_t Fraction, typename U, typename std::enable_if<std::is_arithmetic<U>::value, std::nullptr_t>::type = nullptr>
constexpr GOBLIB_INLINE FixedPointNumber<BT, Fraction> operator*(const U a, const FixedPointNumber<BT, Fraction>& b)
{
    return FixedPointNumber<BT, Fraction>(b) *= a;
}

template<typename BT, std::size_t Fraction>
constexpr GOBLIB_INLINE FixedPointNumber<BT, Fraction> operator/(const FixedPointNumber<BT, Fraction>& a, const FixedPointNumber<BT, Fraction>& b)
{
    return FixedPointNumber<BT, Fraction>(a) /= b;

}
template<typename BT, std::size_t Fraction, typename U, typename std::enable_if<std::is_arithmetic<U>::value, std::nullptr_t>::type = nullptr>
constexpr GOBLIB_INLINE FixedPointNumber<BT, Fraction> operator/(const FixedPointNumber<BT, Fraction>& a, const U b)
{
    return FixedPointNumber<BT, Fraction>(a) /= b;
}
template<typename BT, std::size_t Fraction, typename U, typename std::enable_if<std::is_arithmetic<U>::value, std::nullptr_t>::type = nullptr>
constexpr GOBLIB_INLINE FixedPointNumber<BT, Fraction> operator/(const U a, const FixedPointNumber<BT, Fraction>& b)
{
    return FixedPointNumber<BT, Fraction>(a) /= b;
}
/// @}

/// @cond
template<typename> struct is_fixed_point_number : std::false_type {};
/// @endcond

/*! @brief Check FixedPointNumber class */
template<typename T, std::size_t Frac> struct is_fixed_point_number<goblib::FixedPointNumber<T,Frac>> : std::true_type {};


/// @cond
struct FixedPointNumberLimits
{
    constexpr static float log10_2 = 0.30102999566398114f; // std::log10(2)

    template<typename BT, std::size_t Fraction>
    constexpr static int digits10(std::size_t bits) noexcept
    {
        using U = std::common_type<std::size_t, long long int>::type;
        return static_cast<int>( ((U)bits * goblib::FixedPointNumber<BT, Fraction>(log10_2).raw()) >> Fraction);
    }
    /*
      ceil(1 + digits * log10(radix))
    */
    template<typename BT, std::size_t Fraction>
    constexpr static int max_digits10(std::size_t bits) noexcept
    {
        using U = std::common_type<std::size_t, long long int>::type;
        return static_cast<int>( ((U)bits * goblib::FixedPointNumber<BT, Fraction>(log10_2).raw() + ((U)1 << Fraction) - 1) >> Fraction);
    }
};
/// @endcond


namespace math
{
template<typename T, typename std::enable_if<goblib::is_fixed_point_number<T>::value, std::nullptr_t>::type = nullptr>
GOBLIB_INLINE constexpr bool equal(const T x, const T y) { return x == y; }

template<typename T, typename std::enable_if<goblib::is_fixed_point_number<T>::value, std::nullptr_t>::type = nullptr>
GOBLIB_INLINE constexpr bool not_equal(const T x, const T y) { return !(x == y); }

template<typename T, typename std::enable_if<goblib::is_fixed_point_number<T>::value, std::nullptr_t>::type = nullptr>
GOBLIB_INLINE constexpr const T& fmin(const T& left, const T& right)
{
    return std::min(left, right);
}

template<typename T, typename std::enable_if<goblib::is_fixed_point_number<T>::value, std::nullptr_t>::type = nullptr>
GOBLIB_INLINE constexpr const T& fmax(const T left, const T right)
{
    return std::max(left, right);
}

template<typename T> GOBLIB_INLINE constexpr auto round(const T v)
        -> typename std::enable_if<goblib::is_fixed_point_number<T>::value, T>::type
{
    return T(static_cast<typename T::base_type>(v) + goblib::math::sign(v.raw()) * ((goblib::math::abs(v.raw()) & (typename T::base_type(1) << (T::FRACTION - 1))) != 0) );
}
                       
template<typename T> GOBLIB_INLINE constexpr auto floor(const T v)
        -> typename std::enable_if<goblib::is_fixed_point_number<T>::value, T>::type
{
    return T::from_raw((v.raw() >> T::FRACTION) << T::FRACTION);
}

template<typename T> GOBLIB_INLINE constexpr auto ceil(const T v)
        -> typename std::enable_if<goblib::is_fixed_point_number<T>::value, T>::type
{
    return T(static_cast<typename T::base_type>(v) + ((v.raw() > 0) && ((v.raw() & (T::FACTOR - 1)) != 0)) );
}

template<typename T> GOBLIB_INLINE constexpr auto fabs(const T v)
        -> typename std::enable_if<goblib::is_fixed_point_number<T>::value, T>::type
{
    return T::from_raw(std::abs(v.raw()));
}
//
}


/// @name Specialzed for goblib functions
/// @note Use std::clamp or goblib::clamp in gob_utility.hpp if C++17 or later.
/// @{
#if defined(GOBLIB_CPP14_OR_EARLIER)
template<typename T, typename std::enable_if<goblib::is_fixed_point_number<T>::value, std::nullptr_t>::type = nullptr>
GOBLIB_INLINE constexpr const T& clamp(const T& v, const T& low, const T& high)
{
    return goblib::math::fmin( goblib::math::fmax(v, low), high);
}
#endif
/// @}

//
}

namespace std
{

/*! @brief std::numeric_limits specialized for FixedPointNumber
  @sa https://en.cppreference.com/w/cpp/types/numeric_limits
 */
template<typename BT, std::size_t Fraction>
struct numeric_limits<goblib::FixedPointNumber<BT, Fraction>>
{
    static_assert(std::is_integral<BT>::value, "BT must be an integral");
    static_assert(Fraction > 0, "Fraction must be greater than zero");

    constexpr static goblib::FixedPointNumber<BT, Fraction> min() noexcept { return goblib::FixedPointNumber<BT, Fraction>::from_raw(std::numeric_limits<BT>::min()); }
    constexpr static goblib::FixedPointNumber<BT, Fraction> max() noexcept { return goblib::FixedPointNumber<BT, Fraction>::from_raw(std::numeric_limits<BT>::max()); }
    constexpr static goblib::FixedPointNumber<BT, Fraction> lowest() noexcept { return min(); }
    constexpr static goblib::FixedPointNumber<BT, Fraction> epsilon() noexcept { return goblib::FixedPointNumber<BT, Fraction>::from_raw(1); }
    constexpr static goblib::FixedPointNumber<BT, Fraction> round_error() noexcept { return goblib::FixedPointNumber<BT, Fraction>(1) / 2; }
    constexpr static goblib::FixedPointNumber<BT, Fraction> denorm_min() noexcept { return min(); }

    /// @name Invalid values
    /// @warning has_infinity,has_quiet_Nan,has_signaling_Nan are false.
    /// @{
    constexpr static goblib::FixedPointNumber<BT, Fraction> infinity() noexcept { return goblib::FixedPointNumber<BT, Fraction>(); }
    constexpr static goblib::FixedPointNumber<BT, Fraction> quiet_NaN() noexcept { return goblib::FixedPointNumber<BT, Fraction>(); }
    constexpr static goblib::FixedPointNumber<BT, Fraction> signaling_NaN() noexcept { return goblib::FixedPointNumber<BT, Fraction>(); }
    /// @}

    constexpr static bool is_specialized = true;
    constexpr static int digits = std::numeric_limits<BT>::digits;
    constexpr static int digits10 = 1;
    constexpr static int max_digits10 = goblib::FixedPointNumberLimits::max_digits10<BT,Fraction>(std::numeric_limits<BT>::digits - Fraction) + goblib::FixedPointNumberLimits::max_digits10<BT,Fraction>(Fraction);
    constexpr static bool is_signed = std::numeric_limits<BT>::is_signed;
    constexpr static bool is_integer = false;
    constexpr static bool is_exact = std::numeric_limits<BT>::is_exact;

    constexpr static int radix = std::numeric_limits<BT>::radix;

    constexpr static int min_exponent = ( 1 - Fraction );
    constexpr static int min_exponent10 =  -goblib::FixedPointNumberLimits::digits10<BT,Fraction>(Fraction);
    constexpr static int max_exponent = ( std::numeric_limits<BT>::digits - Fraction );
    constexpr static int max_exponent10 = goblib::FixedPointNumberLimits::digits10<BT,Fraction>(std::numeric_limits<BT>::digits - Fraction);

    constexpr static bool has_infinity = false;
    constexpr static bool has_quiet_NaN = false;
    constexpr static bool has_signaling_NaN = false;
    constexpr static std::float_denorm_style has_denorm = std::numeric_limits<BT>::has_denorm;
    constexpr static bool has_denorm_loss = std::numeric_limits<BT>::has_denorm_loss;

    constexpr static bool is_iec559 = false;
    constexpr static bool is_bounded = std::numeric_limits<BT>::is_bounded;
    constexpr static bool is_modulo = std::numeric_limits<BT>::is_modulo;

    constexpr static bool traps = std::numeric_limits<BT>::traps;
    constexpr static bool tinyness_before = false;
    constexpr static std::float_round_style round_style = std::round_to_nearest;
};

template<typename BT, std::size_t Fraction> constexpr bool numeric_limits<goblib::FixedPointNumber<BT, Fraction>>::is_specialized;
template<typename BT, std::size_t Fraction> constexpr int  numeric_limits<goblib::FixedPointNumber<BT, Fraction>>::digits;
template<typename BT, std::size_t Fraction> constexpr int  numeric_limits<goblib::FixedPointNumber<BT, Fraction>>::digits10;
template<typename BT, std::size_t Fraction> constexpr int  numeric_limits<goblib::FixedPointNumber<BT, Fraction>>::max_digits10;
template<typename BT, std::size_t Fraction> constexpr bool numeric_limits<goblib::FixedPointNumber<BT, Fraction>>::is_signed;
template<typename BT, std::size_t Fraction> constexpr bool numeric_limits<goblib::FixedPointNumber<BT, Fraction>>::is_integer;
template<typename BT, std::size_t Fraction> constexpr bool numeric_limits<goblib::FixedPointNumber<BT, Fraction>>::is_exact;
template<typename BT, std::size_t Fraction> constexpr int  numeric_limits<goblib::FixedPointNumber<BT, Fraction>>::radix;
template<typename BT, std::size_t Fraction> constexpr int  numeric_limits<goblib::FixedPointNumber<BT, Fraction>>::min_exponent;
template<typename BT, std::size_t Fraction> constexpr int  numeric_limits<goblib::FixedPointNumber<BT, Fraction>>::min_exponent10;
template<typename BT, std::size_t Fraction> constexpr int  numeric_limits<goblib::FixedPointNumber<BT, Fraction>>::max_exponent;
template<typename BT, std::size_t Fraction> constexpr int  numeric_limits<goblib::FixedPointNumber<BT, Fraction>>::max_exponent10;
template<typename BT, std::size_t Fraction> constexpr bool numeric_limits<goblib::FixedPointNumber<BT, Fraction>>::has_infinity;
template<typename BT, std::size_t Fraction> constexpr bool numeric_limits<goblib::FixedPointNumber<BT, Fraction>>::has_quiet_NaN;
template<typename BT, std::size_t Fraction> constexpr bool numeric_limits<goblib::FixedPointNumber<BT, Fraction>>::has_signaling_NaN;
template<typename BT, std::size_t Fraction> constexpr std::float_denorm_style numeric_limits<goblib::FixedPointNumber<BT, Fraction>>::has_denorm;
template<typename BT, std::size_t Fraction> constexpr bool numeric_limits<goblib::FixedPointNumber<BT, Fraction>>::has_denorm_loss;
template<typename BT, std::size_t Fraction> constexpr bool numeric_limits<goblib::FixedPointNumber<BT, Fraction>>::is_iec559;
template<typename BT, std::size_t Fraction> constexpr bool numeric_limits<goblib::FixedPointNumber<BT, Fraction>>::is_bounded;
template<typename BT, std::size_t Fraction> constexpr bool numeric_limits<goblib::FixedPointNumber<BT, Fraction>>::is_modulo;
template<typename BT, std::size_t Fraction> constexpr bool numeric_limits<goblib::FixedPointNumber<BT, Fraction>>::traps;
template<typename BT, std::size_t Fraction> constexpr bool numeric_limits<goblib::FixedPointNumber<BT, Fraction>>::tinyness_before;
template<typename BT, std::size_t Fraction> constexpr std::float_round_style numeric_limits<goblib::FixedPointNumber<BT, Fraction>>::round_style;

//
}
#endif
