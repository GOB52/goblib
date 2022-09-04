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
#include <cstdlib>
#include <iostream>
#include "gob_macro.hpp"

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


inline bool assertion_failed(char const* formatted)
{
        return (std::cerr << formatted << std::endl), std::abort(), false;
}

inline constexpr bool assertion_check(bool cond, char const* formatted)
{
    return cond ? true : goblib::debug::assertion_failed(formatted);
}
//
}}

/*! 
  @def GOBLIB_DUMP
  @brief dump output to stdout.
  @param buf target buffer
  @param sz length of buffer
 */
#   define GOBLIB_DUMP(buf, sz) do { goblib::debug::dump((buf), (sz)); } while(0)

#   define GOBLIB_ASSERTION_FAILED_FORMAT(expr, file, line) \
            "***** Internal Program Error - assertion (" #expr ") failed: " file "(" GOBLIB_STRINGIFY(line) ")"

/*!
  @def GOBLIB_ASSERT
  @brief Assertion check,It can use in constexpr function.
  @note From https://github.com/bolero-MURAKAMI/Sprout/blob/master/sprout/assert.hpp (BSL 1.0)
  @sa https://boleros.hateblo.jp/entry/20130319/1363719366
 */
#   define GOBLIB_ASSERT(expr) \
    ((void)goblib::debug::assertion_check((expr), GOBLIB_ASSERTION_FAILED_FORMAT(expr, __FILE__, __LINE__)))

#else
#   define GOBLIB_DUMP(buf, sz) /* NOP */
#   define GOBLIB_ASSERT(expr) (void)(0)
#endif

//
#endif
