/*!
  Goblin Library

  @file   gob_macro.hpp
  @brief  Macro definition.
*/
#pragma once
#ifndef GOBLIB_MACRO_HPP
/// @cond
#define GOBLIB_MACRO_HPP
/// @endcond

/// @name Compiler detection
/// @{
#if !defined(DOXYGEN_PROCESS)
# if defined(__clang__)
#   define GOBLIB_COMPILER_CLANG
# elif defined(_MSC_VER)
#   define GOBLIB_COMPILER_MSC
# elif defined(__BORLANDC__)
#   define GOBLIB_COMPILER_BCC
# elif defined(__MINGW32__) || defined(__MINGW64__)
#   define GOBLIB_COMPILER_MINGW
# elif defined(__INTEL_COMPILER)
#   define GOBLIB_COMPILER_ICC
# elif defined(__GNUG__)
#   define GOBLIB_COMPILER_GCC
# else
#   define GOBLIB_COMPILER_UNKNOWN
#  endif
#else
/*! @brief defined if using Clang */
# define GOBLIB_COMPILER_CLANG
/*! @brief defined if using Microsoft C++ */
# define GOBLIB_COMPILER_MSC
/*! @brief defined if using Borland C++ */
# define GOBLIB_COMPILER_BCC
/*! @brief defined if using MinGW */
# define GOBLIB_COMPILER_MINGW
/*! @brief defined if using Intel compiler */
# define GOBLIB_COMPILER_ICC
/*! @brief defined if using GCC */
# define GOBLIB_COMPILER_GCC
/*! @brief defined if using unknown compiler */
# define GOBLIB_COMPILER_UNKNOWN
#endif
/// @}

#if defined(GOBLIB_COMPILER_GCC) || defined(DOXYGEN_PROCESS)
/*! @brief To be constexpr only compiled with GCC */
# define GOBLIB_CONSTEXPR_GCC constexpr
#else
# define GOBLIB_CONSTEXPR_GCC
#endif

/// @name C++ Version detection
/// @{
#if __cplusplus >= 201103L || defined(DOXYGEN_PROCESS)
/*!  @brief defined if C++11 or later */
# define GOBLIB_CPP11_OR_LATER
#endif

#if __cplusplus >= 201402L || defined(DOXYGEN_PROCESS)
/*! @brief defined if C++14 or later */
# define GOBLIB_CPP14_OR_LATER
#endif

#if __cplusplus >= 201703L || defined(DOXYGEN_PROCESS)
/*! @brief defined if C++17 or later */
# define GOBLIB_CPP17_OR_LATER
#endif

#if __cplusplus >= 202002L || defined(DOXYGEN_PROCESS)
/*! @brief defined if C++20 or later */
# define GOBLIB_CPP20_OR_LATER
#endif

#if __cplusplus < 201402L || defined(DOXYGEN_PROCESS)
/*! @brief defined if C++11 or earlier */
# define GOBLIB_CPP11_OR_EARLIER
#endif

#if __cplusplus < 201703L || defined(DOXYGEN_PROCESS)
/*! @brief defined if C++14 or earlier */
# define GOBLIB_CPP14_OR_EARLIER
#endif

#if __cplusplus < 202002L || defined(DOXYGEN_PROCESS)
/*! @brief defined if C++17 or earlier */
# define GOBLIB_CPP17_OR_EARLIER
#endif

//
#ifdef GOBLIB_CPP_VERSION_DETECTION
# if defined(GOBLIB_CPP11_OR_LATER) && defined(GOBLIB_CPP11_OR_EARLIER)
#   pragma message "C++11"
# elif defined(GOBLIB_CPP14_OR_LATER) && defined(GOBLIB_CPP14_OR_EARLIER)
#   pragma message "C++14"
# elif defined(GOBLIB_CPP17_OR_LATER) && defined(GOBLIB_CPP17_OR_EARLIER)
#   pragma message "C++17"
# elif defined(GOBLIB_CPP20_OR_LATER)
#   pragma message "C++20 or later"
# else
#   error "Unable to determine CPP version or C++03 or earlier."
# endif
#endif
/// @}

/*! @def GOBLIB_INLINE
  @brief Force inline
*/
#if defined(__GNUG__)
# define GOBLIB_INLINE  __attribute__ ((always_inline)) inline
#elif defined(_MSC_VER)
# define GOBLIB_INLINE __forceinline inline
#else
# define GOBLIB_INLINE  inline
#endif

