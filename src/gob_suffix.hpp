/*!
  Goblin Library

  @file   gob_suffix.hpp
  @brief  User-defined literals.
*/
#ifndef GOBLIB_SUFFIX_HPP
#define GOBLIB_SUFFIX_HPP

#include <cstdint>

namespace goblib
{
/*! 
  Suffix
  @brief User-defined literals.
*/
namespace suffix
{

/// @name For integrer
/// @{

/*! @brief Suffix of int8_t. */
constexpr std::int8_t  operator "" _i8(unsigned long long x)    { return static_cast<std::int8_t>(x); }
/*! @brief Suffix of uint8_t. */
constexpr std::uint8_t operator "" _u8(unsigned long long x)    { return static_cast<std::uint8_t>(x); }

/*! @brief Suffix of int16_t. */
constexpr std::int16_t  operator "" _i16(unsigned long long x)  { return static_cast<std::int16_t>(x); }
/*! @brief Suffix of uint16_t. */
constexpr std::uint16_t operator "" _u16(unsigned long long x)  { return static_cast<std::uint16_t>(x); }

/// @}

//
}}
#endif
