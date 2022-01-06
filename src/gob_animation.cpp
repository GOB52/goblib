/*!
  Goblin Library

  @file   gob_animation.cpp
  @brief  Animation sequence.
*/
#include "gob_animation.hpp"

namespace goblib { namespace graph {

bool AnimationSequencer::pump()
{
    if(size() == 0 || isFinish()) { return true; }
    if(isPause()) { return false; }

    bool done = false; 
    do
    {
        switch(_cs.command)
        {
        case Sequence::Draw:
            _cell = _cs.u8[0];
            _flipH = _cs.u8[2];
            _flipV = _cs.u8[3];
            _current += (--_cs.u8[1] == 0);
            done = true;
            break;
        case Sequence::LoopS:
            _cs.u8[1] = _current + 1; // set index for LoopE. [1]
            _stack.push(_cs);
            ++_current;
            break;
        case Sequence::LoopE:
            {
                Sequence lt = _stack.top(); _stack.pop();
                if(--lt.u8[0])
                {
                    _current = lt.u8[1]; // [1]
                    _stack.push(lt);
                }
                else
                {
                    ++_current;
                }
            }
            break;
        case Sequence::Offset:
            _ox = _cs.i16[0];
            _oy = _cs.i16[1];
            ++_current;
            break;
        case Sequence::Callback:
            if(_callback) { _callback(_arg, _current); }
            ++_current;
            break;
        case Sequence::Goto:
            _current = _cs.u8[0];
            break;
        default:
            break;
        }

        if(_prev != _current && _sequences.size())
        {
            _cs = _sequences[_current];
            _prev = _current;
        }
    }
    while(!done && _current < _sequences.size());

    return isFinish();
}

//
}}
