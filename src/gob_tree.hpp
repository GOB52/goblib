/*!
  Goblin Library

  @file  gob_tree.hpp
  @brief Tree by linked list.
*/
#pragma once
#ifndef GOBLIB_TREE_HPP
#define GOBLIB_TREE_HPP

#include <cstdint>
#include <type_traits> // std::std::is_same
#include <functional> // std::function
#include <cassert>

#include "gob_macro.hpp"
#include "gob_template_helper.hpp"

namespace goblib
{

template<class T> class FamilyTree;

/*!
  @brief Node for doubly-linked list.
  @remark Derive this for Tree<Derived>.
*/
struct Node
{
    Node() : _left(nullptr), _right(nullptr) {}
    virtual ~Node() = 0;

  protected:
    virtual void onChain(){}
    virtual void onUnchain(){}

    template<typename> friend class FamilyTree;
    Node* _left;
    Node* _right;

    /// @name Get tail of right,left
    /// @{
    Node* rightTail() { auto p = this; while(p->_right) { p = p->_right; } return p; }
    Node* leftTail()  { auto p = this; while(p->_left)  { p = p->_left;  } return p; }
    /// @}

    /// @name Cast right and left to deriverd class
    /// @{
    template<class T> GOBLIB_INLINE const T* right() const
    {
        static_assert(std::is_base_of<Node, T>::value, "T must be derived Node");
        return static_cast<const T*>(_right);
    }
    template<class T> GOBLIB_INLINE const T* left() const
    {
        static_assert(std::is_base_of<Node, T>::value, "T must be derived Node");
        return static_cast<const T*>(_left);
    }
    template<class T> GOBLIB_INLINE T* right()
    {
        static_assert(std::is_base_of<Node, T>::value, "T must be derived Node");
        return static_cast<T*>(_right);
    }
    template<class T> GOBLIB_INLINE T* left()
    {
        static_assert(std::is_base_of<Node, T>::value, "T must be derived Node");
        return static_cast<T*>(_left);
    }
    /// @}

};

/*!
  @brief left-child, right-sibling representation of tree
  @tparam T type of node.
  @remark Also known as doubly chained tree, child-sibling representation, filial-heir chain.
*/
template<class T> class FamilyTree
{
  public:
    static_assert(std::is_base_of<Node, T>::value ,"T must be derived from Node");
    static_assert(goblib::template_helper::can_determine_less<T>::value, "Cannot compare T < T ");

    FamilyTree() : _root(), _reserve(nullptr) {}
    virtual ~FamilyTree(){}

    /// @name Function type
    /// @{ 
    using Callback = std::function<void(T*)>;
    using ConstCallback = std::function<void(const T*)>;
    using Compare = std::function<bool(T*)>;
    using CallbackWithDepth = std::function<void(T*, const std::uint32_t)>;
    using ConstCallbackWithDepth = std::function<void(const T*, const std::uint32_t)>;
    /// @}

    
    /// @name Property
    /// @{
    /*! @brief Get root node */
    GOBLIB_INLINE const T* root() const { return &_root; }
    /*! @brief Get root node */
    GOBLIB_INLINE T* root() { return const_cast<T*>( static_cast<const FamilyTree&>(*this).root()); }

    /*! @brief Number of registered (exclude root) */
    std::size_t size() const
    {
        T* top = static_cast<T*>(root()->_left);
        std::size_t sz = 0;
        if(top)
        {
            ConstCallback f = [&sz](const T* c){ ++sz; };
            callback(f, top);
        }
        return sz;
    }
    /*! @brief Empry? */
    GOBLIB_INLINE bool empty() const { return root()->_left ? false : true; }
    
    /*! @brief Number of reserved for register */
    std::size_t reservedNodes() const
    {
        std::size_t sz = 0;
        auto p = _reserve;
        while(p) { ++sz; p = static_cast<T*>(p->_right); }
        return sz;
    }
    /// @}

    /// @name Callback
    /// @{
    void callback(Callback func)
    {
        this->callback(func, static_cast<T*>(root()));
    }

    void callback(Callback func) const
    {
        this->callback(func, const_cast<T*>(root()));
    }
                
    template<class U, typename std::enable_if<std::is_base_of<T, U>::value, nullptr_t>::type = nullptr>
    void callback(Callback func, U* start)
    {
        static_assert(goblib::template_helper::is_callable<decltype(func), T*>::value, "func isnot callable.");
        static_cast<const FamilyTree&>(*this).callback(func, start);
    }

    template<class U, typename std::enable_if<std::is_base_of<T, U>::value, nullptr_t>::type = nullptr>
    void callback(Callback func, U* start) const
    {
        static_assert(goblib::template_helper::is_callable<decltype(func), T*>::value, "func isnot callable.");
        T* p = static_cast<T*>(start);
        while(p)
        {
            func(p);
            if(p->_left) { callback(func, static_cast<T*>(p->_left)); }
            p = static_cast<T*>(p->_right);
        }
    }
    /// @}

    /// @name Operations
    /// @{
    /*! 
      @brief Clear
      @attention Memory of nodes does not deallocate. 
    */
    GOBLIB_INLINE void clear() { _root._left = _root._right = nullptr; }

    /*! @brief Insert node */
    void insertNode(T* node, T* parent = nullptr);
    /*! @brief Remove node
      @attention Memory of node does not deallocate. 
    */
    void removeNode(T* node) { removeNode(node, root()); }
    /*! @brief Remove nodes if match condition
      @attention Memory of nodes does not deallocate. 
    */
    GOBLIB_INLINE void removeNode_if(Compare func) {removeNode_if(func, root()); }

    /*! @brief Does node exists in tree? */
    bool exists(T* node) { return exists(node, root()); }
    /*! @brief Does node exists below start in tree? */
    bool exists(T* node, T* start);

    /*! @brief Reserve node registration */
    void reserveInsertNode(T* node, T* parent = nullptr);
    /*! @brief Insert nodes from nodes reserved */
    void insertReservedNodes();
    /// @}

  protected:
    virtual void onInsertNode(T*) {}
    virtual void onRemoveNode(T*) {}
    
    T* sort(T* head);
    void _sort(T* node, T*& sorted);

    T* removeNode(T* node, T* ptr)
    {
        Compare f = [&node](T* p) { return node == p; };
        return removeNode_if(f, ptr);
    }
    T* removeNode_if(Compare func, T* ptr);

    /// @name Callback with depth
    /// @{
    void callback_with_depth(CallbackWithDepth func, T* start = nullptr, const std::uint32_t depth = 0)
    {
        static_assert(goblib::template_helper::is_callable<decltype(func), T*, const std::uint32_t>::value, "func isnot callable.");
        static_cast<const FamilyTree&>(*this).callback_with_depth(func, start, depth);
    }

    void callback_with_depth(CallbackWithDepth func, T* start = nullptr, std::uint32_t depth = 0) const
    {
        static_assert(goblib::template_helper::is_callable<decltype(func), T*, const std::uint32_t>::value, "func isnot callable.");
        T* p = static_cast<T*>(start ? start : root()->_left);
        while(p)
        {
            func(p, depth);
            if(p->_left) { callback_with_depth(func, static_cast<T*>(p->_left), depth + 1); }
            p = static_cast<T*>(p->_right);
        }
    }
    /// @}

  private:
    T _root;        //!< Root is a persistent object and has no siblibng.
    T* _reserve;    //!< left is parent, right is next reserved node.
};

template<class T> void FamilyTree<T>::insertNode(T* node, T* parent)
{
    assert(node && "node is null");
    assert(node != root() && "node is root()");
    if(!node || node == root()) { return; }

    if(!parent) { parent = root(); }

    if(parent->_left)
    {
        node->_right = parent->_left;
        parent->_left = sort(node);
    }
    else
    {
        parent->_left = node;
    }

    node->onChain();
    onInsertNode(node);
}

template<class T> void FamilyTree<T>::reserveInsertNode(T* node, T* parent)
{
    if(!node) { return; }

    // node->_left is node's parent(Temporarily stored) (=>*1)
    node->_left = parent ? parent : root();
    node->_right = nullptr;

    if(!_reserve)
    {
        _reserve = node;
        return;
    }

    // Connect to tail of _reserve right.
    auto p = _reserve->rightTail();;
    p->_right = node;
}

template<class T> void FamilyTree<T>::insertReservedNodes()
{
    while(_reserve)
    {
        T* next = static_cast<T*>(_reserve->_right);
        T* parent = static_cast<T*>(_reserve->_left ? _reserve->_left : root()); // (=>*1)

        _reserve->_left = _reserve->_right = nullptr;
        insertNode(_reserve, parent);

        _reserve = next;
    }
}

template<class T> T* FamilyTree<T>::removeNode_if(Compare func, T* ptr)
{
    static_assert(goblib::template_helper::is_callable<decltype(func), T*>::value, "func isnot callable");
    if(!ptr){ return nullptr; }

    //    if(!func(ptr))
    {
        ptr->_left  = removeNode_if(func, static_cast<T*>(ptr->_left));
        ptr->_right = removeNode_if(func, static_cast<T*>(ptr->_right));
        //  return ptr;
    }
    if(!func(ptr)) { return ptr; }

    auto left = ptr->template left<T>();
    auto right = ptr->template right<T>();

    ptr->onUnchain();
    onRemoveNode(ptr);

    // Has no children.
    if(!left)
    {
        return static_cast<T*>(right);
    }
    // Has no siblings.
    else if(!right)
    {
        return static_cast<T*>(left);
    }

    // Connect child to parent's siblings.
    T* tail = static_cast<T*>(right->rightTail());
    tail->_right = left;

    return sort(static_cast<T*>(right));
}

template<class T> bool FamilyTree<T>::exists(T* node, T* start)
{
    auto p = start ? start : root()->_left;
    while(p)
    {
        if(p == node) { return true; }
        if(p->_left) { if(exists(node, static_cast<T*>(p->_left))) { return true; } }
        p = p->_right;
    }
    return false;
}

template<class T> T* FamilyTree<T>::sort(T* head)
{
    T* cur = head;
    T* sorted = nullptr;
    while(cur)
    {
        T* next = static_cast<T*>(cur->_right);
        _sort(cur, sorted);
        cur = next;
    }
    return sorted;
}

template<class T> void FamilyTree<T>::_sort(T* node, T*& sorted)
{
    if(!sorted || !(*sorted < *node)) // *sorted >= *node
    {
        node->_right = sorted;
        sorted = node;
    }
    else
    {
        T* cur = sorted;
        while(cur->_right &&(*(static_cast<T*>(cur->_right)) < *node))
        {
            cur = static_cast<T*>(cur->_right);
        }
        node->_right = static_cast<T*>(cur->_right);
        cur->_right = node;
    }
}

//
}
#endif
