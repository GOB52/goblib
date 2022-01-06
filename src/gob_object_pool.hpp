/*!
  Goblin Library

  @file  gob_object_pool.hpp
  @brief Simple object pool
*/
#pragma once
#ifndef GOBLIB_OBJECT_POOL_HPP
#define GOBLIB_OBJECT_POOL_HPP

#include "gob_macro.hpp"
#include <cstddef>
#include <array>
#include <stack>
#include <initializer_list>
#include <type_traits>
#include <cassert>

namespace goblib
{

/*! 
  @brief Object pool
  @tparam T Type of object
  @tparam Max Max elements
*/
template<class T> class ObjectPool
{
  public:
    /// @name Constructor
    /// @{
    ObjectPool(std::size_t size) : _pool(nullptr), _size(size), _stack()
    {
        assert(size > 0);
        _pool = reinterpret_cast<T*>( new std::uint8_t[ sizeof(T) * _size ]);
        assert(_pool);

        for(std::size_t i = 0; i < _size; ++i)
        {
            push(_pool + i);
        }
        assert(_size == _stack.size() && "illegl size of stack");
    }
    /// @}
    ~ObjectPool()
    {
        if(_pool)
        {
            delete[] reinterpret_cast<std::uint8_t*>(_pool);
        }
    }

    /// @name Property
    /// @{
    /*! @brief Capacity of pool */
    GOBLIB_INLINE std::size_t size() const { return _size; }
    /*! @brief Number of available objects in pool. */
    GOBLIB_INLINE std::size_t available() const { return _stack.size(); }
    /*! @brief Empty pool? */
    GOBLIB_INLINE bool empty() const { return _stack.empty(); }
    /// @}

    /// @name Construct/Destruct
    /// @{
    /*! 
      @brief Get and construct
      @param params Construct parameters
      @retval != nullptr Constructed object pointer.
      @retval == nullptr Pool is empty.
      @code
      class A { 
      public:
      A(){}; 
      A(int a, const char* b){} 
      };
      ObjectPool<A,8> pool;
      auto p = pool.construct(1,"construct params"); // call A(int,const char*)
      @endcode
    */
    template<class... Params> GOBLIB_NODISCARD T* construct(Params&&... params)
    {
        if(empty()) { return nullptr; }
        T* ptr = top();
        pop();
        new (ptr) T(std::forward<Params>(params)...); // placement new idiom.
        return ptr;
    }
    /*!
      @brief Return to pool and destruct.
      @param ptr pointer of object
    */
    void destruct(T* ptr)
    {
        if(ptr)
        {
            ptr->~T(); // call destructor of T.
            push(ptr);
        }
    }
    /// @}

  private:
    /// @name Operation
    /// @{
    GOBLIB_INLINE T* top() { return _stack.empty() ? nullptr : _stack.top(); }
    GOBLIB_INLINE void pop() { _stack.pop(); }
    GOBLIB_INLINE void push(T* p)
    {
        assert(p && "ptr is null");
        assert(_stack.size() <= _size && "pool is full");
        assert( p >= _pool && "Illegal pointer:l");
        assert( p < _pool + _size && "Illegal pointer:g");
        _stack.push(p);
    }
    /// @}
    
  private:
    T* _pool;
    const std::size_t _size;
    std::stack<T*> _stack;  // pointer of entity
};

//
}
#endif
