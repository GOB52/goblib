/*!
  Goblin Library

  @file   gob_memory_stream.hpp
  @brief  Memory stream.
*/
#pragma once
#ifndef GOBLIB_MEMORY_STREAM_HPP
#define GOBLIB_MEMORY_STREAM_HPP

#include <gob_stream.hpp>
#include <algorithm> // std::min
#include <cstring> // std::memcpy
#include <gob_utility.hpp> // goblib::clamp
#include <array>
#include <vector>
#include <cassert>

namespace goblib
{

/*!
  @brief Memory stream.
*/
class MemoryStream : public Stream
{
  public:
    using pos_type = typename Stream::pos_type;
    using off_type = typename Stream::off_type;

    static_assert( sizeof(std::size_t) <= sizeof(pos_type), "pos_type is not enough to represent position");
    
    /// @name Constructor
    /// @{
    
    /// @brief Pointer and size
    template<typename U> MemoryStream(U buf, std::size_t sz)
            : Stream(), _len(sz * sizeof(decltype(*buf)) )
            , _head(reinterpret_cast<const std::uint8_t*>(buf))
            , _cur(const_cast<std::uint8_t*>(_head))
            , _tail(_head + _len)
    { static_assert(std::is_pointer<U>::value, "U must be pointer"); }

    /// @brief assign from std::array
    template<typename U, std::size_t N> MemoryStream(std::array<U,N >& a)
            : Stream(), _len( a.size() * sizeof(typename std::array<U,N>::value_type) )
            , _head(reinterpret_cast<std::uint8_t*>(a.data()))
            , _cur(const_cast<std::uint8_t*>(_head))
            , _tail(_head + _len)
    {}

    /// @brief assign from std::vector
    template<typename U> MemoryStream(std::vector<U>& v)
            : Stream(), _len( v.size() * sizeof(typename std::vector<U>::value_type) )
            , _head(reinterpret_cast<std::uint8_t*>(v.data()))
            , _cur(const_cast<std::uint8_t*>(_head))
            , _tail(_head + _len)
    {}
    /// @}

    virtual ~MemoryStream(){}
    
    /// @name Open, Close
    /// @{
    virtual bool is_open() const override { return _cur != nullptr; }
    virtual bool open(const char* /*path*/) override { return false; } // NOP
    virtual void close() override { _cur = nullptr; }
    /// @}

    /// @name Property
    /// @{
    virtual pos_type size() const { return _tail - _head; }
    /// @}
    
    /// @name Read
    /// @{
    template<typename U> pos_type read(U buf, std::size_t len)
    {
        return read(reinterpret_cast<std::uint8_t*>(buf), len);
    }
    virtual pos_type read(std::uint8_t* buf, std::size_t len) override
    {
        if(is_tail() || len == 0) { return 0; }
        auto clen = std::min(static_cast<pos_type>(_tail - _cur), static_cast<pos_type>(len));
        std::memcpy(buf, _cur, clen);
        _cur += clen;
        return clen;
    }
    /// @}

    /// @name Seek
    /// @{
    virtual bool seek(off_type off, seekdir s) override
    {
        switch(s)
        {
        case seekdir::beg:
            if(off < 0 || off > _tail - _head) { return false; }
            _cur = const_cast<std::uint8_t*>(_head + off);
            break;
        case seekdir::cur:
            if(off < -(_cur - _head) || off > _tail - _cur) { return false; }
            _cur += off;
            break;
        case seekdir::end:
            if(off > 0 || off < -(_tail - _head)) { return false; }
            _cur = const_cast<std::uint8_t*>(_tail + off);
            break;
        }
        assert(_cur >= _head && "invalid position _cur < _head");
        assert(_cur <= _tail && "invalid position _cur > _tail");
        return true;
    }
    virtual pos_type position() const override { return static_cast<pos_type>(_cur - _head); }
    virtual bool is_tail() const override { return _cur >= _tail; }
    /// @}
    
  private:
    const pos_type _len;
    const std::uint8_t* const _head;
    std::uint8_t* _cur;
    const std::uint8_t* const _tail;
};

}
#endif

