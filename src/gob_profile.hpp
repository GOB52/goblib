/*!
  Goblin Library

  @file   gob_profile.hpp
  @brief  Simple profiler.
  @attention To use them, you need to define GOBLIB_ENABLE_PROFILE.
*/
#pragma once
#ifndef GOBLIB_PROFILE_HPP
#define GOBLIB_PROFILE_HPP

#if defined(GOBLIB_ENABLE_PROFILE)

#include <chrono> // std::chrono
#include <cstring> // std::strncpy
#include "gob_macro.hpp"
#include "gob_utility.hpp" // for goblib::size
#include "gob_template_helper.hpp"
#include <cstdio>

namespace goblib
{

/*!
  @brief Class and macro for simple profiling. 
  @attention To use them, you need to define GOBLIB_ENABLE_PROFILE.
*/
namespace profile
{

/*!
  @brief Measuring instance survival time.
  @tparam Clock any clock class in std::chrono.(e.g. std::chrono::steady_clock)
  @tparam Duration Unit of elapsed time. (e.g. std::chrono::milliseconds)
  @attention To use them, you need to define GOBLIB_ENABLE_PROFILE.
*/
template<class Clock, class Duration> class MeasuringInstrument
{
#if defined(GOBLIB_CPP20_OR_LATER)
    static_assert(std::chrono::is_clock<Clock>::value, "Clock must be any clock class in std::chrono.");
#else
    static_assert(goblib::template_helper::is_clock<Clock>::value, "Clock must be any clock class in std::chrono.");
#endif
    template <class T> struct is_duration : std::false_type {};
    template <class Rep, class Period> struct is_duration<std::chrono::duration<Rep, Period>> : std::true_type {};

    static_assert(is_duration<Duration>::value, "Duration must be std::chrono::duration.");

    using TimePoint = std::chrono::time_point<Clock>;
    
  public:
    explicit MeasuringInstrument(const char* tag = "", bool prt = true) : _tag{}, _start(Duration(0)), _print(prt)
    {
        std::strncpy(_tag, tag, sizeof(_tag));
        _tag[goblib::size(_tag) - 1] = '\0';
        ++_indent;
        _start = Clock::now();
    }
    ~MeasuringInstrument()
    {
        if(_print) { print(elapsed()); }
        --_indent;
    }

    GOBLIB_INLINE const char* tag() const { return _tag; }

    GOBLIB_INLINE Duration elapsed() const
    {
        return std::chrono::duration_cast<Duration>(Clock::now() - _start);
    }

#if defined(GOBLIB_CPP17_OR_LATER)
    constexpr static std::size_t TAG_SIZE = 32;
#else
    enum { TAG_SIZE = 32 };
#endif    
  private:
    char _tag[TAG_SIZE];
    TimePoint  _start;
    bool _print;

  protected:
    GOBLIB_INLINE void print(Duration t)
    {
        printf("%*sProfile:[%s] %lld\n", _indent * 2, "", tag(), t.count());
    }

  private:
    static std::uint32_t _indent;
};
template<class Clock, class Duration>  std::uint32_t MeasuringInstrument<Clock, Duration>::_indent = 0;

/*! @typedef Ordinary
  Ordinary measuring instruments.
*/
using Ordinary = MeasuringInstrument<std::chrono::steady_clock, std::chrono::milliseconds>;
/*! @typedef HighPrecision
  High precision measuring instruments.
*/
using HighPrecision = MeasuringInstrument<std::chrono::high_resolution_clock, std::chrono::nanoseconds>;

/*! Simple profile in scope.*/
#define GOBLIB_SCOPED_PROFILE(tag) goblib::profile::Ordinary GOBLIB_CONCAT(pf_,__LINE__)((#tag))
/*! Simple profile in scope.(High precision) */
#define GOBLIB_SCOPED_PROFILE_HIGH(tag) goblib::profile::HighPrecision GOBLIB_CONCAT(pf_,__LINE__)((#tag))

//
}}

#else // defined(GOBLIB_ENABLE_PROFILE)

#define GOBLIB_SCOPED_PROFILEK(tag) /* Nop */
#define GOBLIB_SCOPED_PROFILE_HIGH(tag) /* Nop */

#endif // defined(GOBLIB_ENABLE_PROFILE)
#endif

