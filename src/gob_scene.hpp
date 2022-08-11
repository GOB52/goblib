/*!
  Goblin Library

  @file  gob_scene.hpp
  @brief Scene management by task tree
*/
#pragma once
#ifndef GOBLIB_SCENE_HPP
#define GOBLIB_SCENE_HPP

#include "gob_task.hpp"
#include <vector>
#include <type_traits>

namespace goblib
{
class SceneManageTask;

/*! @brief Scene must be derivered this */
class SceneTask : public goblib::Task
{
  public:
    using SceneType = std::uint32_t;

    SceneTask(SceneType sid, goblib::Task::PriorityType pri, const char* tag = "" )
            : goblib::Task(pri, tag)
            , _sceneId(sid)
            , _manager(nullptr)
    { assert(sid !=0 && "sid must be not zero"); }
    virtual ~SceneTask() {}

    SceneType sceneId() const { return _sceneId; }

    void pushScene(SceneTask* st);
    void popScene();
    
  protected:    
    /// @name Override if you want detect on change scene.
    /// @{
    /*! @brief Function will be called when to be current scene.
      @param pre Previous current scene.
      @param resume true:Resume false:First time
    */
    virtual void onEnterCurrentScene(SceneType pre, bool resume){(void)pre, (void)resume; }
    /*! @brief Function will be called when to be no longer current scene.
      @param cur SceneType that to be current.
    */
    virtual void onLeaveCurrentScene(SceneType cur){ (void)cur; }
    /// @}
 
  private:
    const SceneType _sceneId;
    SceneManageTask* _manager;
    
    friend class SceneManageTask;
};


/*! @brief Managerment for SceneTask
  SceneTask is child of this.
*/
class SceneManageTask : public goblib::Task
{
  public:
    SceneManageTask(goblib::TaskTree<goblib::Task>& tree, goblib::Task::PriorityType pri, goblib::Task* parent = nullptr);
    virtual~ SceneManageTask(){}

    goblib::Task* parent() { return _parent; }
    
    void push(SceneTask* st);
    void pop();

    void print();
    
  protected:
    virtual bool onRelease() override;
    /*! @brief Called when scene has been changed. */
    virtual void onChangeScene(SceneTask::SceneType to, SceneTask::SceneType from){ (void)to, (void)from; }

  private:
    goblib::TaskTree<goblib::Task>& _tree;
    goblib::Task* _parent;
    std::vector<SceneTask*> _stack; // back() is current scene
};

//
}
#endif