#define GOBLIB_STRINGIFY_AGAIN(x) #x
/*! @def GOBLIB_STRINGIFY
  @brief Stringigy macro
*/
#define GOBLIB_STRINGIFY(x) GOBLIB_STRINGIFY_AGAIN(x)

#define GOBLIB_CONCAT_AGAIN(a,b) a ## b
/*! @def GOBLIB_CONCAT
  @brief  Concat macro
*/
#define GOBLIB_CONCAT(a,b) GOBLIB_CONCAT_AGAIN(a,b)

/// @name  C++ Attribute
/// @{
#if defined(GOBLIB_CPP14_OR_LATER) || defined(DOXYGEN_PROCESS)
/*! @def GOBLIB_DEPRECATED
  @brief Indicates that the use of the name or entity declared with this attribute is allowed, but discouraged for some reason.
*/
# define GOBLIB_DEPRECATED               [[deprectaed]]
/*! @def GOBLIB_DEPRECATED_MSG
  @brief with message
  @param msg string-literal
 */
# define GOBLIB_DEPRECATED_MSG(msg)      [[deprecated(msg)]]
#else
# if defined(__GNUG__)
#   define GOBLIB_DEPRECATED               __attribute__((deprecated))
#   define GOBLIB_DEPRECATED_MSG(msg)      __attribute__((deprecated(msg)))
# elif defined(_MSC_VER)
#   define GOBLIB_DEPRECATED               __declspec(deprecated)
#   define GOBLIB_DEPRECATED_MSG(msg)      __declspec(deprecated(msg))
# else
#   define GOBLIB_DEPRECATED               /*Nop*/
#   define GOBLIB_DEPRECATED_MSG(msg)	/*Nop*/
# endif
#endif

#if defined(GOBLIB_CPP20_OR_LATER) || defined(DOXYGEN_PROCESS)
/*! @def GOBLIB_NODISCARD
  @brief If a function declared nodiscard or a function returning an enumeration or class declared nodiscard by value is called from a discarded-value expression other than a cast to void, the compiler is encouraged to issue a warning. 
 */
# define GOBLIB_NODISCARD                [[nodiscard]]
/*! @def GOBLIB_NODISCARD_MSG
  @brief with message
  @param msg string-literal
  @warning Messages is valid from C++20.
 */
# define GOBLIB_NODISCARD_MSG(msg)       [[nodiscard(msg)]]
#elif defined(GOBLIB_CPP17_OR_LATER)
# define GOBLIB_NODISCARD                [[nodiscard]]
# define GOBLIB_NODISCARD_MSG(msg)       [[nodiscard]]
#else
# if defined(__GNUG__)
#   define GOBLIB_NODISCARD                __attribute__((warn_unused_result))
#   define GOBLIB_NODISCARD_MSG(msg)	/*Nop*/
# else
#   define GOBLIB_NODISCARD                /*Nop*/
#   define GOBLIB_NODISCARD_MSG(msg)	/*Nop*/
# endif
#endif

#if defined(GOBLIB_CPP17_OR_LATER) || defined(DOXYGEN_PROCESS)
/*! @def GOBLIB_MAYBE_UNUSED
  @brief Suppresses warnings on unused entities.
*/
# define GOBLIB_MAYBE_UNUSED     [[maybe_unused]]
/*! @def GOBLIB_FALLTHROUGH
  @brief Indicates that the fall through from the previous case label is intentional and should not be diagnosed by a compiler that warns on fallthrough. 
*/
# define GOBLIB_FALLTHROUGH      [[fallthrough]]
#else
# if defined(__GNUC__)
#   define GOBLIB_MAYBE_UNUSED     __attribute__((unused))
# else
#   define GOBLIB_MAYBE_UNUSED     /*Nop*/
# endif
# if defined(__GNUC__) &&  __GNUC__ >= 7
#   define GOBLIB_FALLTHROUGH      __attribute__ ((fallthrough))
# else
#   define GOBLIB_FALLTHROUGH      /* @fallthrough@ */
# endif
#endif

#if defined(__GNUC__) || defined(DOXYGEN_PROCESS)
/*! @def GOBLIB_WEAK
  @brief The weak attribute causes the declaration to be emitted as a weak symbol rather than a global. This is primarily useful in defining library functions which can be overridden in user code, though it can also be used with non-function declarations.
*/
# define GOBLIB_WEAK     __attribute__((weak))
#else
# define GOBLIB_WEAK     /* nop */
#endif

/// @}
#endif
