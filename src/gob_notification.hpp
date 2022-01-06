/*!
  Goblin Library

  @file  gob_notification.hpp
  @brief Notification system
*/
#pragma once
#ifndef GOBLIB_NOTIFICATION_HPP
#define GOBLIB_NOTIFICATION_HPP

#include <vector>
#include <cassert>
#ifndef NDEBUG
#include <algorithm>
#endif

namespace goblib
{

/*! @brief Observer(receiver)
  The class you want to observe will be derived from this class.
  @tparam T Type of subject.
*/
template<class T> class Observer
{
  public:
    Observer(){}
    virtual ~Observer(){}
    /*! @brief Must be override */
    virtual void onNotify(const T* subject, void* arg) = 0;
};


/*! @brief Subject(sender)
  The class you want to notify will be derived from this class.
  @tparam T Type of sender.
*/
template<class T> class Subject
{
  public:
    Subject() : _observers() {}
    virtual ~Subject(){}

    /// @name Observer
    /// @{
    void insertObserver(Observer<T>& o)
    {
#ifndef NDEBUG
        assert(!std::any_of(_observers.begin(), _observers.end(), [&o](Observer<T>* e){ return &o == e; })
               && "Already inserted");
#endif
        _observers.push_back(&o);
    }
    void removeObserver(Observer<T>& o)
    {
        auto it = std::remove(_observers.begin(), _observers.end(), &o);
        _observers.erase(it, _observers.end());
    }
    void clearObservers() { _observers.clear(); }
    /// @}
    
    /*! 
      @brief Notify to observers
      @warning Does not return until observers processed
    */
    void notify(void* arg = nullptr)
    {
        for(auto& e : _observers)
        {
            e->onNotify(static_cast<T*>(this), arg);
        }
    }

  private:
    std::vector<Observer<T>*> _observers;
};

//
}

#endif
