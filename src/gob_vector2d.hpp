/*!
  Goblin Library

  @file   gob_vector2d.hpp
  @brief  2D Vector
*/
#pragma once
#ifndef GOBLIB_VECTOR2D_HPP
#define GOBLIB_VECTOR2D_HPP

#include <cstdint>
#include <cmath>
#include <limits>
#include <type_traits>
#include "gob_shape2d.hpp"
#include "gob_math.hpp"
#include "gob_utility.hpp"
#include "gob_fixed_point_number.hpp"
#include "gob_macro.hpp"

namespace goblib { namespace shape2d {
/*!
  Vector2
  @brief 2D Vector
  @tparam T coordinate type
*/
template<typename T> class Vector2 : public Shape<T>
{
    static_assert(goblib::is_fixed_point_number<T>::value || std::is_arithmetic<T>::value, "T must be arithmetic type");

  public:
    /// @name Constructor
    /// @{
    constexpr Vector2() : _x(0), _y(0) {}
    constexpr Vector2(T x, T y) : _x(x), _y(y) {}
    constexpr Vector2(const Vector2<T>& v) : _x(v._x), _y(v._y) {}
    constexpr Vector2(const Point<T>& v) : _x(v.x()), _y(v.y()) {}
    Vector2(Vector2<T>&& o) : _x(o._x), _y(o._y) {/*o.zero();*/ }
    template<typename U> constexpr Vector2(const Vector2<U>& v) : _x(static_cast<T>(v.x())), _y(static_cast<T>(v.y())) {}
    /// @}
    
    /// @name  Property
    /// @{
    GOBLIB_INLINE constexpr T x() const { return _x; }
    GOBLIB_INLINE constexpr T y() const { return _y; }
    /// @}

    /// @name Assignment
    /// @{
    GOBLIB_INLINE Vector2<T>& operator=(const Vector2<T>& v)
    {
        if(this != &v)
        {
            _x = v._x;
            _y = v._y;
        }
        return *this;
    }
    GOBLIB_INLINE Vector2<T>& operator=(Vector2<T>&& v)
    {
        if(this != &v)
        {
            _x = v._x;
            _y = v._y;
            v.zero();
        }
        return *this;
    }
    template<typename U> GOBLIB_INLINE Vector2<T>& operator=(const Vector2<U>& v)
    {
        if(this != &v)
        {
            _x = static_cast<T>(v._x);
            _y = static_cast<T>(v._y);
        }
        return *this;
    }
    /// @}

    /// @name Check
    /// @{
    GOBLIB_INLINE constexpr bool valid() const { return std::isfinite(_x) && std::isfinite(_y); }
    GOBLIB_INLINE constexpr bool perpendicular(const Vector2<T>& v, const T tolerance = Vector2<T>::VECTOR2_EPSILON) const
    {
        return std::abs(normalizeV().dot(v.normalizeV())) < tolerance;
    }
    GOBLIB_INLINE constexpr bool parallel(const Vector2<T>& v, const T tolerance = Vector2<T>::VECTOR2_EPSILON) const
    {
        return std::abs(normalizeV().cross(v.normalizeV())) < tolerance;
    }
    GOBLIB_INLINE constexpr bool near(const Vector2<T>& v, const T tolerance = static_cast<T>(0.0001f))
    {
        return goblib::math::equal_tolerance(x(), v.x(), tolerance) && goblib::math::equal_tolerance(y(), v.y(), tolerance);
    }
    /// @}
    
    /// @name Calculate
    /// @{
    GOBLIB_INLINE T length() const
    {
        return std::sqrt(dot(*this));
    }
    GOBLIB_INLINE constexpr T lengthSq() const
    {
        return dot(*this);
    }

    GOBLIB_INLINE T distance(const Vector2<T>& v) const
    {
        return (v - *this).length();
    }
    GOBLIB_INLINE constexpr T distanceSq(const Vector2<T>& v) const
    {
        return (v - *this).lengthSq();
    }
    
    GOBLIB_INLINE constexpr T angle() const
    {
        return std::atan2(_y, _x);
    }
    GOBLIB_INLINE T angle(const Vector2<T>& v) const
    {
        return std::acos( dot(v) / (length() * v.length()) );
    }

    GOBLIB_INLINE Vector2<T>& abs()
    {
        _x = std::abs(_x);
        _y = std::abs(_y);
        return *this;
    }
    GOBLIB_INLINE constexpr Vector2<T> absV() const
    {
        return Vector2<T>(std::abs(_x), std::abs(_y));
    }
    
    GOBLIB_INLINE constexpr T dot(const Vector2<T>& v) const
    {
        return (_x * v._x) + (_y * v._y);
    }

    GOBLIB_INLINE constexpr T cross(const Vector2<T>& v) const
    {
        return (_x * v._y) - (_y * v._x);
    }

    GOBLIB_INLINE void normalize()
    {
        *this /= length();
    }
    GOBLIB_INLINE constexpr Vector2<T> normalizeV() const
    {
        return *this / length();
    }
    
    GOBLIB_INLINE void perpendicular()
    {
        std::swap(_x, _y);
        _x = -_x;
    }
    GOBLIB_INLINE constexpr Vector2<T> perpendicularV() const
    {
        return Vector2<T>(-_y, _x);
    }

    GOBLIB_INLINE void truncate(const T len)
    {
        auto a = angle();
        _x = len * std::cos(a);
        _y = len * std::sin(a);
    }
    GOBLIB_INLINE constexpr Vector2<T> truncateV(const T len) const
    {
        return Vector2<T>(len * std::cos(angle()), len * std::sin(angle()));
    }
    
    GOBLIB_INLINE void clamp(const T minl, const T maxl)
    {
        truncate(goblib::clamp(length(), minl, maxl));
    }
    GOBLIB_INLINE constexpr Vector2<T> clampV(const T minl, const T maxl) const
    {
        return truncateV(goblib::clamp(length(), minl, maxl));
    }
    
    GOBLIB_INLINE constexpr Vector2<T> projectionV(const Vector2<T>& v) const
    {
        return (dot(v) / v.dot(v)) * v;
    }

    constexpr Vector2<T> rejectionV(const Vector2<T>& v) const
    {
        return *this - projectionV(v);
    }

    constexpr Vector2<T> reflectionV(const Vector2<T>& norm) const
    {
#ifdef DEBUG
        assert(norm.lengthSq() <= 1.0f && "norm must be normalized vector");
#endif
        return *this - 2.0f * dot(norm) * norm;
    }
    /// @}
    
    /// @name Compare
    /// @{
    GOBLIB_INLINE constexpr bool operator==(const Vector2<T>& v) const
    {
        return goblib::math::equal(_x, v._x) && goblib::math::equal(_y, v._y);
    }
    GOBLIB_INLINE constexpr bool operator!=(const Vector2<T>& v) /*const*/ { return !(*this == v); }
    /// @}
    
    /// @name Compound Assignment
    /// @{
    GOBLIB_INLINE Vector2<T>& operator+=(const Vector2<T>& v)
    {
        _x += v._x;
        _y += v._y;
        return *this;
    }

    GOBLIB_INLINE Vector2<T>& operator-=(const Vector2<T>& v)
    {
        _x -= v._x;
        _y -= v._y;
        return *this;
    }

    GOBLIB_INLINE Vector2<T>& operator*=(const T& value)
    {
        _x *= value;
        _y *= value;
        return *this;
    }
    GOBLIB_INLINE Vector2<T>& operator*=(const Vector2<T>& v)
    {
        _x *= v._x;
        _y *= v._y;
        return *this;
    }

    GOBLIB_INLINE Vector2<T>& operator/=(const T& value)
    {
        return *this *= (static_cast<T>(1) / value);
    }
    GOBLIB_INLINE Vector2<T>& operator/=(const Vector2<T>& v)
    {
        auto xx = static_cast<T>(1) / v._x;
        auto yy = static_cast<T>(1) / v._y;
        _x *= xx;
        _y *= yy;
        return *this;
    }
    /// @}

    /// @name  Unary negation
    /// @{
    GOBLIB_INLINE constexpr Vector2<T> operator-() const
    {
        return Vector2<T>(-_x, -_y);
    }
    /// @}

    /// @name Override
    /// @{
    GOBLIB_INLINE virtual void zero() override { _x = _y = static_cast<T>(0); }
    GOBLIB_INLINE virtual void move(const T& mx, const T& my) override
    {
        _x = mx;
        _y = my;
    }
    GOBLIB_INLINE virtual void offset(const T& ox, const T& oy) override
    {
        _x += ox;
        _y += oy;
    }
    /// @}

#if defined(GOBLIB_CPP17_OR_LATER)
    constexpr static Vector2<T> X_AXIS = Vector2<T>(1, 0); //!< X axis.
    constexpr static Vector2<T> Y_AXIS = Vector2<T>(0, 1); //!< Y axis.
    constexpr static Vector2<T> ZERO_VECTOR = Vector2<T>(0, 0); //!< Zero vector
#else
    static const Vector2<T> X_AXIS;
    static const Vector2<T> Y_AXIS;
    static const Vector2<T> ZERO_VECTOR;
#endif
    
  private:
    T _x, _y;
    constexpr static T VECTOR2_EPSILON = std::numeric_limits<T>::epsilon();
};

#if defined(GOBLIB_CPP14_OR_EARLIER)
template<typename T> const Vector2<T> Vector2<T>::X_AXIS = Vector2<T>(1, 0); //!< X axis.
template<typename T> const Vector2<T> Vector2<T>::Y_AXIS = Vector2<T>(0, 1); //!< Y axis.
template<typename T> const Vector2<T> Vector2<T>::ZERO_VECTOR = Vector2<T>(0, 0); //!< Zero vector
#endif

/// @name Arithmetic
/// @{
template<typename T> GOBLIB_INLINE
constexpr Vector2<T> operator+(const Vector2<T>& a, const Vector2<T>& b)
{
    return Vector2<T>(a.x() + b.x(), a.y() + b.y());
}

template<typename T> GOBLIB_INLINE
constexpr Vector2<T> operator-(const Vector2<T>& a, const Vector2<T>& b)
{
    return Vector2<T>(a.x() - b.x(), a.y() - b.y());
}

template<typename T> GOBLIB_INLINE
constexpr Vector2<T> operator*(const Vector2<T>& v, const T m)
{
    return Vector2<T>(v.x() * m, v.y() * m);
}

template<typename T> GOBLIB_INLINE
constexpr Vector2<T> operator*(const T m, const Vector2<T>& v)
{
    return Vector2<T>(v.x() * m, v.y() * m);
}

template<typename T> GOBLIB_INLINE
constexpr Vector2<T> operator*(const Vector2<T>& a, const Vector2<T>& b)
{
    return Vector2<T>(a.x() * b.x(), a.y() * b.y());
}

template<typename T> GOBLIB_INLINE
constexpr Vector2<T> operator/(const Vector2<T>& v, const T m)
{
    return v * (static_cast<T>(1) / m);
}

template<typename T> GOBLIB_INLINE
constexpr Vector2<T> operator/(const Vector2<T>& a, const Vector2<T>& b)
{
    return Vector2<T>(a.x() * (static_cast<T>(1) / b.x()), a.y() * (static_cast<T>(1) / b.y()));
}
/// @}

/// @name Calculation
/// @{ 

// @brief Is v left of bs - be vector?
template<typename T> GOBLIB_INLINE
bool isLeft(const Vector2<T>& v, const Vector2<T>& bs, const Vector2<T>& be)
{
    return std::isless((be - bs).cross(v - bs), 0.0f);
}
// @brief Is v right of bs - be vector?
template<typename T> GOBLIB_INLINE
bool isRight(const Vector2<T>& v, const Vector2<T>& bs, const Vector2<T>& be)
{
    return std::isgreater((be - bs).cross(v - bs), 0.0f);
}

// @brief Is as - ae vector intersect bs - be vector?
// @retval true intersect
template<typename T>
bool isIntersect(const Vector2<T>& as, const Vector2<T>& ae, const Vector2<T>& bs, const Vector2<T>& be)
{
    auto v1 = ae - as;
    auto v2 = be - bs;
    auto cv1v2 = v1.cross(v2);

    if(cv1v2 == 0.0f) { return false; } // parallel?
    
    auto v = bs - as;
    auto t1 = v.cross(v2) / cv1v2;
    auto t2 = v.cross(v1) / cv1v2;

    return(std::isless(t1, 0.0f) || std::isgreater(t1, 1.0f) ||
           std::isless(t2, 0.0f) || std::isgreater(t2, 1.0f)) ? false : true;
}

// @brief get crossing point and reflection point if intersect
// @retval true intersect
template<typename T>
bool reflect(const Vector2<T>& as, const Vector2<T>& ae, const Vector2<T>& bs, const Vector2<T>& be, Vector2<T>& cp, Vector2<T>& rp)
{
    auto v1 = ae - as;
    auto v2 = be - bs;
    auto cv1v2 = v1.cross(v2);

    if(cv1v2 == 0.0f) { return false; } // parallel?
    
    auto v = bs - as;
    auto t1 = v.cross(v2) / cv1v2;
    auto t2 = v.cross(v1) / cv1v2;

    // No crossing?
    if(std::isless(t1, 0.0f) || std::isgreater(t1, 1.0f) ||
       std::isless(t2, 0.0f) || std::isgreater(t2, 1.0f))
    {
        return false;
    }
    // Calculate crossing point and reflection point
    cp= as + (v1 * t1);
    rp = v1.reflectionV(v2.perpendicularV().normalizeV()) * (1.0f - t1) + cp;

    return true;
}
/// @}

#if defined(GOBLIB_CPP17_OR_LATER)
/// @name  Template specialization
/// @{
template<>  GOBLIB_INLINE constexpr
float Vector2<float>::angle() const { return ::atan2f(_y, _x); } // Using atan2f() if float.
template<>  GOBLIB_INLINE constexpr
double Vector2<double>::angle() const { return std::atan2(_y, _x); } // Using atan2() if double.
template<>  GOBLIB_INLINE constexpr
long double Vector2<long double>::angle() const { return ::atan2l(_y, _x); } // Using atan2l() if long double.
/// @}
#endif

//
}}
#endif
