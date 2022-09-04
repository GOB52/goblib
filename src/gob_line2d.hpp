/*!
  Goblin Library

  @file   gob_line2d.hpp
  @brief  2d Line segment
*/
#pragma once
#ifndef GOBLIB_LINE2D_HPP
#define GOBLIB_LINE2D_HPP

#include "gob_math.hpp"
#include "gob_macro.hpp"
#include "gob_point2d.hpp"

namespace goblib { namespace shape2d {

// -------------------------------------------------------------------
/*!
  LineSegment
  @brief 2D line segment definition.
*/
template<typename T> class LineSegment
{
    static_assert(goblib::is_fixed_point_number<T>::value || std::is_arithmetic<T>::value, "T must be arithmetic type");

  public:
    using pos_type = T;

    /// @name Constructor
    /// @{
    constexpr LineSegment() : LineSegment(T(0),T(0),T(0),T(0)) {}
    constexpr LineSegment(const T sx, const T sy, const T ex, const T ey) : _start(sx, sy), _end(ex, ey) {}
    constexpr LineSegment(const Point<T>& s, const Point<T>& e) : _start(s) , _end(e) {}
    constexpr LineSegment(const LineSegment<T>& o) : _start(o._start), _end(o._end) {}
    constexpr LineSegment(LineSegment<T>&& o) : _start(o._start), _end(o._end) {}
    /// @}

    /// @name  Property
    /// @{
    constexpr GOBLIB_INLINE T sx() const { return _start.x(); }
    constexpr GOBLIB_INLINE T sy() const { return _start.y(); }
    constexpr GOBLIB_INLINE T ex() const { return _end.x(); }
    constexpr GOBLIB_INLINE T ey() const { return _end.y(); }
    
    constexpr GOBLIB_INLINE Point<T> start() const { return _start; }
    constexpr GOBLIB_INLINE Point<T> end() const { return _end; }
    /// @}
    
    /// @name Assignment
    /// @{
    GOBLIB_INLINE LineSegment<T>& operator=(const LineSegment<T>& o)
    {
        if(this != &o)
        {
            _start = o._start; _end = o._end;
        }
        return *this;
    }
    GOBLIB_INLINE LineSegment<T>& operator=(LineSegment<T>&& o)
    {
        if(this != o)
        {
            _start = o._start; _end = o._end;
            o.zero();
        }
        return *this;
    }
    /// @}

    /// @name Calculate
    /// @{
    GOBLIB_INLINE T length() const { return (_end - _start).length(); }
    constexpr GOBLIB_INLINE T lengthSq() const { return (_end - _start).lengthSq(); }

    /*! @brief Angle for arithmetic */
    template <typename U = T, typename std::enable_if<std::is_arithmetic<U>::value, std::nullptr_t>::type = nullptr>
    GOBLIB_INLINE auto angle() const -> decltype(std::atan2(U(),U()))
    { return std::atan2(_end.y() - _start.y(), _end.x() - _start.x()); }
    /*! @brief Angle for fixed point number */
    template <typename U = T, typename std::enable_if<goblib::is_fixed_point_number<U>::value, std::nullptr_t>::type = nullptr>
    GOBLIB_INLINE float angle() const
    { return std::atan2( (_end.y() - _start.y()).template value<float>(), (_end.x() - _start.x()).template value<float>()); }
    /// @}

    /// @name Cast
    /// @{
    GOBLIB_INLINE explicit operator bool() const { return _start != _end; }
    /// @}

    GOBLIB_INLINE void zero()
    {
        _start = _end = Point<T>();
    }
    GOBLIB_INLINE void move(const T& mx, const T& my) 
    {
        _end.offset(mx - _start.x(), my - _start.y());
        _start.move(mx, my);
    }
    GOBLIB_INLINE void offset(const T& ox, const T& oy) 
    {
        _start.offset(ox, oy);
        _end.offset(ox, oy);
    }
    
  private:
    Point<T> _start, _end;
};


/// @name Compare
/// @{
template<typename T> constexpr GOBLIB_INLINE bool operator==(const LineSegment<T>& a, const LineSegment<T>& b)
{
    return a.start() == b.start() && a.end() == b.end();
}

template<typename T> constexpr GOBLIB_INLINE bool operator!=(const LineSegment<T>& a, const LineSegment<T>& b)
{
    return std::rel_ops::operator!=(a,b);
}
/// @}

//
}}
#endif
