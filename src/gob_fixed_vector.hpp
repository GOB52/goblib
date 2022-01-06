/*!
  Goblin Library

  @file   gob_fixed_vector.hpp
  @brief  Fixed vector compatible with std::vector
*/
#pragma once
#ifndef GOBLIB_FIXED_VECTOR_HPP
#define GOBLIB_FIXED_VECTOR_HPP

#include <cstddef>
#include <iterator>
#include <initializer_list>
#include <type_traits>
#include <algorithm>
#include <memory>

#include "gob_macro.hpp"
#include "gob_utility.hpp"

#ifdef min
#undef min
#endif

namespace goblib
{

/*! @brief Fixed size vector.
  No memory expansion to prevent unexpected memory fragmentation.
  Class has method of compatibiliy for std::vector.
  @attention No throw exceptions
  @sa https://en.cppreference.com/w/cpp/container/vector
*/
template<typename T, std::size_t Max = 16>
class FixedVector
{
    static_assert(Max > 0, "Max must be greater than zero");

  public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    /// @name Constructor
    /// @{
    FixedVector() : _data(nullptr), _tail(nullptr)
    {
        _tail = _data = reinterpret_cast<T*>(new std::uint8_t[sizeof(T) * Max]);
    }
    explicit FixedVector(size_type n) : _data(nullptr), _tail(nullptr) { _create(n, T()); }
    FixedVector(size_type n, const T& value) : _data(nullptr), _tail(nullptr) { _create(n, value); }

    template <class InputIter> FixedVector(InputIter first, InputIter last) : _data(nullptr), _tail(nullptr)
    {
        assert(last - first <= Max && "Illegal size");
        auto n = std::min(Max, static_cast<size_type>(last - first));

        _data = reinterpret_cast<T*>(new std::uint8_t[sizeof(T) * Max]);
        assert(_data);
        std::uninitialized_copy_n(first, n, _data);
        _tail = _data + n;
    }
    FixedVector(std::initializer_list<T> il) : FixedVector(il.begin(), il.end()) {}
    FixedVector(const FixedVector& x) : _data(nullptr), _tail(nullptr)
    {
        _create(x.begin(), x.end());
    }
    FixedVector(FixedVector&& x)
#if defined(GOBLIB_CPP17_OR_LATER)
            noexcept
#endif
            : _data(nullptr), _tail(nullptr)
    {
        _data = x._data;
        _tail = x._tail;
        x._data = x._tail = nullptr;
    }
    /// @}

    ~FixedVector()
    {
        clear();
        std::uint8_t* p = reinterpret_cast<std::uint8_t*>(_data);
        goblib::safeDeleteArray(p);
    }

    /// @name Operator=
    /// @{
    FixedVector& operator=(const FixedVector& x)
    {
        if(this != &x)
        {
            clear();
            std::copy(x.begin(), x.end(), begin());
            _tail = begin() + x.size();
        }
        return *this;
    }
    
    FixedVector& operator=(FixedVector&& x)
#if defined(GOBLIB_CPP17_OR_LATER)
            noexcept
#endif
    {
        if(this != &x)
        {
            clear();
            std::uint8_t* p = reinterpret_cast<std::uint8_t*>(_data);
            goblib::safeDeleteArray(p);
        
            _data = x._data;
            _tail = x._tail;
            x._data = x._tail = nullptr;
        }
        return *this;
    }

    FixedVector& operator=(std::initializer_list<T> il)
    {
        assert(il.size() <= Max);
        clear();
        std::copy(il.begin(), il.end(), begin());
        _tail = begin() + il.size();
        return *this;
    }
    /// @}

    /// @name Assign
    /// @{
    template <class InputIterator> void assign(InputIterator first, InputIterator last)
    {
        assert(last - first <= Max && "Illegal size");
        clear();
        auto n = std::min(max_size(), static_cast<size_type>(last - first));
        last = first + n;
        std::copy(first, last, begin());
        _tail = _data + n;
    }
    void assign(size_type n, const T& u)
    {
        assert(n <= Max && "Illegal size");
        clear();
        n = std::min(max_size(), n);
        std::fill(begin(), begin() + n, u);
        _tail = _data + n;
    }
    GOBLIB_INLINE void assign(std::initializer_list<T> il) { assign(il.begin(), il.end()); }
    /// @}
    
    /// @name Element access
    /// @{
    GOBLIB_INLINE reference at(size_type n) &           { return _data[n]; }
    GOBLIB_INLINE const_reference at(size_type n) const&{ return _data[n]; }

    GOBLIB_INLINE reference operator[](size_type n) &   { return _data[n]; }
    GOBLIB_INLINE const_reference operator[](size_type n) const& { return _data[n]; }
    GOBLIB_INLINE T operator[](size_type n) const &&    { return std::move(_data[n]); }

    GOBLIB_INLINE reference front()                     { return *_data; }
    GOBLIB_INLINE const_reference front() const         { return *_data; }
    GOBLIB_INLINE reference back()                      { return *(_tail - 1); }
    GOBLIB_INLINE const_reference back() const          { return *(_tail - 1); }

