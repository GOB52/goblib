/*!
  Goblin Library

  @file   gob_rect2d.hpp
  @brief  2D Rectangle.
*/
#pragma once
#ifndef GOBLIB_RECT2D_HPP
#define GOBLIB_RECT2D_HPP

#ifdef min
#undef min
#endif

#include "gob_shape2d.hpp"
#include "gob_math.hpp"
#include "gob_fixed_point_number.hpp"
#include "gob_macro.hpp"
#include <limits> // numeric_limits
#include <algorithm> // std::min.max
#include <type_traits> // std::is_integral,...
#include <utility>

namespace goblib { namespace shape2d {

// -------------------------------------------------------------------
/*!
  Rectangle
  @brief 2D Rectangle.
  @tparam T type of value.
*/
template<typename T>
class Rectangle : public Shape <T>
{
    static_assert(goblib::is_fixed_point_number<T>::value || std::is_arithmetic<T>::value, "T must be arithmetic type");

  public:
    /// @name Constructor
    /// @{
    constexpr Rectangle() : Rectangle(T(0),T(0),T(0),T(0)) {}
    constexpr Rectangle(T x, T y, T w, T h) : _pos(x,y), _w(w), _h(h) {}
    constexpr Rectangle(const Point<T>& pos, const T& w, const T& h) : _pos(pos), _w(w), _h(w) {}
    constexpr Rectangle(const Point<T>& lt, const Point<T>& rb) : _pos(lt), _w(rb.x() - lt.x() + (T)1), _h(rb.y() - lt.y() + (T)1) {}
    constexpr Rectangle(const Rectangle<T>& o) : _pos(o._pos), _w(o._w), _h(o._h) {}
    Rectangle(Rectangle<T>&& o) : _pos(o._pos), _w(o._w), _h(o._h) { o.zero(); }
    /// @}

    /// @name Assignment
    /// @{
    GOBLIB_INLINE Rectangle<T>& operator=(const Rectangle<T>& o)
    {
        _pos = o._pos;
        _w = o._w;
        _h = o._h;
        return *this;
    }
    GOBLIB_INLINE Rectangle<T>& operator=(Rectangle<T>&& o)
    {
        _pos = o._pos;
        _w = o._w;
        _h = o._h;
        o.zero();
        return *this;
    }
    /// @}

    /// @name  Property
    /// @{
    constexpr GOBLIB_INLINE T left() const { return _pos.x(); }
    constexpr GOBLIB_INLINE T top() const { return _pos.y(); }
    constexpr GOBLIB_INLINE T right() const { return  left() + width() - T(1); }
    constexpr GOBLIB_INLINE T bottom() const { return top() + height() - T(1); }

    constexpr GOBLIB_INLINE T width() const { return _w; }
    constexpr GOBLIB_INLINE T height() const { return _h; }

    constexpr GOBLIB_INLINE Point<T> leftTop() const { return _pos; }
    constexpr GOBLIB_INLINE Point<T> centerTop() const { return Point<T>(left() + width()/2, top()); }
    constexpr GOBLIB_INLINE Point<T> rightTop() const { return Point<T>(right(), top()); }
    constexpr GOBLIB_INLINE Point<T> leftCenter() const { return Point<T>(left(), top() + height()/2); }
    constexpr GOBLIB_INLINE Point<T> center() const { return Point<T>(left() + width()/2, top() + height()/2); }    
    constexpr GOBLIB_INLINE Point<T> rightCenter() const { return Point<T>(right(), top() + height()/2); }
    constexpr GOBLIB_INLINE Point<T> leftBottom() const { return Point<T>(left(), bottom()); }
    constexpr GOBLIB_INLINE Point<T> centerBottom() const { return Point<T>(left() + width()/2, bottom()); }
    constexpr GOBLIB_INLINE Point<T> rightBottom() const { return Point<T>(right(), bottom()); }
    /// @}

    /// @name Check
    /// @{
    constexpr GOBLIB_INLINE bool empty() const { return _w <= T(0) || _h <= T(0); }
    constexpr GOBLIB_INLINE bool valid() const { return !empty() && !(_pos.x() > std::numeric_limits<T>::max() - _w) && !(_pos.y() > std::numeric_limits<T>::max() - _h); }

