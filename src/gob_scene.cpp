/*!
  Goblin Library

  @file  gob_scene.cpp
  @brief Scene management by task tree
*/
#include "gob_scene.hpp"
#include <algorithm>
#include <cstdio>
#include <cassert>

namespace goblib
{

void SceneTask::pushScene(SceneTask* st)
{
    assert(_manager);
    assert(st);
    _manager->push(st);
}

void SceneTask::popScene()
{
    assert(_manager);
    _manager->pop();
}

SceneManageTask::SceneManageTask(goblib::TaskTree<goblib::Task>& tree, goblib::Task::PriorityType pri, goblib::Task* parent)
        : goblib::Task(pri, "scene manager")
        , _tree(tree), _parent(parent), _stack()
{
    tree.reserveInsertNode(this, parent);
    _stack.reserve(16);
}

void SceneManageTask::push(SceneTask* st)
{
    assert(st);
    assert(st->_manager == nullptr && "st already used by managers");
    
    SceneTask::SceneType pid = 0;
    if(!_stack.empty())
    {
        // No longer current
        auto& cur = _stack.back();
        pid = cur->sceneId();
        cur->pause(true, true); // Pause include children
        cur->onLeaveCurrentScene(st->sceneId());
    }

    // To be current
    _tree.reserveInsertNode(st, this);
    _stack.push_back(st);

    st->_manager = this;
    st->onEnterCurrentScene(pid, false);
    onChangeScene(st->sceneId(), pid);
}

void SceneManageTask::pop()
{
    SceneTask::SceneType pid = 0;
    SceneTask::SceneType cid = 0;
    SceneTask* pre = nullptr;
    SceneTask* cur = nullptr;
    
    if(_stack.empty()) { return; }
    
    pre = _stack.back();
    pid = pre->sceneId();
    _stack.pop_back();

    if(!_stack.empty())
    {
        cur = _stack.back();
        cid = cur->sceneId();
    }

    // Release current
    pre->onLeaveCurrentScene(cid);
    pre->release(true); // Release include children
    pre->_manager = nullptr;
    
    // Resume
    if(cur)
    {
        cur->pause(false, true); // Resume include children
        cur->onEnterCurrentScene(pid, true);
    }
    onChangeScene(cid, pid);
}

bool SceneManageTask::onRelease()
{
    // Release all children.
    release(true); 
    // Wait until children has been killed.
    return std::all_of(_stack.begin(), _stack.end(), [](SceneTask* st) { return st->isKill(); });
}

void SceneManageTask::print()
{
    printf("Scene:%zu\n", _stack.size());
    std::for_each(_stack.rbegin(), _stack.rend(), [this](SceneTask* st)
    {
        printf("%c:[%16s]:%u\n", (this->_stack.back() == st) ? 'C' : ' ', st->tag(), st->sceneId());
    });
}

//
}