    GOBLIB_INLINE T* data() noexcept                    { return  _data; }
    GOBLIB_INLINE const T* data() const noexcept        { return _data; }
    /// @}

    /// @name Iterators
    /// @{
    GOBLIB_INLINE iterator begin() noexcept { return _data; }
    GOBLIB_INLINE iterator end() noexcept { return _tail; }
    GOBLIB_INLINE const_iterator begin() const noexcept { return _data; }
    GOBLIB_INLINE const_iterator end() const noexcept { return _tail; }
    GOBLIB_INLINE const_iterator cbegin() const noexcept { return _data; }
    GOBLIB_INLINE const_iterator cend() const noexcept { return _tail; }
    
    GOBLIB_INLINE reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    GOBLIB_INLINE reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    GOBLIB_INLINE const_reverse_iterator rbegin() const noexcept { return reverse_iterator(end()); }
    GOBLIB_INLINE const_reverse_iterator rend() const noexcept { return reverse_iterator(begin()); }
    GOBLIB_INLINE const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }
    GOBLIB_INLINE const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }
    /// @}

    /// @name Capacity
    /// @{
    GOBLIB_INLINE std::size_t size() const noexcept { return _tail - _data; }
    GOBLIB_INLINE std::size_t max_size() const noexcept { return Max; }
    GOBLIB_INLINE std::size_t capacity() const noexcept { return Max; }
    GOBLIB_NODISCARD GOBLIB_INLINE bool empty() const noexcept { return begin() == end(); }
    /// @}

    /// @name NOP
    /// @{
    GOBLIB_INLINE void reserve() { }
    GOBLIB_INLINE void shrink_to_fit(){ }
    /// @}
    
    /// @name  Modifiers
    /// @attention insert/push_back/emplace/emplace_back.
    /// If the number of elements exceeds Max by insertion, it will not be inserted.
    /// @todo Support boolean specialization.
    /// @{
    void clear()
    {
        iterator it = begin();
        while(it != end()) { (*it).~T(); ++it; }
        _tail = _data;
    }

    iterator insert(const_iterator position, const T& x)
    {
        return insert(position, 1, x);
    }
    iterator insert(const_iterator position, T&& x)
    {
        assert(position >= _data && "Out of range");
        assert(position + 1 <= _data + Max && "Out of range");
        assert(position <= _tail && "Out of range");
        if(size() < max_size())
        {
            iterator it = const_cast<iterator>(position);
            std::move_backward(it, it + 1, end() + 1);
            *it = std::move(x);
            ++_tail;
            return it;
        }
        return end(); // Failed to insert;
    }
    iterator insert(const_iterator position, size_type n, const T& x)
    {
        assert(position >= _data && "Out of range");
        assert(position + n <= _data + Max && "Out of range");
        assert(position <= _tail && "Out of range");
        if(size() + n <= max_size())
        {
            iterator it = const_cast<iterator>(position);
            std::move_backward(it, it + n, end() + n);
            std::fill(it, it + n, x);
            _tail += n;
            return it;
        }
        return end(); // Failed to insert;
    }
    template <class InputIterator> iterator insert(const_iterator position, InputIterator first, InputIterator last)
    {
        auto n = last - first;
        assert(position >= _data && "Out of range");
        assert(position + n <= _data + Max && "Out of range");
        assert(position <= _tail && "Out of range");
        if(size() + n <= max_size())
        {
            iterator it = const_cast<iterator>(position);
            std::move_backward(it, it + n, end() + n);
            std::copy(first, last, it);
            _tail += n;
            return it;
        }
        return end(); // Failed to inster
    }
    iterator insert(const_iterator position, std::initializer_list<T> il)
    {
        return insert(position, il.begin(), il.end());
    }

    template <class... Args> iterator emplace(const_iterator position, Args&&... args)
    {
        assert(position >= _data && "Out of range");
        assert(position <= _tail && "Out of range");
        if(size() < max_size())
        {
            iterator it = const_cast<iterator>(position);
            std::move_backward(it, it + 1, end() + 1);
            new (it) T(std::forward<Args>(args)...);
            ++_tail;
            return it;
        }
        return end(); // Failed to emplace
    }
    //template <class... Args> iterator vector<bool>::emplace(const_iterator position, Args&&... args); // C++14
    
    iterator erase(const_iterator position)
    {
        assert(position >= _data && "Out of range");
        assert(position <= _tail && "Out of range");
        iterator it = const_cast<iterator>(position);
        (*it++).~T();
        --_tail;
        return it;
    }

    iterator erase(const_iterator first, const_iterator last)
    {
        assert(first >= _data && "Out of range");
        assert(first <= _tail && "Out of range");
        assert(last >= _data && "Out of range");
        assert(last <= _tail && "Out of range");

        auto n = last - first;
        iterator ft = const_cast<iterator>(first);
        iterator lt = const_cast<iterator>(last);
        while(ft < lt) { (*ft++).~T(); }
        std::move(lt, end(), const_cast<iterator>(first));
        _tail -= n;
        return lt + 1;
    }
    
    void push_back(const T& x)
    {
        assert(size() < max_size() && "size full");
        if(size() < max_size())
        {
            *_tail++ = x;
        }
    }       
    void push_back(T&& x)
    {
        assert(size() < max_size() && "size full");
        if(size() < max_size())
        {
            *_tail++ = std::move(x);
        }
    }

