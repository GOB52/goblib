/*!
  Goblin Library

  @file  gob_task.hpp
  @brief Task system.
  @warning This is NOT THREAD.
*/
#pragma once
#ifndef GOBLIB_TASK_HPP
#define GOBLIB_TASK_HPP

#include "gob_macro.hpp"
#include "gob_tree.hpp"
#include <vector>
#include <cstdio>

namespace goblib
{
class Task;
template<class T> class TaskTree;

/*!
  @brief Message
*/
struct TaskMessage
{
    std::uint32_t msg;
    void* arg;

    TaskMessage() : msg(0), arg(nullptr), target(nullptr) {}
    explicit TaskMessage(std::uint32_t m) : msg(m), arg(nullptr), target(nullptr) {}
    template<typename U> TaskMessage(std::uint32_t m, U val) : msg(m), arg(reinterpret_cast<void*>(val)), target(nullptr){}
    TaskMessage(const TaskMessage& m) : msg(m.msg), arg(m.arg), target(m.target) {}
    TaskMessage& operator=(const TaskMessage& m)
    {
        if(this != &m)
        {
            msg = m.msg; arg = m.arg; target = m.target;
        }
        return *this;
    }
    
  private:
    Task* target;
    template<typename> friend class TaskTree;
};


/*!
  @brief Task definition.
  @attention class Derived by FamilyTree must be friend.
*/
class Task : public goblib::Node
{
  public:
    /*! 
      @enum Status
      @brief Using for the processing branch in pump().
    */
    enum Status : std::uint32_t
    {
        Initialize =    0x00000001, //!< Initialize phase.
        Release =       0x00000002, //!< Release phase
        Restart =       0x00000004, //!< Restart (Release to Initialize) phase
        Execute =       0x00000010, //!< Do something phase
        Pause =         0x80000000, //!< Pause flag (Puase the process of pump())
        Kill =          0x40000000, //!< Was released or force killed, so unlink in TaskTree::pump()
    };
    using StatusType = std::underlying_type<Status>::type;
    constexpr static StatusType MASK_STATUS = (Status::Initialize | Status::Release | Status::Restart | Status::Execute);
    constexpr static StatusType MASK_FLAG =   (Status::Pause | Status::Kill);

    using PriorityType = std::int32_t;

  public:
    Task() : Task(128, "dc") {}
    Task( PriorityType pri, const char* tag);
    virtual ~Task(){}

    /// @name Property
    /// @{
    GOBLIB_INLINE const char* tag() const { return _tag; }
    GOBLIB_INLINE PriorityType priority() const { return _priority; }
    GOBLIB_INLINE StatusType status() const { return _status; }
    GOBLIB_INLINE bool isInitialize() const { return (status() & MASK_STATUS) == Status::Initialize; }
    GOBLIB_INLINE bool isExecute() const { return (status() & MASK_STATUS) == Status::Execute; }
    GOBLIB_INLINE bool isRelease() const { return (status() & MASK_STATUS) == Status::Release; }
    GOBLIB_INLINE bool isPause() const { return status() & Status::Pause; }
    GOBLIB_INLINE bool isKill() const { return status() & Status::Kill; }
    /// @}

    /// @name Operation
    /// @{
    GOBLIB_INLINE void initialize() { _status = Status::Initialize; }
    void release(bool includeChildren = false)
    {
        _status = (_status & Status::Kill) | Status::Release;
        if(includeChildren && _left) { left<Task>()->_release(includeChildren); }
    }
    void restart(bool includeChildren = false)
    {
        _status = (_status & MASK_FLAG) | Status::Restart;
        if(includeChildren && _left) { left<Task>()->_restart(includeChildren); }
    }
    void kill (bool includeChildren = false)
    {
        _status |= Status::Kill;
        if(includeChildren && _left) { left<Task>()->_kill(includeChildren); }
    }
    void pause(bool b, bool includeChildren)
    {
        _status = (b ? (_status | Status::Pause) : (_status & ~Status::Pause));
        if(includeChildren && _left) { left<Task>()->_pause(b, includeChildren); }
    }
    GOBLIB_INLINE void pause(bool includeChildren = false) { pause(true, includeChildren); }
    GOBLIB_INLINE void resume(bool includeChildren = false){ pause(false, includeChildren); }
    /// @}
    
