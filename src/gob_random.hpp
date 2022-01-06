/*!
  Goblin Library

  @file   gob_random.hpp
  @brief  Random generation.
*/
#pragma once
#ifndef GOBLIB_RANDOM_HPP
#define GOBLIB_RANDOM_HPP

#include <random>
#include "gob_template_helper.hpp"
#include "gob_macro.hpp"

namespace goblib
{
/*! 
  @brief Wrapper for RNG.
  @tparam Engine pseudo-random number engine. (e.g. std::mt19937)
*/
template<class Engine> class Rng
{
    static_assert(goblib::template_helper::is_rng<Engine>::value, "Engine must be random number engine");

  public:
    /// @name Construction and Seeding
    /// @{
    explicit Rng(typename Engine::result_type seed = Engine::default_seed) : _engine(seed) {}
    Rng(const Rng& o) : _engine(o._engine) {}
    Rng(Rng&& o) : _engine(std::move(o._engine)) {}
    
    /*! Sets the current state of the engine  */
    GOBLIB_INLINE void seed(std::uint32_t seed) { _engine.seed(seed); }
    /// @}

    Rng& operator=(const Rng& o)
    {
        if(this != &o)
        {
            _engine = o._engine;
        }
        return *this;
    }
    Rng& operator=(Rng&& o)
    {
        if(this != &o)
        {
            _engine = std::move(o._engine);
        }
        return *this;
    }
    
    /// @name  Generation
    /// @{
    /*! 
      @brief Produces random values, uniformly distributed on the closed interval [a,b], that is, distributed according to the discrete probability function
      @tparam Type for range and return value.
      @param a Minimum value
      @param b Maximum value
      @return value of type U
    */
    template<typename U, typename std::enable_if< std::is_integral<U>::value, std::nullptr_t>::type = nullptr>
    GOBLIB_INLINE U operator()(const U a, const U b)
    {
        std::uniform_int_distribution<> d(a,b);
        return d(_engine);
    }

    template<typename U, typename std::enable_if< std::is_floating_point<U>::value, std::nullptr_t>::type = nullptr>
    GOBLIB_INLINE U operator()(const U a, const U b)
    {
        std::uniform_real_distribution<> d(a,b);
        return d(_engine);
    }

    /*! @brief Advances the engine's state and returns the generated value */
    GOBLIB_INLINE typename Engine::result_type operator()() { return _engine(); }
    
    /*! Advances the engine's state by a specified amount */
    GOBLIB_INLINE void discard(unsigned long long z)
    {
        while(z--) { (*this)(); }
    }
    /// @}
    
    /// @name Characteristics
    /// @{
    typename Engine::result_type min() const { return Engine::min(); }
    typename Engine::result_type max() const { return Engine::max(); }
    /// @}

  private:
    Engine _engine;
};

}
#endif
