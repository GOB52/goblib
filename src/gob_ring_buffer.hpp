/*!
  Goblin Library

  @file   gob_ring_buffer.hpp
  @brief  Ring buffer (also known as circular buffer)
*/
#pragma once
#ifndef GOBLIB_RINGBUFFER_HPP
#define GOBLIB_RINGBUFFER_HPP

#include "gob_macro.hpp"
#include <cstdint>
#include <cstddef>
#include <array>
#include <initializer_list>
#include <type_traits>
#include <cstring>
#include <cassert>
#include <iterator>
#if defined(GOBLIB_CPP17_OR_LATER)
#include <optional>
#endif

namespace goblib
{

/// @cond
template<class RB> class RingBufferIterator
{
  public:
    using value_type = typename RB::value_type;
    using difference_type = std::ptrdiff_t;
    using pointer = typename RB::value_type*;
    using reference = typename RB::reference;
    //    using iterator_category = std::random_access_iterator_tag;
    using iterator_category = std::bidirectional_iterator_tag;

    RingBufferIterator(RB& rb, std::size_t tail) : _rb(&rb), _idx(tail) {}
    RingBufferIterator(const RingBufferIterator& o) = default;
    RingBufferIterator& operator=(const RingBufferIterator& o) = default;
    
    GOBLIB_INLINE typename RB::const_reference operator*() { return this->_rb->_buf[_idx % _rb->capacity()]; }

    GOBLIB_INLINE RingBufferIterator& operator++() { ++_idx; return *this; }
    GOBLIB_INLINE RingBufferIterator& operator--() { --_idx; return *this; }
    GOBLIB_INLINE RingBufferIterator operator++(int) { RingBufferIterator it = *this; ++*this; return it; }
    GOBLIB_INLINE RingBufferIterator operator--(int) { RingBufferIterator it = *this; --*this; return it; }

    GOBLIB_INLINE RingBufferIterator& operator+=(std::size_t n) { _idx += n; return *this; }
    GOBLIB_INLINE RingBufferIterator& operator-=(std::size_t n) { _idx -= n; return *this; }
    GOBLIB_INLINE RingBufferIterator operator+(std::size_t n) const
    {
        return RingBufferIterator(*this) += n;
    }
    GOBLIB_INLINE RingBufferIterator operator-(std::size_t n) const
    {
        return RingBufferIterator(*this) -= n;
    }
    GOBLIB_INLINE std::size_t operator-(const RingBufferIterator& o)
    {
        assert(_rb == o._rb && "Diffrent iterators of container");
        assert(_idx >= o._idx && "o must be lesser than this.");
        return _idx - o._idx;
    }
    
    GOBLIB_INLINE bool operator==(const RingBufferIterator& b) const { return (_rb == b._rb) && _idx == b._idx; }
    GOBLIB_INLINE bool operator<(const RingBufferIterator& b) const { return _idx < b._idx; }
    GOBLIB_INLINE bool operator!=(const RingBufferIterator& b) const { return !(*this == b); }
    GOBLIB_INLINE bool operator>(const RingBufferIterator& b) const { return b < (*this); }
    GOBLIB_INLINE bool operator<=(const RingBufferIterator& b) const { return !(*this > b); }
    GOBLIB_INLINE bool operator>=(const RingBufferIterator& b) const { return !(*this < b); }
    
  private:
    RB* _rb;
    std::size_t _idx;
};
/// @endcond

/*! 
  @brief Ring buffer (also known as circular buffer)
  @note Overwite if buffer is full.
  @tparam T type of data
  @tparam N capacity
  @sa https://en.wikipedia.org/wiki/Circular_buffer
 */
template<typename T, std::size_t N> class RingBuffer
{
    static_assert(N > 0, "Size must be greater than zero.");

  public:
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = const T&;
#if defined(GOBLIB_CPP17_OR_LATER)
    using optional_type = std::optional<T>;
#endif
    friend class RingBufferIterator<RingBuffer>;
    using const_iterator = RingBufferIterator<RingBuffer>;
    
    /// @name Constructor
    /// @{
    RingBuffer() : _buf(), _head(0), _tail(0), _full(false) {}
    RingBuffer(size_type n, const_reference value) : RingBuffer()
    {
        assign(n, value);
    }
    template <class InputIter> RingBuffer(InputIter first, InputIter last) : RingBuffer()
    {
        assign(first, last);
    }
    RingBuffer(std::initializer_list<T> il) : RingBuffer(il.begin(), il.end()) {}

    RingBuffer(const RingBuffer&) = default;
    RingBuffer(RingBuffer&&) = default;
    /// @}
    ~RingBuffer(){}

    /// @name Property
    /// @{
    GOBLIB_NODISCARD GOBLIB_INLINE bool empty() const { return !full() && (_head == _tail); }
    GOBLIB_NODISCARD GOBLIB_INLINE bool full() const { return _full; }
    GOBLIB_INLINE size_type size() const { return full() ? N : (_head >= _tail ? _head - _tail : N + _head - _tail); }
    GOBLIB_INLINE size_type capacity() const { return N; }
    /// @}

    /// @name Assignment
    /// @{
    RingBuffer& operator=(const RingBuffer&) = default;
    RingBuffer& operator=(RingBuffer&&) = default;

    template <class InputIterator> void assign(InputIterator first, InputIterator last)
    {
        clear();
        size_type sz = last - first;
        if(sz > N) { first += (sz - N); }
        auto n = std::min(N, sz);
        while(n--) { push_back(*first++); }
    }
    void assign(size_type n, const_reference v)
    {
        clear();
        n = std::min(N, n);
        while(n--) { push_back(v); }
    }
    GOBLIB_INLINE void assign(std::initializer_list<T> il) { assign(il.begin(), il.end()); }
    /// @}
    
    /// @name I/O
    void clear() { _full = false; _head = _tail; }

    void push_front(const value_type& v)
    {
        _tail = (_tail -1 + N) % N;
        _buf[_tail] = v;
        if(_full) { _head = (_head -1 + N) % N; }
        _full = (_head == _tail);
    }
    void push_back(const value_type& v)
    {
        _buf[_head] = v;
        _head = (_head + 1) % N;
        if(_full) { _tail = (_tail + 1) % N; }
        _full = (_head == _tail);
    }

    GOBLIB_INLINE void pop_front()
    {
        if(!empty())
        {
            _tail = (_tail + 1) % N;
            _full = false;
        }
    }
    GOBLIB_INLINE void pop_back()
    {
        if(!empty())
        {
            _head = (_head - 1 + N) % N;
            _full = false;
        }
    }
    /// @}

    /// @name Access
    /// @{
    /*! 
      @brief Access the first element 
      @warning Return T() if empty.
     */
    GOBLIB_INLINE value_type front() const
    {
        assert(!empty() && "empty buffer");
        return !empty() ? _buf[_tail] : T();
    }
    /*! 
      @brief Access the last element 
      @warning Return T() if empty.
    */
    GOBLIB_INLINE value_type back() const
    {
        assert(!empty() && "empty buffer");
        return !empty() ? _buf[(_head - 1 + N) % N] : T();
    }

    #if 0
    /*! @brief Access specified element (R/W)*/
    GOBLIB_INLINE reference operator[] (size_type i) const&
    {
        assert(size() > 0 && "container empty");
        assert(i < size() && "index overflow");
        return _buf[ (_tail + i) % N ];
    }
    #endif
    
    /*! @brief Access specified element */
    GOBLIB_INLINE const_reference operator[] (size_type i) const&
    {
        assert(size() > 0 && "container empty");
        assert(i < size() && "index overflow");
        return _buf[ (_tail + i) % N ];
    }

#if defined(GOBLIB_CPP17_OR_LATER) || defined(DOXYGEN_PROCESS)
    /*! 
      @brief Access the first element
      @return std::optional<T>
      @warning C++17 or later 
    */
    GOBLIB_INLINE optional_type front_optional() const
    {
        return !empty() ? std::make_optional(_buf[_tail]) : std::nullopt;
    }
    /*! 
      @brief Access the last element
      @return std::optional<T>
      @warning C++17 or later 
    */
    GOBLIB_INLINE optional_type back_optional() const
    {
        return !empty() ? std::make_optional(_buf[(_head - 1 + N) % N]) : std::nullopt;
    }
    /*!
      @brief Access specified element
      @param i Position of the element to return
      @return std::optional<T>
      @warning C++17 or later 
     */
    GOBLIB_INLINE optional_type at_optional(size_type i) const
    {
        return (!empty() && i < size()) ? std::make_optional(_buf[ (_tail + i) % N ]) : std::nullopt;
    }
#endif
    /// @}

    /// @name Iterators
    /// @brief This class has const_iterator only.
    /// @{
    const_iterator begin() noexcept { return const_iterator(*this, _tail); }
    const_iterator end()   noexcept { return const_iterator(*this, _tail + size()); }
    const_iterator cbegin()noexcept { return const_iterator(*this, _tail); }
    const_iterator cend()  noexcept { return const_iterator(*this, _tail + size()); }
    /// @}
    
    /// @name Read,Write
    /// @{
    /*! @brief Read from buffer
      @param outbuf Output buffer
      @param num Max elements of output buffer
      @return Number of output elements
     */
    std::size_t read(value_type* outbuf, const std::size_t num)
    {
        std::size_t sz = std::min(num, size());
        if(sz == 0) { return sz; }
        
        auto src = &_buf[_tail];
        std::size_t elms = std::min(N - _tail, sz);

        std::copy(src, src + elms, outbuf);
        _tail = (_tail + elms) % N;
        _full = false;
        std::size_t ret = elms;

        if(elms < sz)
        {
            outbuf += elms;
            src = &_buf[_tail];
            elms = sz - elms;

            std::copy(src, src + elms, outbuf);

            _tail = (_tail + elms) % N;
            ret += elms;
        }
        return ret;
    }

    /*! @brief Write to buffer
      @param inbuf Input buffer
      @param num Number of elements
      @return Number of input elements
     */
    std::size_t write(const value_type* inbuf, const std::size_t num)
    {
        std::size_t num2 = num;
        if(num2 > N) { inbuf += num2 - N; num2 = N; }
        if(num2 == 0) { return 0; }

        std::size_t sz = std::min(N, num2);
        std::size_t space = capacity() - size();
        auto dst = &_buf[_head];
        std::size_t elms = std::min(N - _head, sz);
        
        std::copy(inbuf, inbuf + elms, dst);
        _head = (_head + elms) % N;
        _full = _full || (elms >= space);
        if(_full) { _tail = _head; }
        std::size_t ret = elms;

        if(elms < sz)
        {
            inbuf += elms;
            elms = sz - elms;
            dst = &_buf[_head];

            std::copy(inbuf, inbuf + elms, dst);
            ret += elms;
            _head = (_head + elms) % N;
            _full = ret >= space;
            if(_full) { _tail = _head; }
        }
        return ret;
    }

    /*!
      @brief Fill buffer
      @param v Value
     */
    void fill(const value_type& v)
    {
        clear();
        _buf.fill(v);
        _full = true;
    }
    /// @}

  private:
    std::array<value_type, N> _buf;
    std::size_t _head, _tail;
    bool _full;
};

//
}
#endif