    /// @name Compare
    /// @{
    GOBLIB_INLINE bool operator<(const Task& o) const { return _priority < o._priority; }
    /// @}

    void pump(const float delta);

  protected:
    /// @name Override
    /// @{
    virtual bool onInitialize(){ return true; }
    virtual bool onRelease(){ return true; }
    virtual void onExecute(const float /*delta*/) {}
    virtual void onReceive(const TaskMessage& /*msg*/) {}
    /// @}

  private:
    void _release(bool includeChildren)
    {
        _status = (_status & Status::Kill) | Status::Release;
        if(includeChildren && _left) { left<Task>()->_release(includeChildren); }
        if(_right) { right<Task>()->_release(includeChildren); }
    }
    void _restart(bool includeChildren)
    {
        _status = (_status & MASK_FLAG) | Status::Restart;
        if(_left && includeChildren) { left<Task>()->_restart(includeChildren); }
        if(_right) { right<Task>()->_restart(includeChildren); }
    }
    void _kill(bool includeChildren)
    {
        _status |= Status::Kill;
        if(_left && includeChildren) { left<Task>()->_kill(includeChildren); }
        if(_right) { right<Task>()->_kill(includeChildren); }
    }
    void _pause(bool b, bool includeChildren)
    {
        _status = (b ? (_status | Status::Pause) : (_status & ~Status::Pause));
        if(_left && includeChildren) { left<Task>()->_pause(b, includeChildren); }
        if(_right) { right<Task>()->_pause(b, includeChildren); }
    }

  private:
    Task(const Task&) = delete;
    Task(Task&&) = delete;
    Task& operator=(const Task&) = delete;
    Task& operator=(Task&&) = delete;
    
  private:
    char _tag[16];
    PriorityType  _priority;
    StatusType _status;

    template<typename> friend class TaskTree;
};


/*!
  @brief Parent-child task system.
*/
template<class  T>
class TaskTree : public FamilyTree<T>
{
    static_assert(std::is_base_of<Task, T>::value, "T muse be Task or derived of Task");
    constexpr static std::size_t QUEUE_RESERVE_SIZE = 16;

  public:
    /*!
      @param qreserve Message queue max size for post message.
    */
    explicit TaskTree(std::size_t qreserve = QUEUE_RESERVE_SIZE)
            : FamilyTree<T>(), _message(qreserve), _broadcastMessage(qreserve), _pause(false)
    {
        assert(qreserve > 0 && "qreserve muset be greater than zero");
        _message.clear(); _broadcastMessage.clear();
    }
    
    /// @name Pause global
    /// @{
    /*! @brief Pause TaskTree::pump */
    GOBLIB_INLINE bool isPauseGlobal() const { return _pause; }
    GOBLIB_INLINE void pauseGlobal(bool b) { _pause = b; }
    GOBLIB_INLINE void pauseGlobal()  { pauseGlobal(true);  }
    GOBLIB_INLINE void resumeGlobal() { pauseGlobal(false); }
    /// @}

    /// @name Pause each task
    /// @{
    /*! @brief Pause each task */
    void pause(bool b, bool includeChildren)
    {
        typename FamilyTree<T>::Callback f = [&b, &includeChildren](T* t){ t->pause(b, includeChildren); };
        this->callback(f);
    }
    void pause()  { pause( true, true); }
    void resume() { pause(false, true); }
    /// @}
    
