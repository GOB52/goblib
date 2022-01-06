/*!
  Goblin Library

  @file   gob_stream.hpp
  @brief  Stream base class.
*/
#pragma once
#ifndef GOBLIB_STREAM_HPP
#define GOBLIB_STREAM_HPP

#include <cstdint>
#include <cstddef>
#include <type_traits> // std::make_signed

namespace goblib
{

/*! @brief enum type for specifying the starting point of a seek.*/
enum seekdir : std::int_fast8_t
{
    beg,    //!< Based on the [beg]inning of the stream.
    cur,    //!< Based on the [cur]rent position of the stream.
    end,    //!< Based on the [end] of the stream.
};

/*!
  @brief stream base
  @tparam type of stream position. (std::uint64_t as default)
*/
class Stream
{
  public:
    using pos_type = std::uint64_t;
    using off_type = typename std::make_signed<pos_type>::type;
    
    Stream() = default;
    virtual ~Stream() = 0;

    /// @name Open,Close
    /// @{
    virtual bool is_open() const = 0;
    virtual bool open(const char* path) = 0;
    virtual void close() = 0;
    /// @}

    /// @name Property
    /// @{
    virtual pos_type size() const = 0;
    /// @}
    
    /// @name Read
    /// @{
    template<typename U> pos_type read(U buf, std::size_t len)
    {
        return read(reinterpret_cast<std::uint8_t*>(buf), len);
    }
    virtual pos_type read(std::uint8_t* buf, std::size_t len) = 0;

    std::uint8_t read8()
    {
        std::uint8_t b8;
        read(&b8, 1);
        return b8;
    }
    std::uint16_t read16()
    {
        std::uint16_t b16;
        read(reinterpret_cast<std::uint8_t*>(&b16), 2);
        return b16;
    }
    std::uint32_t read32()
    {
        std::uint32_t b32;
        read(reinterpret_cast<std::uint8_t*>(&b32), 4);
        return b32;
    }
    /// @}

    /// @name Seek
    /// @{
    virtual bool seek(off_type off, seekdir s) = 0;
    bool seek(pos_type pos) { return seek(static_cast<off_type>(pos), beg); }
    virtual pos_type position() const = 0;
    virtual bool is_tail() const = 0;
    /// @}
    
  private:
    // Not copyable.
    Stream(const Stream&) = delete;
    Stream& operator=(const Stream&) = delete;
};

//
}
#endif
