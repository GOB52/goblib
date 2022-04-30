/*!
  Goblin Library

  @file   gob_app.hpp
  @brief  Application class
*/
#pragma once
#ifndef GOBLIB_APP_HPP
#define GOBLIB_APP_HPP

#include <cassert>
#include <chrono>
#include <thread>
#include <ratio>
#include "gob_utility.hpp"
#if defined(GOBLIB_CPP17_OR_EARLIER)
#include "gob_template_helper.hpp"
#endif
#include "gob_macro.hpp"

namespace goblib
{
/*!
  App
  @brief Application base class to run at a fixed FPS.
  @tparam Clock using std::chrono clock. (steady_clock as default)
  @tparam UFPS Number of update() calls per second. (It will be called every frame) 30 as default.
  @tparam FFPS Number of fixedUpdate() calls per second. (Frame-rate independant) 30 as default.
*/
template<class Clock = std::chrono::steady_clock, std::uint32_t UFPS = 30, std::uint32_t FFPS = 30>
class App
{
#if defined(GOBLIB_CPP20_OR_LATER)
    static_assert(std::is_clock<Clock>::value, "Clock must satisfies the Clock requirements");
#else
    static_assert(goblib::template_helper::is_clock<Clock>::value, "Clock must satisfies the Clock requirements");
#endif

  protected:
    using Duration = typename Clock::duration;
    using TimePoint = typename Clock::time_point;
    using UpdateDuration = std::chrono::duration<float, std::ratio<1, UFPS> >;
    using FixedUpdateDuration = std::chrono::duration<float, std::ratio<1, FFPS> >;
    
  public:
    App() : _updateTick(1), _fixedUpdateTick(1), _deltaTime(1), _last(Clock::now()),
            _accumulationTime(FFPS >= UFPS ? (float)FFPS/UFPS : 1.0f), _delta(1.0f), _fps(0), _frames(0)
         
    {}
    virtual ~App(){}

    /// @name Property
    /// @{
    GOBLIB_INLINE float fps() const {return _fps;}
    GOBLIB_INLINE std::uint32_t frames() const { return _frames; }
    GOBLIB_INLINE float delta() const { return _delta; }
    GOBLIB_INLINE Duration deltaTime() const { return _deltaTime; }
    /// @}

    /*! Call in application loop */
    void pump()
    {
        while(_accumulationTime >= _fixedUpdateTick.count())
        {
            fixedUpdate();
            _accumulationTime -= _fixedUpdateTick.count();
        }
        update(_delta);
        render();
        sleep_until(_last + _updateTick);

        auto now = Clock::now();
        _deltaTime = now - _last;
        _last = now;
        _accumulationTime += std::chrono::duration_cast<FixedUpdateDuration>(_deltaTime).count();
        auto ud = std::chrono::duration_cast<UpdateDuration>(_deltaTime).count();
        _delta = goblib::clamp(ud, MIN_DELTA, MAX_DELTA);
        _fps = UFPS / ud;
        ++_frames;
    }

  protected:
    /// @name Override
    /// @{
    /*!  
      Function called fixedfFps times in 1 second.(Frame-rate independant)
    */
    virtual void fixedUpdate() = 0;

    /*!  
      Function called once in pump().(It will be called every frame)
      @param delta Delta time that normalized by FPS.
    */
    virtual void update(float /*delta*/) = 0;

    /*! Function called once after fixedUpdate() and update() in pump() (It will be called every frame) */
    virtual void render() = 0;

    /*! Thread sleep 
      @warning Override as needed, as different hardware has different accuracies and wait times.
    */
    GOBLIB_INLINE virtual void sleep_until(const std::chrono::time_point<Clock, UpdateDuration>& abs_time)
    {
        std::this_thread::sleep_until(abs_time);
    }
    /// @}

  private:
    const UpdateDuration _updateTick;
    const UpdateDuration _fixedUpdateTick;
    Duration _deltaTime;
    TimePoint  _last;
    float _accumulationTime, _delta,_fps;
    std::uint32_t _frames;
    
    constexpr static float MIN_DELTA = 1.0f;
    constexpr static float MAX_DELTA = 4.0f;
};

template<class Clock, std::uint32_t UFPS, std::uint32_t FFPS>
constexpr float App<Clock, UFPS, FFPS>::MIN_DELTA;
template<class Clock, std::uint32_t UFPS, std::uint32_t FFPS>
constexpr float App<Clock, UFPS, FFPS>::MAX_DELTA;

//
}
#endif
