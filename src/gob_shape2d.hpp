/*!
  Goblin Library

  @file   gob_shape2d.hpp
  @brief  2d Shape base and coordinate.
*/
#pragma once
#ifndef GOBLIB_SHAPE2D_HPP
#define GOBLIB_SHAPE2D_HPP

#include "gob_math.hpp"
#include "gob_macro.hpp"
#include "gob_fixed_point_number.hpp"
#include <type_traits> // std::is_integral,...
#include <utility> // std::rel_ops
#include <initializer_list>

namespace goblib
{
/*!
  shape2d
  @brief 2D shapes.
*/
namespace shape2d
{

// -------------------------------------------------------------------
/*! 
  Shape
  @brief Interface class for shapes.
  @remarks Abstract class
*/
template<typename T>
class Shape
{
  public:
    using pos_type = T;
    virtual void zero() = 0;
    virtual void move(const T& mx, const T& my) = 0;
    virtual void offset(const T& mx, const T& my) = 0;
    //    virtual void rotate(const float& rad) = 0;
};

// -------------------------------------------------------------------
/*!
  Point
  @brief 2D coordinate.
*/
template<typename T> class Point : public Shape<T>
{
    static_assert(goblib::is_fixed_point_number<T>::value || std::is_arithmetic<T>::value, "T must be arithmetic type");

  public:
    /// @name Constructor
    /// @{
    constexpr Point() : Point(T(0),T(0)) {}
    constexpr Point(const T& x, const T& y) : _x(x), _y(y) {}
    constexpr Point(const Point<T>& o) : _x(o._x), _y(o._y) {}
    Point(Point<T>&& o) : _x(o._x), _y(o._y) { o.zero(); }

    template<typename U> constexpr Point(const Point<U>& o) : _x(o.x()), _y(o.y()) {}
    template<typename U, typename V,
             typename std::enable_if<std::is_arithmetic<U>::value, std::nullptr_t>::type = nullptr,
             typename std::enable_if<std::is_arithmetic<V>::value, std::nullptr_t>::type = nullptr >
    constexpr Point(const U& x, const V& y) : _x(x), _y(y) {}
    /// @}

    /// @name  Property
    /// @{
    constexpr GOBLIB_INLINE T x() const { return _x; }
    constexpr GOBLIB_INLINE T y() const { return _y; }
    /// @}

    //    friend constexpr Point<T> operator*<>(const Point<T>& a, const U& b);
    //    friend constexpr Point<T> operator*<>(const U& a, const Point<T>& b);
    
    /// @name Assignment
    /// @{
    GOBLIB_INLINE Point<T>& operator=(const Point<T>& o)
    {
        if(this != &o)
        {
            _x = o.x();
            _y = o.y();
        }
        return *this;
    }
    GOBLIB_INLINE Point<T>& operator=(Point<T>&& o)
    {
        if(this != &o)
        {
            _x = o.x();
            _y = o.y();
            o.zero();
        }
        return *this;
    }
    /// @}

    /// @name Unary Negation/Plus
    /// @{
    GOBLIB_INLINE Point<T> operator+() const { return Point<T>(*this); }
    GOBLIB_INLINE Point<T> operator-() const { return Point<T>(-_x, -_y); }
    /// @}
    
    /// @name Cast
    /// @{
    template<typename U> GOBLIB_INLINE operator Point<U>() const { return Point<U>(static_cast<U>(_x), static_cast<U>(_y)); }
    /// @}

    /// @name Compound Assignment
    /// @{
    GOBLIB_INLINE Point<T>& operator+=(const Point<T>& o)
    {
        _x += o.x();
        _y += o.y();
        return *this;
    }
    GOBLIB_INLINE Point<T>& operator-=(const Point<T>& o)
    {
        _x -= o.x();
        _y -= o.y();
        return *this;
    }
    template<typename U,
             typename std::enable_if<goblib::is_fixed_point_number<T>::value || std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr>
    GOBLIB_INLINE Point<T>& operator*=(const U& v)
    {
        _x *= v;
        _y *= v;
        return *this;
    }
    /// @}

    /// @name Calculate
    /// @{
    /*! @brief Length from origin */
    GOBLIB_INLINE T length() const { return std::sqrt(lengthSq()); }
    /*! @brief Length squared from origin */
    constexpr GOBLIB_INLINE T lengthSq() const { return (_x * _x) + (_y * _y); }
    /*! @brief Angle from origin for arithmetic */
    template <typename U = T, typename std::enable_if<std::is_arithmetic<U>::value, std::nullptr_t>::type = nullptr>
    GOBLIB_INLINE auto angle() const -> decltype(std::atan2(U(),U())) { return std::atan2(_y, _x); }
    /*! @brief Angle from origin for fixed point number */
    template <typename U = T, typename std::enable_if<goblib::is_fixed_point_number<U>::value, std::nullptr_t>::type = nullptr>
    GOBLIB_INLINE float angle() const { return std::atan2(_y.template value<float>(), _x.template value<float>()); }
    /// @}
    
    /// @name Override
    /// @{
    GOBLIB_INLINE virtual void zero() override { _x = _y = T(0); }
    GOBLIB_INLINE virtual void move(const T& x, const T& y) override { _x = x; _y = y; }
    GOBLIB_INLINE virtual void offset(const T& ox, const T& oy) override { _x += ox; _y += oy; }
    /// @}

  private:
    T _x, _y;
};

/// @name Arithmetic
/// @file gob_shape2d.hpp
/// @{
template<typename T> GOBLIB_INLINE
constexpr Point<T> operator+(const Point<T>& a, const Point<T>& b)
{
    return Point<T>(a) += b;
}

template<typename T> GOBLIB_INLINE
constexpr Point<T> operator-(const Point<T>& a, const Point<T>& b)
{
    return Point<T>(a) -= b;
}

template<typename T, typename U,
         typename std::enable_if<goblib::is_fixed_point_number<T>::value || std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr>
GOBLIB_INLINE constexpr Point<T> operator*(const Point<T>& a, const U& b)
{
    return Point<T>(a) *= b;
}

template<typename T, typename U,
         typename std::enable_if<goblib::is_fixed_point_number<T>::value || std::is_arithmetic<T>::value, std::nullptr_t>::type = nullptr>
GOBLIB_INLINE constexpr Point<T> operator*(const U& a, const Point<T>& b)
{
    return Point<T>(b) *= a;
}
/// @}

/// @name Compare
/// @{
template<typename T> constexpr GOBLIB_INLINE bool operator==(const Point<T>& a, const Point<T>& b)
{
    return goblib::math::equal(a.x(), b.x()) && goblib::math::equal(a.y(), b.y());
}

/*! @brief The distance from the origin is used to determine if it is lesser. */
template<typename T> constexpr GOBLIB_INLINE bool operator<(const Point<T>& a, const Point<T>& b)
{
    return a.lengthSq() < b.lengthSq();
}

template<typename T> constexpr GOBLIB_INLINE bool operator!=(const Point<T>& a, const Point<T>& b)
{
    return std::rel_ops::operator!=(a,b);
}

template<typename T> constexpr GOBLIB_INLINE bool operator<=(const Point<T>& a, const Point<T>& b)
{
    return std::rel_ops::operator<=(a,b);
}

template<typename T> constexpr GOBLIB_INLINE bool operator>(const Point<T>& a, const Point<T>& b)
{
    return std::rel_ops::operator>(a,b);
}

template<typename T> constexpr GOBLIB_INLINE bool operator>=(const Point<T>& a, const Point<T>& b)
{
    return std::rel_ops::operator>=(a,b);
}
/// @}

//
}}
#endif