#if defined(GOBLIB_CPP17_OR_LATER)
    template <class... Args> reference emplace_back(Args&&... args)
#else
    template <class... Args> void emplace_back(Args&&... args)
#endif
    {
        assert(size() < max_size() && "size full");
        if(size() < max_size())
        {
            new (_tail++) T(std::forward<Args>(args)...);
#if defined(GOBLIB_CPP17_OR_LATER)
            return back();
#endif
        }
#if defined(GOBLIB_CPP17_OR_LATER)
        return *end(); // Failed to emplace_back
#endif
    }
    //    template <class... Args> void vector<bool>::emplace_back(Args&&... args);        // (2) C++11
    //    template <class... Args> reference vector<bool>::emplace_back(Args&&... args);   // (2) C++17

    void pop_back() { if(!empty()) { (--_tail)->~T(); }}

    void resize(size_type sz)
    {
        resize(sz, T());
    }

    void resize(size_type sz, const T& c)
    {
        assert(sz <= Max && "Illegal size");
        auto n = std::min(Max, sz);
#if defined(GOBLIB_CPP17_OR_LATER)
        /// @Fixme C++17 style
        if(n > size())
        {
            insert(end(), n - size(), c);
        }
        else if( n < size())
        {
            erase(begin() + n, end());
        }
#elif defined(GOBLIB_CPP14_OR_LATER)
        if(n > size())
        {
            insert(end(), n - size(), c);
        }
        else if(n < size())
        {
            auto cnt = size() - n;
            while(cnt-- > 0) { pop_back(); }
        }
#else
        if(n > size())
        {
            insert(end(), n - size(), c);
        }
        else if(n < size())
        {
            erase(begin() + n, end());
        }
#endif
    }   

    void swap(FixedVector& x)
#if defined(GOBLIB_CPP17_OR_LATER)
            noexcept
#endif
    {
        std::swap(_data, x._data);
        std::swap(_tail, x._tail);
    }
    /// @}

  private:
    void _create(size_type n, const T& value)
    {
        assert(n <= Max && "Illegal size");
        n = std::min(n, Max);
        _data = reinterpret_cast<T*>(new std::uint8_t[sizeof(T) * Max]);
        assert(_data);
        _tail = _data + n;
        std::uninitialized_fill_n(_data, n, value);
    }
    void _create(const_iterator beg, const_iterator end)
    {
        auto n = end - beg;
        assert(n <= Max && "Illegal range");
        n = std::min(static_cast<std::size_t>(n), Max);
        _data = reinterpret_cast<T*>(new std::uint8_t[sizeof(T) * Max]);
        assert(_data);
        _tail = _data + n;
        std::uninitialized_copy_n(beg, n, _data);
    }
    
  private:
    iterator _data;
    iterator _tail;

};

/// @name Compare
/// @todo Support C++20 (add <=> delete !=,<,<=,>,>=)
/// @{
template <class T, std::size_t Max>
bool operator==(const FixedVector<T,Max>& x, const FixedVector<T,Max>& y)
{
#if defined(GOBLIB_CPP14_OR_LATER)
    return &x == &y || (x.size() == y.size() && std::equal(x.begin(), x.end(), y.begin(), y.end()));
#else
    return &x == &y || (x.size() == y.size() && std::equal(x.begin(), x.end(), y.begin()));
#endif
}

template <class T, std::size_t Max>
bool operator!=(const FixedVector<T,Max>& x, const FixedVector<T,Max>& y)
{
    return std::rel_ops::operator!=(x,y);
}

template <class T, std::size_t Max>
bool operator<(const FixedVector<T,Max>& x, const FixedVector<T,Max>& y)
{
    return std::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}
    
template <class T, std::size_t Max>
bool operator<=(const FixedVector<T,Max>& x, const FixedVector<T,Max>& y)
{
    return std::rel_ops::operator<=(x,y);
}

template <class T, std::size_t Max>
bool operator>(const FixedVector<T,Max>& x, const FixedVector<T,Max>& y)
{
    return std::rel_ops::operator>(x,y);
}

template <class T, std::size_t Max>
bool operator>=(const FixedVector<T,Max>& x, const FixedVector<T,Max>& y)
{
    return std::rel_ops::operator>=(x,y);
}
/// @}
//
}

namespace std
{
/// @name Specialization for std::swap
/// @todo Support C++20 erase/erase_if
/// @{

#if defined(GOBLIB_CPP17_OR_LATER)
template <class T, std::size_t Max> GOBLIB_INLINE
void swap(goblib::FixedVector<T,Max>& x, goblib::FixedVector<T,Max>& y) noexcept(noexcept(x.swap(y)))
{
    x.swap(y);
}
#else
template <class T, std::size_t Max> GOBLIB_INLINE
void swap(goblib::FixedVector<T,Max>& x, goblib::FixedVector<T,Max>& y)
{
    x.swap(y);
}
#endif
/// @}

//
}

#endif
