/*!
  Goblin Library

  @file   gob_animation.hpp
  @brief  Animation sequence.
*/
#ifndef GOBLIB_ANIMATION_HPP
#define GOBLIB_ANIMATION_HPP

#include <cstdint>
#include <cstddef>
#include <vector>
#include <stack>
#include <initializer_list>
#include <cassert>
#include <gob_macro.hpp>
#include <gob_fixed_vector.hpp>

namespace goblib
{
/*! @brief Graphics */
namespace graph
{

/*! 
  Sequence
  @brief Command sequence for AnimationSequence
*/
struct Sequence
{
    /*!
      @enum Command
    */
    enum : std::uint8_t
    {
        Nop,    //!< No operation
        Draw,   //!< Draw cell (params(u8): cell, frames, flipH, flipV)
        //
        Offset, //!< Offset cell position (params(s16): offsetx, offsety)
        Goto,   //!< Goto sequence index (params(u8): index)
        LoopS,  //!< Loop block start. (params(u8): loop count.)
        LoopE,  //!< Loop block end.
        Callback, //!< Call callabck function.
    };
    
    constexpr explicit Sequence(std::uint8_t cmd = Nop) : command(cmd), _pad{0,},u32{0} {}

    constexpr Sequence(std::uint8_t cmd, const std::uint8_t p0, const std::uint8_t p1 = 0, const std::uint8_t p2 = 0, const std::uint8_t p3 = 0)
            : command(cmd) ,_pad{0,}
            , u8{ p0, p1, p2, p3 }
    {}
    constexpr Sequence(std::uint8_t cmd, const std::int8_t p0, const std::int8_t p1 = 0, const std::int8_t p2 = 0, const std::int8_t p3 = 0)
            : command(cmd), _pad{0,}
            , i8{ p0, p1, p2, p3 }
    {}
    constexpr Sequence(std::uint8_t cmd, const std::uint16_t p0, const std::uint16_t p1 = 0)
            : command(cmd), _pad{0,}
            , u16{ p0, p1 }
    {}
    constexpr Sequence(std::uint8_t cmd, const std::int16_t p0, const std::int16_t p1 = 0)
            : command(cmd), _pad{0}
            , i16{ p0, p1 }
    {}
    constexpr Sequence(std::uint8_t cmd, const std::uint32_t p0)
            : command(cmd), _pad{0,}
            , u32{ p0 }
    {}
    constexpr Sequence(std::uint8_t cmd, const std::int32_t p0)
            : command(cmd), _pad{0}
            , i32{ p0 }
    {}
    
    std::uint8_t command;
    std::uint8_t _pad[3];
    union
    {
        std::uint8_t  u8[4];
        std::int8_t   i8[4];
        std::uint16_t u16[2];
        std::int16_t  i16[2];
        std::uint32_t u32[1];
        std::int32_t  i32[1];
    };

    GOBLIB_INLINE  void clear() { command = Nop; u32[0] = 0; }
};


/*!
  @brief Manage for sequences.
*/
class AnimationSequencer
{
  public:
    using Callback = void(*)(void*, std::uint8_t);
    using Stack = std::stack<Sequence, std::vector<Sequence> >;
    constexpr static std::size_t SEQUENCE_MAX = 255;
    
    AnimationSequencer()
            : _sequences()
            , _current(0)
            , _prev(0)
            , _pause(false)
            , _cell(0)
            , _ox(0)
            , _oy(0)
            , _flipH(false)
            , _flipV(false)
            , _callback(nullptr)
            , _arg(nullptr)
    {}
    AnimationSequencer(std::initializer_list<Sequence> init)
            : _sequences(init)
            , _current(0)
            , _prev(0)
            , _pause(false)
            , _cell(0)
            , _ox(0)
            , _oy(0)
            , _flipH(false)
            , _flipV(false)
            , _cs{_sequences[0]}
            , _callback(nullptr)
            , _arg(nullptr)
    {}

    AnimationSequencer(const Sequence* s, std::size_t len)
            : _sequences(len)
            , _current(0)
            , _prev(0)
            , _pause(false)
            , _cell(0)
            , _ox(0)
            , _oy(0)
            , _flipH(false)
            , _flipV(false)
            , _callback(nullptr)
            , _arg(nullptr)
    { setSequences(s, len); }

    AnimationSequencer(const std::vector<Sequence>& v)
            : _sequences(v.begin(), v.end())
            , _current(0)
            , _prev(0)
            , _pause(false)
            , _cell(0)
            , _ox(0)
            , _oy(0)
            , _flipH(false)
            , _flipV(false)
            , _cs{_sequences[0]}
            , _callback(nullptr)
            , _arg(nullptr)
    {}
    AnimationSequencer(const AnimationSequencer&) = default;
    AnimationSequencer& operator=(const AnimationSequencer&) = default;

    void setSequences(const Sequence* s, std::size_t len)
    {
        assert(len <= SEQUENCE_MAX && "Size over");
        assert(len <= _sequences.capacity() && "Expand memory");

        _sequences.clear();
        while(len--) { _sequences.push_back(*s++); }
        _current = _prev = _cell = 0;
        _cs = _sequences[_current];
    }

    GOBLIB_INLINE void setCallback(Callback f, void* arg)
    {
        _callback = f;
        _arg = arg;
    }

    /// @name Propery
    /// @{
    GOBLIB_INLINE std::size_t size() const { return _sequences.size(); }
    std::size_t stepSize() const
    {
        std::size_t sz = 0;
        for(auto& e : _sequences)
        {
            sz += e.command == Sequence::Draw ? e.u8[1] : 0;
        }
        return sz;
    }

    GOBLIB_INLINE std::uint8_t cell() const { return _cell;  }
    GOBLIB_INLINE std::int16_t offsetX() const { return _ox; }
    GOBLIB_INLINE std::int16_t offsetY() const { return _oy; }
    GOBLIB_INLINE std::uint8_t index() const { return _current; }

    GOBLIB_INLINE bool isFlipH() const { return _flipH; }
    GOBLIB_INLINE bool isFlipV() const { return _flipV; }
    GOBLIB_INLINE bool isFinish() const { return _current >= _sequences.size(); }
    GOBLIB_INLINE bool isPause() const  { return _pause; }
    /// @}
    
    void reset()
    {
        _current = _prev = 0;
        _cs.clear();
        if(!_sequences.empty()) { _cs = _sequences[0]; }

        _cell = 0;
        _ox = _oy = 0;
        _flipH = _flipV = false;
        Stack empty;
        _stack.swap(empty); // same as clear stack.
    }

    GOBLIB_INLINE void pause(bool b) { _pause = b; }
    
    bool pump();

  private:
    AnimationSequencer(AnimationSequencer&&) = delete;
    AnimationSequencer& operator=(AnimationSequencer&&) = delete;
    
  private:
    std::vector<Sequence> _sequences; 
    std::uint8_t _current; // index of sequences (0~254 valid)
    std::uint8_t _prev;
    std::uint8_t _pause;

    std::uint8_t _cell; // cell no
    std::int16_t _ox, _oy; // offset x,y
    bool _flipH, _flipV; // flip horizon/vertical?

    Sequence _cs; // current sequence data.
    Stack _stack; // stack for loop section.

    Callback _callback;
    void* _arg;
};

//
}}
#endif
