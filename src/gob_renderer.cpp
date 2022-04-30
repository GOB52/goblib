/*!
  Goblin Library

  @file  gob_renderer.cpp
  @brief Rendering system
*/
#include "gob_renderer.hpp"
#include <cstdio>

namespace goblib { namespace graph {

void Renderer2D::print()
{
    printf("Renderer: %zu\n", size());
    for(auto& e : _objects)
    {
        printf("[%5u : %c]\n", e->zorder(), e->visible() ? 'S' : 'H');

    }
}

//
}}
