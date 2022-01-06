/*!
  Goblin Library

  @file   gob_singleton.hpp
  @brief  Singleton base class.
*/
#pragma once
#ifndef GOBLIB_SINGLETON_HPP
#define GOBLIB_SINGLETON_HPP

#include <memory> // std::unique_ptr
#include <gob_macro.hpp>

namespace goblib
{

/*!
  @class Singleton
  @brief Singleton base class.
  @ref example_singleton.cpp "see also example"
  @example example_singleton.cpp
*/
template <class T> class Singleton
{
    using PointerType = std::unique_ptr<T>;

  public :
    /*! @brief Get instance. */
    static T& instance()
    {
        static typename T::PointerType _instance(T::create());
        return get(_instance);
    }

  protected:
    GOBLIB_INLINE static T *create() { return new T() ; }
    GOBLIB_INLINE static  T& get(const PointerType &ptr) { return *ptr; }

    Singleton() = default;
    virtual ~Singleton(){}

  private:
    // Not copybye, Not movable.
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(const Singleton&&) = delete;
    Singleton& operator=(const Singleton&&) = delete;
};

//
}
#endif
