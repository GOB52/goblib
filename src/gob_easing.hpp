/*!
  Goblin Library

  @file   gob_easing.hpp
  @brief  Easing
  @todo Convert some non-constexpr functions to constexpr.
*/
#pragma once
#ifndef GOBLIB_EASING_HPP
#define GOBLIB_EASING_HPP

#include "gob_macro.hpp"
#include "gob_template_helper.hpp"
#include <cstdint>
#include <cmath> // std::cos,sin...
#include <gob_math.hpp> // pi,half_pi

#include <cstdio>

namespace goblib
{
/*!
  @brief Easing functions.
 */
namespace easing
{

/// @name Easing behavior
/// @note t [0.0 ~ 1.0]
/// @{
GOBLIB_INLINE constexpr float linear(const float t)
{
    return 1.0f * t;
}

GOBLIB_INLINE constexpr float quadratic_in(const float t)
{
    return 1.0f * t * t;
}

GOBLIB_INLINE constexpr float quadratic_out(const float t)
{
    return -1.0f * t * (t - 2.0f);
}

GOBLIB_INLINE constexpr float quadratic_inout(const float t)
{
    return (t * 2.0f) < 1.0f ?
        0.5f * (t * 2.0f) * (t * 2.0f) :
        -0.5f * (((t * 2.0f) - 1.0f) * (((t * 2.0f) - 1.0f) - 2.0f) - 1.0f);
}

GOBLIB_INLINE constexpr float cubic_in(const float t)
{
    return 1.0f * t * t * t;
}

GOBLIB_INLINE constexpr float cubic_out(const float t)
{
    return 1.0f * ((t - 1.0f)* (t - 1.0f) * (t - 1.0f) + 1.0f);
}

GOBLIB_INLINE constexpr float cubic_inout(const float t)
{
    return (t * 2.0f) < 1.0f ?
        0.5f * (t * 2.0f) * (t * 2.0f ) * (t * 2.0f) :
        0.5f * ((t * 2.0f - 2.0f) * (t * 2.0f - 2.0f) * (t * 2.0f - 2.0f) + 2.0f);
}

GOBLIB_INLINE constexpr float quartic_in(const float t)
{
    return 1.0f * t * t * t * t;
}

GOBLIB_INLINE constexpr float quartic_out(const float t)
{
    return -1.0f * ((t - 1.0f) * (t - 1.0f) * (t - 1.0f) * (t - 1.0f) - 1.0f);
}

GOBLIB_INLINE constexpr float quartic_inout(const float t)
{
    return (t * 2.0f) < 1.0f ?
        0.5f * (t * 2.0f) * (t * 2.0f) * (t * 2.0f) * (t * 2.0f) :
        -0.5f * ((t * 2.0f - 2.0f) * (t * 2.0f - 2.0f) * (t * 2.0f - 2.0f) * (t * 2.0f - 2.0f) - 2.0f);
}

GOBLIB_INLINE constexpr float quintic_in(const float t)
{
    return 1.0f * t * t * t * t * t;
}

GOBLIB_INLINE constexpr float quintic_out(const float t)
{
    return 1.0f * ((t - 1.0f) * (t - 1.0f) * (t - 1.0f) * (t - 1.0f) * (t - 1.0f) + 1.0f);
}

GOBLIB_INLINE constexpr float quintic_inout(const float t)
{
    return (t * 2.0f) < 1.0f ?
        0.5f * (t * 2.0f) * (t * 2.0f) * (t * 2.0f) * (t * 2.0f) * (t * 2.0f) :
        0.5f * ((t * 2.0f - 2.0f) * (t * 2.0f - 2.0f) * (t * 2.0f - 2.0f) * (t * 2.0f - 2.0f) * (t * 2.0f - 2.0f) + 2.0f);
}

#if defined(__GNUG__) && !defined(__clang__)
GOBLIB_INLINE constexpr float sinusoidal_in(const float t)
#else
GOBLIB_INLINE float sinusoidal_in(const float t)
#endif
{
    return -1.0f * std::cos(t * math::constants::half_pi_f) + 1.0f;
}

#if defined(__GNUG__) && !defined(__clang__)
GOBLIB_INLINE constexpr float sinusoidal_out(const float t)
#else
GOBLIB_INLINE float sinusoidal_out(const float t)
#endif
{
    return 1.0f * std::sin(t * math::constants::half_pi_f);
}

#if defined(__GNUG__) && !defined(__clang__)
GOBLIB_INLINE constexpr float sinusoidal_inout(const float t)
#else
GOBLIB_INLINE float sinusoidal_inout(const float t)
#endif
{
    return -0.5f * (std::cos(t * math::constants::pi_f) - 1.0f);
}

#if defined(__GNUG__) && !defined(__clang__)
GOBLIB_INLINE constexpr float exponential_in(const float t)
#else
GOBLIB_INLINE float exponential_in(const float t)
#endif
{
    return (t == 0.0f) ? 0.0f : 1.0f * std::pow(2.0f, 10.0f * (t - 1.0f));
}

#if defined(__GNUG__) && !defined(__clang__)
GOBLIB_INLINE constexpr float exponential_out(const float t)
#else
GOBLIB_INLINE float exponential_out(const float t)
#endif
{
    return (t == 1.0f) ? 1.0f : 1.0f * (-std::pow(2.0f, -10.0f * t) + 1.0f);
}

#if defined(__GNUG__) && !defined(__clang__)
GOBLIB_INLINE constexpr float exponential_inout(const float t)
#else
GOBLIB_INLINE float exponential_inout(const float t)
#endif
{
    return (t == 0.0f) ? 0.0f :
        (t == 1.0f) ? 1.0f :
        (t * 2.0f) < 1.0f ?
        0.5f * std::pow(2.0f, 10.0f * (t * 2.0f - 1.0f)) :
        0.5f * (-std::pow(2.0f, -10.0f * (t * 2.0f - 1.0f)) + 2.0f);
}

#if defined(__GNUG__) && !defined(__clang__)
GOBLIB_INLINE constexpr float circular_in(const float t)
#else
GOBLIB_INLINE float circular_in(const float t)
#endif
{
    return -1.0f * (std::sqrt(1.0f - t * t) - 1.0f);
}

#if defined(__GNUG__) && !defined(__clang__)
GOBLIB_INLINE constexpr float circular_out(const float t)
#else
GOBLIB_INLINE  float circular_out(const float t)
#endif
{
    return 1.0f * std::sqrt(1.0f - (t - 1.0f) * (t - 1.0f));
}

#if defined(__GNUG__) && !defined(__clang__)
GOBLIB_INLINE constexpr float circular_inout(const float t)
#else
GOBLIB_INLINE float circular_inout(const float t)
#endif
{
    return (t * 2.0f) < 1.0f ?
        -0.5f * (std::sqrt(1.0f - (t * 2.0f) * (t * 2.0f)) - 1.0f) :
        0.5f * (std::sqrt(1.0f - (t * 2.0f - 2.0f) * (t * 2.0f - 2.0f)) + 1.0f);
}

namespace
{
constexpr float BackFactor = 1.70158f;
constexpr float BackFactor2 = BackFactor * 1.525f;
}
GOBLIB_INLINE constexpr float back_in(const float t)
{
    return 1.0f * t * t * ((BackFactor + 1.0f ) * t - BackFactor);
}

GOBLIB_INLINE constexpr float back_out(const float t)
{
    return 1.0f * ((t - 1.0f)* (t - 1.0f) * ((BackFactor + 1.0f ) * (t - 1.0f) + BackFactor) + 1.0f);
}

GOBLIB_INLINE constexpr float back_inout(const float t)
{
    return (t * 2.0f) < 1.0f ?
        0.5f * ((t * 2.0f) * (t * 2.0f) * ((BackFactor2 + 1.0f) * (t * 2.0f) - BackFactor2) ) :
        0.5f * ((t * 2.0f - 2.0f) * (t * 2.0f - 2.0f) * ((BackFactor2 + 1.0f) * (t * 2.0f - 2.0f) + BackFactor2) + 2.0f);
}

namespace
{
constexpr float ElasticFactor = math::constants::pi2_f / 3.0f;
constexpr float ElasticFactor2 = math::constants::pi2_f / 4.5f;
}
#if defined(__GNUG__) && !defined(__clang__)
GOBLIB_INLINE constexpr float elastic_in(const float t)
#else
GOBLIB_INLINE float elastic_in(const float t)        
#endif
{
    return (t <= 0.0f) ? 0.0f :
        (t >= 1.0f) ? 1.0f :
        -std::pow(2.0f, 10.0f * t - 10.0f) * std::sin((t * 10.0f - 10.75f) * ElasticFactor);
}

#if defined(__GNUG__) && !defined(__clang__)
GOBLIB_INLINE constexpr float elastic_out(const float t)
#else
GOBLIB_INLINE float elastic_out(const float t)
#endif
{
    return (t <= 0.0f) ? 0.0f :
        (t >= 1.0f) ? 1.0f :
        std::pow(2.0f, -10.0f * t) * std::sin((t * 10.0f - 0.75f) * ElasticFactor) + 1.0f;
}

#if defined(__GNUG__) && !defined(__clang__)
GOBLIB_INLINE constexpr float elastic_inout(const float t)
#else
GOBLIB_INLINE float elastic_inout(const float t)
#endif
{
    return (t <= 0.0f) ? 0.0f :
        (t >= 1.0f) ? 1.0f :
        t < 0.5f ?
        -0.5f * (std::pow(2.0f, 20.0f * t - 10.0f) * std::sin((20.0f * t - 11.125f) * ElasticFactor2)) :
        0.5f * (std::pow(2.0f, -20.0f * t + 10.0f) * std::sin((20.0f * t - 11.125f) * ElasticFactor2)) + 1.0f;
}

namespace
{
constexpr float BounceFactor = 2.75f;
constexpr float BounceFactor2 = 7.5625f;
}
GOBLIB_INLINE constexpr float bounce_out(const float t)
{
    return t < (1.0f / BounceFactor) ? BounceFactor2 * t * t :
        t < (2.0f / BounceFactor) ? BounceFactor2 * (t - (1.5f / BounceFactor)) * (t - (1.5f / BounceFactor)) + 0.75f :
        t < (2.5f / BounceFactor) ? BounceFactor2 * (t - (2.25f / BounceFactor)) * (t- (2.25f / BounceFactor)) + 0.9375f :
        BounceFactor2 * (t - (2.625f / BounceFactor)) * (t - (2.625f / BounceFactor)) + 0.984375f;
}

GOBLIB_INLINE constexpr float bounce_in(const float t)
{
    return 1.0f - bounce_out(1.0f - t);
}

GOBLIB_INLINE constexpr float bounce_inout(const float t)
{
    return t < 0.5f ? (1.0f - bounce_out(1.0f - 2.0f * t)) * 0.5f
            :  (1.0f + bounce_out(2.0f * t - 1.0f)) * 0.5f;
}
/// @}


// Functor structures
struct easing_linear            { GOBLIB_INLINE constexpr float operator()(const float t) const { return linear(t); } };

struct easing_quardratic_in     { GOBLIB_INLINE constexpr float operator()(const float t) const { return quadratic_in(t); } };
struct easing_quardratic_out    { GOBLIB_INLINE constexpr float operator()(const float t) const { return quadratic_out(t); } };
struct easing_quardratic_inout  { GOBLIB_INLINE constexpr float operator()(const float t) const { return quadratic_inout(t); } };

struct easing_cubic_in          { GOBLIB_INLINE constexpr float operator()(const float t) const { return cubic_in(t); } };
struct easing_cubic_out         { GOBLIB_INLINE constexpr float operator()(const float t) const { return cubic_out(t); } };
struct easing_cubic_inout       { GOBLIB_INLINE constexpr float operator()(const float t) const { return cubic_inout(t); } };

struct easing_quartic_in        { GOBLIB_INLINE constexpr float operator()(const float t) const { return quartic_in(t); } };
struct easing_quartic_out       { GOBLIB_INLINE constexpr float operator()(const float t) const { return quartic_out(t); } };
struct easing_quartic_inout     { GOBLIB_INLINE constexpr float operator()(const float t) const { return quartic_inout(t); } };

struct easing_quintic_in        { GOBLIB_INLINE constexpr float operator()(const float t) const { return quintic_in(t); } };
struct easing_quintic_out       { GOBLIB_INLINE constexpr float operator()(const float t) const { return quintic_out(t); } };
struct easing_quintic_inout     { GOBLIB_INLINE constexpr float operator()(const float t) const { return quintic_inout(t); } };

#if defined(__GNUG__) && !defined(__clang__)
struct easing_sinusoidal_in     { GOBLIB_INLINE constexpr float operator()(const float t) const { return sinusoidal_in(t); } };
struct easing_sinusoidal_out    { GOBLIB_INLINE constexpr float operator()(const float t) const { return sinusoidal_out(t); } };
struct easing_sinusoidal_inout  { GOBLIB_INLINE constexpr float operator()(const float t) const { return sinusoidal_inout(t); } };
#else
struct easing_sinusoidal_in     { GOBLIB_INLINE float operator()(const float t) const { return sinusoidal_in(t); } };
struct easing_sinusoidal_out    { GOBLIB_INLINE float operator()(const float t) const { return sinusoidal_out(t); } };
struct easing_sinusoidal_inout  { GOBLIB_INLINE float operator()(const float t) const { return sinusoidal_inout(t); } };
#endif

#if defined(__GNUG__) && !defined(__clang__)
struct easing_exponential_in    { GOBLIB_INLINE constexpr float operator()(const float t) const { return exponential_in(t); } };
struct easing_exponential_out   { GOBLIB_INLINE constexpr float operator()(const float t) const { return exponential_out(t); } };
struct easing_exponential_inout { GOBLIB_INLINE constexpr float operator()(const float t) const { return exponential_inout(t); } };
#else
struct easing_exponential_in    { GOBLIB_INLINE float operator()(const float t) const { return exponential_in(t); } };
struct easing_exponential_out   { GOBLIB_INLINE float operator()(const float t) const { return exponential_out(t); } };
struct easing_exponential_inout { GOBLIB_INLINE float operator()(const float t) const { return exponential_inout(t); } };
#endif

#if defined(__GNUG__) && !defined(__clang__)
struct easing_circular_in       { GOBLIB_INLINE constexpr float operator()(const float t) const { return circular_in(t); } };
struct easing_circular_out      { GOBLIB_INLINE constexpr float operator()(const float t) const { return circular_out(t); } };
struct easing_circular_inout    { GOBLIB_INLINE constexpr float operator()(const float t) const { return circular_inout(t); } };
#else
struct easing_circular_in       { GOBLIB_INLINE float operator()(const float t) const { return circular_in(t); } };
struct easing_circular_out      { GOBLIB_INLINE float operator()(const float t) const { return circular_out(t); } };
struct easing_circular_inout    { GOBLIB_INLINE float operator()(const float t) const { return circular_inout(t); } };
#endif

struct easing_back_in           { GOBLIB_INLINE constexpr float operator()(const float t) const { return back_in(t); } };
struct easing_back_out          { GOBLIB_INLINE constexpr float operator()(const float t) const { return back_out(t); } };
struct easing_back_inout        { GOBLIB_INLINE constexpr float operator()(const float t) const { return back_inout(t); } };

#if defined(__GNUG__) && !defined(__clang__)
struct easing_elastic_in        { GOBLIB_INLINE constexpr float operator()(const float t) const { return elastic_in(t); } };
struct easing_elastic_out       { GOBLIB_INLINE constexpr float operator()(const float t) const { return elastic_out(t); } };
struct easing_elastic_inout     { GOBLIB_INLINE constexpr float operator()(const float t) const { return elastic_inout(t); } };
#else
struct easing_elastic_in        { GOBLIB_INLINE float operator()(const float t) const { return elastic_in(t); } };
struct easing_elastic_out       { GOBLIB_INLINE float operator()(const float t) const { return elastic_out(t); } };
struct easing_elastic_inout     { GOBLIB_INLINE float operator()(const float t) const { return elastic_inout(t); } };
#endif

struct easing_bounce_in         { GOBLIB_INLINE constexpr float operator()(const float t) const { return bounce_in(t); } };
struct easing_bounce_out        { GOBLIB_INLINE constexpr float operator()(const float t) const { return bounce_out(t); } };
struct easing_bounce_inout      { GOBLIB_INLINE constexpr float operator()(const float t) const { return bounce_inout(t); } };


/*!
  @brief Easing wrapper
  @tparam T type of value.
  @tparam EasingFunctor Easing functor structure.
 */
template<class T, class EasingFunctor> class Easing
{
    static_assert(goblib::template_helper::is_callable< EasingFunctor, const float>::value,"Unable to call EasingFunctor");
    static_assert(goblib::template_helper::is_return_type< float, EasingFunctor, const float>::value, "EasingFunctor must be return float");

  public:
    explicit Easing(const T& cur = 0) : _behavior(), _current(cur), _from(0), _to(0), _count(0), _times(0) {}

    /// @name Property
    /// @{
    GOBLIB_INLINE operator T() const { return _current; }
    GOBLIB_INLINE T value() const { return _current; }
    //GOBLIB_INLINE T value(const float t) const { return _from + (_to - _from) * _behavior(t); }
    GOBLIB_INLINE T value(const float t) const { return (_from * (1.0f - _behavior(t))) + (_to * _behavior(t)); }
    GOBLIB_INLINE bool busy() const { return _times; }
    /// @}

    /*!
      Start easing
      @param from Value of start
      @param to Value of end
      @param times Number of call pump() required to change from [from] to [to].
     */
    void start(const T& from, const T& to, const std::uint32_t& times)
    {
    _current = _from = from;
    _to = to;
    _times = times;
    _count = 0;
    }
    /*!
      Start easing from current value.
      @param to Value of end
      @param times Number of call pump() required to change from [from] to [to].
     */
    void start(const T& to, const std::uint32_t& times)
    {
    start(_current, to, times);
    }

    /*! change current value */
    void pump()
    {
    if(!_times) { return; }
    
    _current = value(static_cast<float>(++_count)/_times);

    if(_count >= _times)
    {
        _current = value(1.0f);
        _count = _times = 0;
    }
    }
    
  private:
    EasingFunctor _behavior;
    T _current, _from, _to;
    std::uint32_t _count, _times;
};

// Easing wrapper classes
template<typename T> class EaseLerp         : public Easing<T, easing::easing_linear>{};

template<typename T> class EaseInQuad       : public Easing<T, easing::easing_quardratic_in>{};
template<typename T> class EaseOutQuad      : public Easing<T, easing::easing_quardratic_out>{};
template<typename T> class EaseInOutQuad    : public Easing<T, easing::easing_quardratic_inout>{};

template<typename T> class EaseInCubic      : public Easing<T, easing::easing_cubic_in>{};
template<typename T> class EaseOutCubic     : public Easing<T, easing::easing_cubic_out>{};
template<typename T> class EaseInOutCubic   : public Easing<T, easing::easing_cubic_inout>{};

template<typename T> class EaseInQuart      : public Easing<T, easing::easing_quartic_in>{};
template<typename T> class EaseOutQuart     : public Easing<T, easing::easing_quartic_out>{};
template<typename T> class EaseInOutQuart   : public Easing<T, easing::easing_quartic_inout>{};

template<typename T> class EaseInQuint      : public Easing<T, easing::easing_quintic_in>{};
template<typename T> class EaseOutQuint     : public Easing<T, easing::easing_quintic_out>{};
template<typename T> class EaseInOutQuint   : public Easing<T, easing::easing_quintic_inout>{};

template<typename T> class EaseInSine       : public Easing<T, easing::easing_sinusoidal_in>{};
template<typename T> class EaseOutSine      : public Easing<T, easing::easing_sinusoidal_out>{};
template<typename T> class EaseInOutSine    : public Easing<T, easing::easing_sinusoidal_inout>{};

template<typename T> class EaseInExpo       : public Easing<T, easing::easing_exponential_in>{};
template<typename T> class EaseOutExpo      : public Easing<T, easing::easing_exponential_out>{};
template<typename T> class EaseInOutExpo    : public Easing<T, easing::easing_exponential_inout>{};

template<typename T> class EaseInCirc       : public Easing<T, easing::easing_circular_in>{};
template<typename T> class EaseOutCirc      : public Easing<T, easing::easing_circular_out>{};
template<typename T> class EaseInOutCirc    : public Easing<T, easing::easing_cubic_inout>{};

template<typename T> class EaseInBack       : public Easing<T, easing::easing_back_in>{};
template<typename T> class EaseOutBack      : public Easing<T, easing::easing_back_out>{};
template<typename T> class EaseInOutBack    : public Easing<T, easing::easing_back_inout>{};

template<typename T> class EaseInElastic    : public Easing<T, easing::easing_elastic_in>{};
template<typename T> class EaseOutElastic   : public Easing<T, easing::easing_elastic_out>{};
template<typename T> class EaseInOutElastic : public Easing<T, easing::easing_elastic_inout>{};

template<typename T> class EaseInBounce     : public Easing<T, easing::easing_bounce_in>{};
template<typename T> class EaseOutBounce    : public Easing<T, easing::easing_bounce_out>{};
template<typename T> class EaseInOutBounce  : public Easing<T, easing::easing_bounce_inout>{};

//
}}
#endif