    constexpr GOBLIB_INLINE bool contains(T x, T y) const
    {
        return valid() &&
                x >= left() && x <= right() && y >= top() && y <= right();
    }
    constexpr GOBLIB_INLINE bool contains(const Rectangle& r) const
    {
        return valid() && r.valid() &&
                r.left() >= left() && r.right() <= right() && r.top() >= top() && r.bottom() <= bottom();
    }
    constexpr GOBLIB_INLINE bool overlaps(const Rectangle& r) const
    {
        return valid() && r.valid() &&
                right() >= r.left() && r.right() >= left() && bottom() >= r.top() && r.bottom() > top();
    }
    /// @}

    /// @name Cast
    /// @{
    GOBLIB_INLINE explicit operator bool() const { return valid(); }
    /// @}
    
    /// @name Compound Assignment
    /// @{
    template<typename U> GOBLIB_INLINE Rectangle<T>& operator+=(const Point<U>& pos)
    {
        _pos += pos;
        return *this;
    }
    template<typename U> GOBLIB_INLINE Rectangle<T>& operator-=(const Point<U>& pos)
    {
        _pos -= pos;
        return *this;
    }

    /*! @brief Get intersect */
    Rectangle<T>& operator&=(const Rectangle<T>& rr)
    {
        T xx = std::max(left(), rr.left());
        T yy = std::max(top(), rr.top());
        _w  = std::min(left() + width(), rr.left() + rr.width()) - xx;
        _h = std::min(top() + height(), rr.top() + rr.height()) - yy;
        _pos.move(xx, yy);
        return *this;
    }

    /*! @brief Get union */
    Rectangle<T>& operator|=(const Rectangle<T>& rr)
    {
        if(empty()) { *this = rr; }
        else if(!rr.empty())
        {
            T xx = std::min(left(), rr.left());
            T yy = std::min(top(), rr.top());
            _w  = std::max(left() + width(), rr.left() + rr.width()) - xx;
            _h = std::max(top() + height(), rr.top() + rr.height()) - yy;
            _pos.move(xx,yy);
        }
        return *this;
    }
    /// @}

    /// @name Operation
    /// @{
    GOBLIB_INLINE void move(const Point<T>& pos) { move(pos.x(), pos.y()); }
    GOBLIB_INLINE void offset(const Point<T>& off) { offset(off.x(), off.y()); }
    GOBLIB_INLINE void inflate(const T& w, const T& h) { _w = w; _h = h; }
    /// @}
    
    /// @name Override
    /// @{
    GOBLIB_INLINE virtual void zero() override
    {
        _pos = Point<T>(); _w = _h = T(0);
    }

    GOBLIB_INLINE virtual void move(const T& mx, const T& my) override
    {
        _pos.move(mx, my);
    }

    GOBLIB_INLINE virtual void offset(const T& ox, const T& oy) override
    {
        _pos.offset(ox, oy);
    }
    /// @}
    
  protected:
    Point<T> _pos;  //!< left top.
    T _w, _h;       //!< width, height
};

/// @name Arithmetic
/// @{
template<typename T> GOBLIB_INLINE Rectangle<T> operator&(const Rectangle<T>& lr, const Rectangle<T>& rr)
{
    return Rectangle<T>(lr) &= rr;
}

template<typename T> GOBLIB_INLINE Rectangle<T> operator|(const Rectangle<T>& lr, const Rectangle<T>& rr)
{
    return Rectangle<T>(lr) |= rr;
}

template<typename T, typename U> Rectangle<T> operator+(const Rectangle<T>& r, const Point<U>& pos)
{
    return Rectangle<T>(r) += pos;
}

template<typename T, typename U> Rectangle<T> operator-(const Rectangle<T>& r, const Point<U>& pos)
{
    return Rectangle<T>(r) -= pos;
}
/// @}
    
/// @name Compare
/// @{
template<typename T> constexpr GOBLIB_INLINE bool operator==(const Rectangle<T>& a, const Rectangle<T>& b)
{
    return a.leftTop() == b.leftTop()
            && goblib::math::equal(a.width(),  b.width())
            && goblib::math::equal(a.height(), b.height());
}

template<typename T> constexpr GOBLIB_INLINE bool operator!=(const Rectangle<T>& a, const Rectangle<T>& b)
{
    return std::rel_ops::operator!=(a,b);
}
/// @}

//
}}
#endif 