    void pump(const float delta = 1.0f)
    {
        if(isPauseGlobal()) { return; }

        typename FamilyTree<T>::Callback callback_pump = [&delta](T* t){ t->pump(delta); };    

        this->deliverMessage();
        this->callback(callback_pump);
        this->insertReservedNodes();
        this->removeNode_if(is_kill);
    }

    /// @name Message
    /// @{
    /*! Call onReceive() of target and does not return until processed message */
    template<class U, typename std::enable_if<std::is_base_of<goblib::Task, U>::value, nullptr_t>::type = nullptr>
    void sendMessage(const TaskMessage& m, U* target)
    {
        assert(target);
        target->onReceive(m);
    }

    /*! Call obReceive() of target on TaskTree::pump() */
    template<class U, typename std::enable_if<std::is_base_of<goblib::Task, U>::value, nullptr_t>::type = nullptr>
    void postMessage(const TaskMessage& m, U* target)
    {
        assert(target);
        TaskMessage msg(m);
        msg.target = static_cast<goblib::Task*>(target);
        _message.push_back(msg);
    }

    void sendBroadcastMessage(const TaskMessage& m)
    {
        sendBroadcastMessage(m, this->root());
    }
    
    /*! Call onReceive() of top and his children  and does not return until processed message */
    template<class U, typename std::enable_if<std::is_base_of<T, U>::value, nullptr_t>::type = nullptr>
    void sendBroadcastMessage(const TaskMessage& m, U* top)
    {
        T* t = static_cast<T*>(top ? top : this->root());
        typename FamilyTree<T>::Callback f = [&m](T* t){ t->onReceive(m); };
        this->callback(f, t);
    }

    void postBroadcastMessage(const TaskMessage& m)
    {
        postBroadcastMessage(m, this->root());
    }

    /*! Call obReceive() of top and his children on TaskTree::pump() */
    template<class U, typename std::enable_if<std::is_base_of<T, U>::value, nullptr_t>::type = nullptr>
    void postBroadcastMessage(const TaskMessage& m, U* top)
    {
        TaskMessage msg(m);
        msg.target = static_cast<T*>(top ? top : this->root());
        _broadcastMessage.push_back(msg);
    }
    
    /*! Underivered messages */
    GOBLIB_INLINE std::size_t undelivered() const { return _message.size(); }
    /*! Underivered broadcast messages */
    GOBLIB_INLINE std::size_t undeliveredBroadcast() const { return _broadcastMessage.size(); }
    /// @}
    
    virtual void print();

  private:
    void deliverMessage();

    TaskTree(const TaskTree&) = delete;
    TaskTree(TaskTree&&) = delete;
    TaskTree& operator=(const TaskTree&) = delete;
    TaskTree& operator=(TaskTree&&) = delete;
    
  private:
    typename FamilyTree<T>::Compare is_kill = [](const T* t){ return t->isKill(); };
    std::vector<TaskMessage> _message; // Undelivered message by post.
    std::vector<TaskMessage> _broadcastMessage; // Undelivered message by post broadcast.
    bool _pause;
};

template<class T> void TaskTree<T>::deliverMessage()
{
    // broadcast from msg,target
    for(auto& msg : _broadcastMessage)
    {
        typename FamilyTree<T>::Callback f = [&msg](T* t)
        {
            t->onReceive(msg);
        };
        this->callback(f, static_cast<T*>(msg.target));
    }
    _broadcastMessage.clear();

    // with destination
    for(auto& msg : _message)
    {
        msg.target->onReceive(msg);
    }
    _message.clear();
}

template<class T> void TaskTree<T>::print()
{
    static auto print_func = [](const T* c, std::uint32_t depth)
    {
        printf("%*c[%12s]:%08x,%-5d\n", depth*4, ' ', c->tag(), c->status(), c->priority());
    };
    printf("TaskTree pause:%d size:%zu\n", _pause, this->size());
    this->callback_with_depth(print_func);
}

//
}
#endif
