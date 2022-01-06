/*!
  Goblin Library

  @file  gob_renderer.hpp
  @brief Rendering system
*/
#pragma once
#ifndef GOB_RENDERER_HPP
#define GOB_RENDERER_HPP

#include <gob_macro.hpp>
#include <cstdint>
#include <vector>
#include <algorithm> // std::sort
#include <cassert>

namespace goblib { namespace graph {

/*! @brief Rendering interface */
class RenderObj2D
{
  public:
    using OrderType = std::uint32_t;

    virtual ~RenderObj2D(){}

    /// @name Show,hide
    /// @{
    GOBLIB_INLINE bool visible() const { return _visible; }

    GOBLIB_INLINE virtual void show(bool b) { _visible = b; }
    GOBLIB_INLINE void show() { show(true); }
    GOBLIB_INLINE void hide() { show(false); }
    /// @}

    GOBLIB_INLINE std::uint32_t zorder() const { return _zorder; }

    /// @name Override
    /// @{
    virtual void render(void* arg) = 0;
    /// @}

  protected:
    explicit RenderObj2D(OrderType order = 0) : _zorder(order), _visible(true) {}
    
  private:
    OrderType _zorder;
    bool _visible;
};

/*! @brief Renderer for RenderObj2D */
class Renderer2D
{
    constexpr static std::size_t RESERVE = 64;

  public:
    /*! @param reserve Reserve size for registration of rendering objectes */
    explicit Renderer2D(std::size_t reserve = RESERVE)
            : _objects(reserve), _initializedSize(reserve), _dirty(false)
    { _objects.clear(); }
    virtual ~Renderer2D() {}

    std::size_t size() const { return _objects.size(); }

    void insert(RenderObj2D* o)
    {
#ifdef DEBUG
        auto it = std::find_if(_objects.begin(), _objects.end(), [&o](RenderObj2D* p) { return p == o; });
        assert(it == _objects.end() && "RenderObj2D already inserted");
#endif
        _objects.push_back(o); _dirty = true;

        assert(_objects.size() <= _initializedSize && "Expnad occur!");
    }

    void remove(RenderObj2D* o)
    {
        auto it = std::remove_if(_objects.begin(), _objects.end(), [&o](RenderObj2D* p) { return p == o; });
        _objects.erase(it, _objects.end());
    }

    void clear() { _objects.clear(); _dirty = false; }

    void zsort(bool force = false)
    {
        if(_dirty || force)
        {
            std::sort(_objects.begin(), _objects.end(), compare_greater);
            _dirty = false;
        }
    }

    virtual void render(void* arg)
    {
        zsort();
        for(auto& e : _objects)
        {
            if(e->visible()) { e->render(arg); }
        }
    }

    void print();
    
  private:
    Renderer2D(const Renderer2D&) = delete;
    Renderer2D& operator=(const Renderer2D&) = delete;

    static bool compare_greater(const RenderObj2D* left, const RenderObj2D* right) { return left->zorder() > right->zorder(); }
    
  private:
    std::vector<RenderObj2D*> _objects;
    const std::size_t _initializedSize;
    bool _dirty;
};

//
}}
#endif
