/*!
  Goblin Library

  @file   gob_endianness.hpp
  @brief  Determine endianness.
*/
#pragma once
#ifndef GOBLIB_ENDIANNESS_HPP
#define GOBLIB_ENDIANNESS_HPP

#include "gob_macro.hpp"

#if defined(GOBLIB_CPP20_OR_LATER)
#include <bit> // std::endian
#else
#include <cstdint>
#endif

namespace goblib
{
/*!
  @brief Determined endianness.
*/
namespace endian
{
#if defined(GOBLIB_CPP20_OR_LATER)
/// @name endian type
/// @{
constexpr bool little = std::endian::native == std::endian::little; //!< true if little endian.
constexpr bool big = std::endian::native == std::endian::big;       //!< true if big endian.
constexpr bool other = !little && !big;                 //!< true if other endian.
/// @}
#else
/// @cond
constexpr std::uint32_t val32 = 0x11223344;
constexpr std::uint8_t ref8 = static_cast<const std::uint8_t&>(val32);
/// @endcond
/// @name endian type
/// @{
constexpr bool little = ref8 == 0x44;   //!< true if little endian.
constexpr bool big = ref8 == 0x11;  //!< true if big endian.
constexpr bool other = !little && !big; //!< true if other endian.
/// @}
#endif
static_assert(little || big || other, "Unable to determine endianness!");
}

//
}
#endif
