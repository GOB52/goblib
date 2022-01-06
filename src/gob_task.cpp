/*!
  Goblin Library

  @file  gob_task.cpp
  @brief Task system.
  @warning This is NOT THREAD.
*/

#include "gob_task.hpp"
#include <cstdio>
#include <cstring>

namespace goblib
{

Task::Task(PriorityType pri, const char* tag)
        : goblib::Node()
        , _tag("")
        , _priority(pri), _status(Status::Initialize)
{
    std::strncpy(_tag, tag, sizeof(_tag) - 1);
    _tag[sizeof(_tag) - 1] = '\0';
}

void Task::pump(const float delta)
{
    if(isKill()) { return; }

    switch(status() & MASK_STATUS)
    {
    case Status::Execute:
        if(!isPause()) {onExecute(delta); }
        break;

    case Status::Restart:
        if(!onRelease())
        {
            break;
        }
        _status = (_status & MASK_FLAG) | Status::Initialize;
        GOBLIB_FALLTHROUGH;
    case Status::Initialize:
        if(onInitialize())
        {
            _status = (_status & MASK_FLAG) | Status::Execute;
        }
        break;
    case Release:
        if(onRelease())
        {
            _status = Status::Kill;
        }
        break;
    }
}

//
}
