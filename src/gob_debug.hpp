/*!
  Goblin Library

  @file  gob_debug.hpp
  @brief Debugging Support
  @remark Disable if NDEBUG defined.
*/
#pragma once
#ifndef GOBLIB_DEBUG_HPP
#define GOBLIB_DEBUG_HPP

#if !defined(NDEBUG) || defined(DOXYGEN_PROCESS)

#include <cstdint>
#include <cstddef>

namespace goblib
{

/*! debug
  @brief Debugging support
 */
namespace debug
{

void dump(const std::uint8_t*  buf, const std::size_t sz);

template<typename T> void dump(T buf, const std::size_t sz)
{
    dump(reinterpret_cast<const std::uint8_t*>(buf), sz);
}

}}

/*! 
  @def GOBLIB_DUMP
  @brief dump output to stdout.
  @param buf target buffer
  @param sz length of buffer
 */
#define GOBLIB_DUMP(buf, sz) do { goblib::debug::dump((buf), (sz)); } while(0)

#else

#define GOBLIB_DUMP(buf, sz) /* NOP */

#endif
#endif
