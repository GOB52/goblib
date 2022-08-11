/*!
  Goblin Library

  @file   gob_ellipse2d.hpp
  @brief  2D Ellipse,Circle
*/

#pragma once
#ifndef GOBLIB_ELLIPSE2D_HPP
#define GOBLIB_ELLIPSE2D_HPP

#include "gob_macro.hpp"
#include "gob_math.hpp"
#include "gob_fixed_point_number.hpp"
#include "gob_point2d.hpp"

#include <cmath> // std::sin,cos
#include <utility> // std::pair

namespace goblib { namespace shape2d {

/*! 
  @brief Ellipse
  @tparam T coordinate type
*/
template<typename T> class Ellipse
{
    static_assert(goblib::is_fixed_point_number<T>::value || std::is_arithmetic<T>::value, "T must be arithmetic type");

  public:
    using pos_type = T;

    constexpr Ellipse() : _center(T(0), T(0)), _radius{T(0), T(0)}, _rotate(0.0f) {}
    constexpr Ellipse(const T& cx, const T& cy, const T& radiusH, const T& radiusV, const float rotate = 0.0f)
            : _center(cx,cy), _radius{radiusH, radiusV}, _rotate(rotate) {}
    constexpr Ellipse(const Point<T>& pos, const T& radiusH, const T& radiusV, const float rotate = 0.0f)
            : Ellipse(pos.x(), pos.y(), radiusH, radiusV, rotate) {}
    
    /// @name Center
    /// @{
    GOBLIB_INLINE Point<T>  center() const { return _center; }
    GOBLIB_INLINE T cx() const { return _center.x(); }
    GOBLIB_INLINE T cy() const { return _center.y(); }
    /// @}
    
    /// @name Rotate, Radius
    // @{
    constexpr GOBLIB_INLINE float rotate() const { return _rotate; }
    constexpr GOBLIB_INLINE std::pair<T,T> radius() const { return std::make_pair(_radius[0], _radius[1]); }

    GOBLIB_INLINE void setRotate(const float rad) { _rotate = rad; }
    GOBLIB_INLINE void setRadius(const T& h, const T& v) { _radius[0] = h; _radius[1] = v; }
    GOBLIB_INLINE void setRadius(const std::pair<T,T>& hv) { _radius[0] = hv.first; _radius[1] = hv.second; }
    /// @}

    /*! Get coordinate on the circumference of ellipse */
    GOBLIB_INLINE constexpr Point<T> pos(const float rad) const
    {
        return Point<T>(
            ((float)_radius[0] * std::cos(rad) * std::cos(_rotate)) - ((float)_radius[1] * std::sin(rad) * std::sin(_rotate)) + _center.x(),
            ((float)_radius[0] * std::cos(rad) * std::sin(_rotate)) + ((float)_radius[1] * std::sin(rad) * std::cos(_rotate) + _center.y())
                        );

        /*! @warning If T is an integer, the output will vary depending on how the calculation is performed.
          @sa goblib::shape2d::Circle::pos
         */
    }

    GOBLIB_INLINE void zero()
    {
        _center = Point<T>();
        _rotate = 0.0f;
        _radius[0] = _radius[1] = T(0);
    }
    GOBLIB_INLINE void move(const T& mx, const T& my)
    {
        _center = Point<T>(mx, my);
    }
    GOBLIB_INLINE void offset(const T& ox, const T& oy)
    {
        _center += Point<T>(ox, oy);
    }
    
  protected:
    Point<T> _center;
    T _radius[2];  // a, b
    float _rotate; //
};

/*! 
  @brief Cirlcle
  @tparam T coordinate type
*/
template<typename T> class Circle
{
    static_assert(goblib::is_fixed_point_number<T>::value || std::is_arithmetic<T>::value, "T must be arithmetic type");

  public:
    using pos_type = T;

    constexpr Circle() : Circle(T(0),T(0),T(0)) {}
    constexpr Circle(const T& cx, const T& cy, const T& radius)
            : _center(cx,cy), _radius(radius) {}
    constexpr Circle(const Point<T>& pos,      const T& radius) : Circle(pos.x(), pos.y(), radius) {}

    /// @name Center
    /// @{
    GOBLIB_INLINE Point<T>  center() const { return _center; }
    GOBLIB_INLINE T cx() const { return _center.x(); }
    GOBLIB_INLINE T cy() const { return _center.y(); }
    /// @}
    
    /// @name Radius
    // @{
    constexpr GOBLIB_INLINE T radius() const { return _radius; }
    GOBLIB_INLINE void setRadius(const T& r) { _radius = r; }
    /// @}

    /*! Get coordinate on the circumference of circle */
    GOBLIB_INLINE constexpr Point<T> pos(const float rad) const
    {
        return Point<T>(T((float)_radius * std::cos(rad) + _center.x()), T((float)_radius * std::sin(rad)) + _center.y()); 
        //        return Point<T>(T((float)_radius * std::cos(rad)), T((float)_radius * std::sin(rad))) + _center; 

        /*!  
          @warning If T is an integer, the output will vary depending on how the calculation is performed.
          @warning 1) return Point<T>(T((float)_radius * std::cos(rad) + _center.x()), T((float)_radius * std::sin(rad)) + _center.y());
          @warning 2) return Point<T>(T((float)_radius * std::cos(rad)), T((float)_radius * std::sin(rad))) + _center;
          @warning  1) Point<int>(-8.682409f + 100, -8.682409f + 100); => (91,91)
          @warning 2) Point<int>(-8.682409f, -8.682409f) + Poiint32<int>(100,100); => (92,92)
        */
    }

    GOBLIB_INLINE virtual void zero()
    {
        _center = Point<T>();
        _radius = T(0);
    }
    GOBLIB_INLINE void move(const T& mx, const T& my) 
    {
        _center = Point<T>(mx, my);
    }
    GOBLIB_INLINE void offset(const T& ox, const T& oy) 
    {
        _center += Point<T>(ox, oy);
    }
    
  protected:
    Point<T> _center;
    T _radius;
};

//
}}
#endif
