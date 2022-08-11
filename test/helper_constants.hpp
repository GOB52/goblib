#ifndef HELPER_CONSTANTS_HPP
#define HELPER_CONSTANTS_HPP

#include <cstdint>
#include <limits>

namespace test { namespace constants {

constexpr std::int32_t I32_MAX = std::numeric_limits<std::int32_t>::max();
constexpr std::int32_t I32_MIN = std::numeric_limits<std::int32_t>::min();

constexpr float F_MAX = std::numeric_limits<float>::max();
constexpr float F_MIN = std::numeric_limits<float>::lowest(); // min(float) returns minimum positive value of flaot.
constexpr float F_INF = std::numeric_limits<float>::infinity();
constexpr float F_NINF = -std::numeric_limits<float>::infinity();
constexpr float F_NAN = std::numeric_limits<float>::quiet_NaN();
constexpr float F_EPS = std::numeric_limits<float>::epsilon();

constexpr double D_MAX = std::numeric_limits<double>::max();
constexpr double D_MIN = std::numeric_limits<double>::lowest();
constexpr double D_INF = std::numeric_limits<double>::infinity();
constexpr double D_NINF = -std::numeric_limits<double>::infinity();
constexpr double D_NAN = std::numeric_limits<double>::quiet_NaN();
constexpr double D_EPS = std::numeric_limits<double>::epsilon();

constexpr long double LD_MAX = std::numeric_limits<long double>::max();
constexpr long double LD_MIN = std::numeric_limits<long double>::lowest();
constexpr long double LD_INF = std::numeric_limits<long double>::infinity();
constexpr long double LD_NINF = -std::numeric_limits<long double>::infinity();
constexpr long double LD_NAN = std::numeric_limits<long double>::quiet_NaN();
constexpr long double LD_EPS = std::numeric_limits<long double>::epsilon();
//
}}
#endif
