/*
  Unittest for gob_math
*/
#include <gob_math.hpp>
#include "helper.hpp"
#include "helper_constants.hpp"
using namespace test::helper;
using namespace test::constants;

#include "gtest/gtest.h"
#include <cmath>

constexpr float rad_deg_1 = 57.295776f;
constexpr float deg_rad_1 = 0.017453f;
constexpr float tolerance = 0.001f;


TEST(Math, PI)
{
    constexpr float f_pi = goblib::math::constants::pi_f;
    EXPECT_FLOAT_EQ(3.141592653589793f, f_pi);
}

namespace
{
template<typename T> void test_fminmax()
{
    constexpr T zero =    (T)+0.0;
    constexpr T n_zero =  (T)-0.0;
    constexpr T p_one =   (T)+1.0;
    constexpr T n_one =   (T)-1.00;
    constexpr T T_NAN = std::numeric_limits<T>::quiet_NaN();
    
    {
        GOBLIB_CONSTEXPR_GCC T f0 = goblib::math::fmin( zero, n_one);
        GOBLIB_CONSTEXPR_GCC T f1 = goblib::math::fmin( n_zero, zero);
        GOBLIB_CONSTEXPR_GCC T f2 = goblib::math::fmin( zero, p_one);
        GOBLIB_CONSTEXPR_GCC T f3 = goblib::math::fmin( zero, T_NAN);
        GOBLIB_CONSTEXPR_GCC T f4 = goblib::math::fmin( T_NAN, T_NAN);

        EXPECT_FLOAT_EQ(f0, n_one) << TypeName<T>();
        EXPECT_FLOAT_EQ(f1, n_zero) << TypeName<T>();
        EXPECT_FLOAT_EQ(f2, zero) << TypeName<T>();
        EXPECT_FLOAT_EQ(f3, zero) << TypeName<T>();
        EXPECT_TRUE(std::isnan(f4)) << TypeName<T>();
    }
    {
        GOBLIB_CONSTEXPR_GCC T f0 = goblib::math::fmax( zero, n_one);
        GOBLIB_CONSTEXPR_GCC T f1 = goblib::math::fmax( n_zero, zero);
        GOBLIB_CONSTEXPR_GCC T f2 = goblib::math::fmax( zero, p_one);
        GOBLIB_CONSTEXPR_GCC T f3 = goblib::math::fmax( zero, T_NAN);
        GOBLIB_CONSTEXPR_GCC T f4 = goblib::math::fmax( T_NAN, T_NAN);

        EXPECT_FLOAT_EQ(f0, zero) << TypeName<T>();
        EXPECT_FLOAT_EQ(f1, zero) << TypeName<T>();
        EXPECT_FLOAT_EQ(f2, p_one) << TypeName<T>();
        EXPECT_FLOAT_EQ(f3, zero) << TypeName<T>();
        EXPECT_TRUE(std::isnan(f4)) << TypeName<T>();
    }
}
//
}

TEST(Math, fminmax)
{
    test_fminmax<float>();
    test_fminmax<double>();
    test_fminmax<long double>();
}

namespace
{
template<typename T> void test_round()
{
    constexpr T v2 = (T)2.0;
    constexpr T v2_1 = (T)2.1;
    constexpr T v2_5 = (T)2.5;
    constexpr T v2_9 = (T)2.9;
    constexpr T v3 = (T)3.0;
    constexpr T n_v2 = (T)-2.0;
    constexpr T n_v2_1 = (T)-2.1;
    constexpr T n_v2_5 = (T)-2.5;
    constexpr T n_v2_9 = (T)-2.9;
    constexpr T n_v3 = (T)-3.0;
    constexpr T inf = std::numeric_limits<T>::infinity();
    constexpr T n_inf = -std::numeric_limits<T>::infinity();
    constexpr T T_NAN = std::numeric_limits<T>::quiet_NaN();

    constexpr T f0 = goblib::math::round(v2);
    constexpr T f1 = goblib::math::round(v2_1);
    constexpr T f2 = goblib::math::round(v2_5);
    constexpr T f3 = goblib::math::round(v2_9);
    constexpr T f4 = goblib::math::round(n_v2);
    constexpr T f5 = goblib::math::round(n_v2_1);
    constexpr T f6 = goblib::math::round(n_v2_5);
    constexpr T f7 = goblib::math::round(n_v2_9);
    constexpr T f8 = goblib::math::round(inf);
    constexpr T f9 = goblib::math::round(n_inf);
    constexpr T fa = goblib::math::round(T_NAN);

    EXPECT_FLOAT_EQ(f0, v2) << TypeName<T>();
    EXPECT_FLOAT_EQ(f1, v2) << TypeName<T>();
    EXPECT_FLOAT_EQ(f2, v3) << TypeName<T>();
    EXPECT_FLOAT_EQ(f3, v3) << TypeName<T>();
    EXPECT_FLOAT_EQ(f4, n_v2) << TypeName<T>();
    EXPECT_FLOAT_EQ(f5, n_v2) << TypeName<T>();
    EXPECT_FLOAT_EQ(f6, n_v3) << TypeName<T>();
    EXPECT_FLOAT_EQ(f7, n_v3) << TypeName<T>();
    EXPECT_FLOAT_EQ(f8, inf) << TypeName<T>();
    EXPECT_FLOAT_EQ(f9, n_inf) << TypeName<T>();
    EXPECT_TRUE(std::isnan(fa)) << TypeName<T>();
}

template<typename T> void test_floor()
{
    constexpr T v2 = (T)2.0;
    constexpr T v2_1 = (T)2.1;
    constexpr T v2_5 = (T)2.5;
    constexpr T v2_9 = (T)2.9;
    constexpr T n_v2 = (T)-2.0;
    constexpr T n_v2_1 = (T)-2.1;
    constexpr T n_v2_5 = (T)-2.5;
    constexpr T n_v2_9 = (T)-2.9;
    constexpr T n_v3 = (T)-3.0;
    constexpr T inf = std::numeric_limits<T>::infinity();
    constexpr T n_inf = -std::numeric_limits<T>::infinity();
    constexpr T T_NAN = std::numeric_limits<T>::quiet_NaN();

    constexpr T f0 = goblib::math::floor(v2);
    constexpr T f1 = goblib::math::floor(v2_1);
    constexpr T f2 = goblib::math::floor(v2_5);
    constexpr T f3 = goblib::math::floor(v2_9);
    constexpr T f4 = goblib::math::floor(n_v2);
    constexpr T f5 = goblib::math::floor(n_v2_1);
    constexpr T f6 = goblib::math::floor(n_v2_5);
    constexpr T f7 = goblib::math::floor(n_v2_9);
    constexpr T f8 = goblib::math::floor(inf);
    constexpr T f9 = goblib::math::floor(n_inf);
    constexpr T fa = goblib::math::floor(T_NAN);
    
    EXPECT_FLOAT_EQ(f0, v2) << TypeName<T>();
    EXPECT_FLOAT_EQ(f1, v2) << TypeName<T>();
    EXPECT_FLOAT_EQ(f2, v2) << TypeName<T>();
    EXPECT_FLOAT_EQ(f3, v2) << TypeName<T>();
    EXPECT_FLOAT_EQ(f4, n_v2) << TypeName<T>();
    EXPECT_FLOAT_EQ(f5, n_v3) << TypeName<T>();
    EXPECT_FLOAT_EQ(f6, n_v3) << TypeName<T>();
    EXPECT_FLOAT_EQ(f7, n_v3) << TypeName<T>();
    EXPECT_FLOAT_EQ(f8, inf) << TypeName<T>();
    EXPECT_FLOAT_EQ(f9, n_inf) << TypeName<T>();
    EXPECT_TRUE(std::isnan(fa)) << TypeName<T>();
}

template<typename T> void test_ceil()
{
    constexpr T v2 = (T)2.0;
    constexpr T v2_1 = (T)2.1;
    constexpr T v2_5 = (T)2.5;
    constexpr T v2_9 = (T)2.9;
    constexpr T v3 = (T)3.0;
    constexpr T n_v2 = (T)-2.0;
    constexpr T n_v2_1 = (T)-2.1;
    constexpr T n_v2_5 = (T)-2.5;
    constexpr T n_v2_9 = (T)-2.9;
    constexpr T inf = std::numeric_limits<T>::infinity();
    constexpr T n_inf = -std::numeric_limits<T>::infinity();
    constexpr T T_NAN = std::numeric_limits<T>::quiet_NaN();

    constexpr T f0 = goblib::math::ceil(v2);
    constexpr T f1 = goblib::math::ceil(v2_1);
    constexpr T f2 = goblib::math::ceil(v2_5);
    constexpr T f3 = goblib::math::ceil(v2_9);
    constexpr T f4 = goblib::math::ceil(n_v2);
    constexpr T f5 = goblib::math::ceil(n_v2_1);
    constexpr T f6 = goblib::math::ceil(n_v2_5);
    constexpr T f7 = goblib::math::ceil(n_v2_9);
    constexpr T f8 = goblib::math::ceil(inf);
    constexpr T f9 = goblib::math::ceil(n_inf);
    constexpr T fa = goblib::math::ceil(T_NAN);

    EXPECT_FLOAT_EQ(f0, v2) << TypeName<T>();
    EXPECT_FLOAT_EQ(f1, v3) << TypeName<T>();
    EXPECT_FLOAT_EQ(f2, v3) << TypeName<T>();
    EXPECT_FLOAT_EQ(f3, v3) << TypeName<T>();
    EXPECT_FLOAT_EQ(f4, n_v2) << TypeName<T>();
    EXPECT_FLOAT_EQ(f5, n_v2) << TypeName<T>();
    EXPECT_FLOAT_EQ(f6, n_v2) << TypeName<T>();
    EXPECT_FLOAT_EQ(f7, n_v2) << TypeName<T>();
    EXPECT_FLOAT_EQ(f8, inf) << TypeName<T>();
    EXPECT_FLOAT_EQ(f9, n_inf) << TypeName<T>();
    EXPECT_TRUE(std::isnan(fa)) << TypeName<T>();
}
//
}

TEST(Math, round)
{
    test_round<float>();
    test_round<double>();
    test_round<long double>();
}

TEST(Math, floor)
{
    test_floor<float>();
    test_floor<double>();
    test_floor<long double>();
}

TEST(Math, ceil)
{
    test_ceil<float>();
    test_ceil<double>();
    test_ceil<long double>();
}

namespace
{
template<typename T> void test_fabs()
{
    static_assert(std::numeric_limits<T>::has_infinity, "T does not has infinity!");
    constexpr T zero = (T)0.0f;
    constexpr T n_zero = (T)-0.0f;
    constexpr T v1_5 = (T)1.5;
    constexpr T n_v1_5 = (T)-1.5;
    constexpr T p_inf = std::numeric_limits<T>::infinity();
    constexpr T n_inf = -std::numeric_limits<T>::infinity();

    constexpr T f0 = goblib::math::fabs(v1_5);
    constexpr T f1 = goblib::math::fabs(n_v1_5);
    constexpr T f2 = goblib::math::fabs(zero);
    constexpr T f3 = goblib::math::fabs(n_zero);
    constexpr T f4 = goblib::math::fabs(p_inf);
    constexpr T f5 = goblib::math::fabs(n_inf);

    EXPECT_FLOAT_EQ(f0, v1_5) << TypeName<T>();
    EXPECT_FLOAT_EQ(f1, v1_5) << TypeName<T>();
    EXPECT_FLOAT_EQ(f2, zero) << TypeName<T>();
    EXPECT_FLOAT_EQ(f3, zero) << TypeName<T>();
    EXPECT_FLOAT_EQ(f4, p_inf) << TypeName<T>();
    EXPECT_FLOAT_EQ(f5, p_inf) << TypeName<T>();
}
template<typename T> void test_abs()
{
    static_assert(std::numeric_limits<T>::has_infinity, "T does not has infinity!");
    constexpr T zero = (T)0.0;
    constexpr T n_zero = (T)-0.0;
    constexpr T v1_5 = (T)1.5;
    constexpr T n_v1_5 = (T)-1.5;
    constexpr T p_inf = std::numeric_limits<T>::infinity();
    constexpr T n_inf = -std::numeric_limits<T>::infinity();

    constexpr T f0 = goblib::math::abs(v1_5);
    constexpr T f1 = goblib::math::abs(n_v1_5);
    constexpr T f2 = goblib::math::abs(zero);
    constexpr T f3 = goblib::math::abs(n_zero);
    constexpr T f4 = goblib::math::abs(p_inf);
    constexpr T f5 = goblib::math::abs(n_inf);

    EXPECT_FLOAT_EQ(f0, v1_5) << TypeName<T>();
    EXPECT_FLOAT_EQ(f1, v1_5) << TypeName<T>();
    EXPECT_FLOAT_EQ(f2, zero) << TypeName<T>();
    EXPECT_FLOAT_EQ(f3, zero) << TypeName<T>();
    EXPECT_FLOAT_EQ(f4, p_inf) << TypeName<T>();
    EXPECT_FLOAT_EQ(f5, p_inf) << TypeName<T>();
}

template<typename T> void test_abs_i()
{
    static_assert(std::numeric_limits<T>::has_infinity == false, "T has infinity!");
    constexpr T zero = (T)0;
    constexpr T n_zero = (T)-0;
    constexpr T v1 = (T)1;
    constexpr T n_v1 = (T)-1;
    constexpr T maximum = std::numeric_limits<T>::max();
    constexpr T lowest = std::numeric_limits<T>::lowest() + (T)1;

    constexpr T f0 = goblib::math::abs(v1);
    constexpr T f1 = goblib::math::abs(n_v1);
    constexpr T f2 = goblib::math::abs(zero);
    constexpr T f3 = goblib::math::abs(n_zero);
    constexpr T f4 = goblib::math::abs(maximum);
    constexpr T f5 = goblib::math::abs(lowest);

    EXPECT_EQ(f0, v1) << TypeName<T>();
    EXPECT_EQ(f1, v1) << TypeName<T>();
    EXPECT_EQ(f2, zero) << TypeName<T>();
    EXPECT_EQ(f3, zero) << TypeName<T>();
    EXPECT_EQ(f4, maximum) << TypeName<T>();
    EXPECT_EQ(f5, -lowest) << TypeName<T>();
}

//
}

TEST(Math, abs)
{
    test_fabs<float>();
    test_fabs<double>();
    test_fabs<long double>();

    test_abs<float>();
    test_abs<double>();
    test_abs<long double>();

    test_abs_i<int>();
    test_abs_i<long>();
    test_abs_i<long long int>();
}

namespace
{
template<typename T> void test_compare_i()
{
    static_assert(std::numeric_limits<T>::has_infinity == false, "T has infinity!");

    constexpr T a = (T)123;
    constexpr T b = (T)-123;
    constexpr T c = (T)100 + (T)23;
    constexpr T maximum = std::numeric_limits<T>::max();
    constexpr T lowest = std::numeric_limits<T>::lowest();

    constexpr bool a0 = goblib::math::equal(a,a);
    constexpr bool a1 = goblib::math::equal(a,b);
    constexpr bool a2 = goblib::math::equal(a,c);
    constexpr bool a3 = goblib::math::equal(a,maximum);
    constexpr bool a4 = goblib::math::equal(a,lowest);

    constexpr bool b0 = goblib::math::equal(b,b);
    constexpr bool b1 = goblib::math::equal(b,c);
    constexpr bool b2 = goblib::math::equal(b,maximum);
    constexpr bool b3 = goblib::math::equal(b,lowest);

    constexpr bool c0 = goblib::math::equal(c,c);
    constexpr bool c1 = goblib::math::equal(c,maximum);
    constexpr bool c2 = goblib::math::equal(c,lowest);

    constexpr bool i0 = goblib::math::equal(maximum, maximum);
    constexpr bool i1 = goblib::math::equal(maximum,lowest);

    constexpr bool ni0 = goblib::math::equal(lowest, lowest);

    constexpr bool f0 = goblib::math::not_equal(a,a);
    constexpr bool f1 = goblib::math::not_equal(a,b);
    constexpr bool f2 = goblib::math::not_equal(a,c);
    constexpr bool f3 = goblib::math::not_equal(a,maximum);
    constexpr bool f4 = goblib::math::not_equal(a,lowest);
    
    EXPECT_TRUE(a0) << TypeName<T>();
    EXPECT_FALSE(a1) << TypeName<T>();
    EXPECT_TRUE(a2) << TypeName<T>();
    EXPECT_FALSE(a3) << TypeName<T>();
    EXPECT_FALSE(a4) << TypeName<T>();

    EXPECT_TRUE(b0) << TypeName<T>();
    EXPECT_FALSE(b1) << TypeName<T>();
    EXPECT_FALSE(b2) << TypeName<T>();
    EXPECT_FALSE(b3) << TypeName<T>();

    EXPECT_TRUE(c0) << TypeName<T>();
    EXPECT_FALSE(c1) << TypeName<T>();
    EXPECT_FALSE(c2) << TypeName<T>();

    EXPECT_TRUE(i0) << TypeName<T>();
    EXPECT_FALSE(i1) << TypeName<T>();

    EXPECT_TRUE(ni0) << TypeName<T>();

    EXPECT_FALSE(f0) << TypeName<T>();
    EXPECT_TRUE(f1) << TypeName<T>();
    EXPECT_FALSE(f2) << TypeName<T>();
    EXPECT_TRUE(f3) << TypeName<T>();
    EXPECT_TRUE(f4) << TypeName<T>();
}

template<typename T> void test_compare()
{
    static_assert(std::numeric_limits<T>::has_infinity, "T does not has infinity!");
    constexpr T a = (T)123.456;
    constexpr T b = (T)-123.456;
    constexpr T c = (T)123.456;
    constexpr T inf = std::numeric_limits<T>::infinity();
    constexpr T n_inf = -std::numeric_limits<T>::infinity();
    constexpr T T_NAN = std::numeric_limits<T>::quiet_NaN();
    
    constexpr bool a0 = goblib::math::equal(a,a);
    constexpr bool a1 = goblib::math::equal(a,b);
    constexpr bool a2 = goblib::math::equal(a,c);
    constexpr bool a3 = goblib::math::equal(a,inf);
    constexpr bool a4 = goblib::math::equal(a,n_inf);
    constexpr bool a5 = goblib::math::equal(a,T_NAN);

    constexpr bool b0 = goblib::math::equal(b,b);
    constexpr bool b1 = goblib::math::equal(b,c);
    constexpr bool b2 = goblib::math::equal(b,inf);
    constexpr bool b3 = goblib::math::equal(b,n_inf);
    constexpr bool b4 = goblib::math::equal(b,T_NAN);

    constexpr bool c0 = goblib::math::equal(c,c);
    constexpr bool c1 = goblib::math::equal(c,inf);
    constexpr bool c2 = goblib::math::equal(c,n_inf);
    constexpr bool c3 = goblib::math::equal(c,T_NAN);

    constexpr bool i0 = goblib::math::equal(inf, inf);
    constexpr bool i1 = goblib::math::equal(inf, n_inf);
    constexpr bool i2 = goblib::math::equal(inf, T_NAN);

    constexpr bool ni0 = goblib::math::equal(n_inf, n_inf);
    constexpr bool ni1 = goblib::math::equal(n_inf, T_NAN);

    constexpr bool f0 = goblib::math::not_equal(a,a);
    constexpr bool f1 = goblib::math::not_equal(a,b);
    constexpr bool f2 = goblib::math::not_equal(a,c);
    constexpr bool f3 = goblib::math::not_equal(a,inf);
    constexpr bool f4 = goblib::math::not_equal(a,n_inf);
    constexpr bool f5 = goblib::math::not_equal(a,T_NAN);

    
    EXPECT_TRUE(a0) << TypeName<T>();
    EXPECT_FALSE(a1) << TypeName<T>();
    EXPECT_TRUE(a2) << TypeName<T>();
    EXPECT_FALSE(a3) << TypeName<T>();
    EXPECT_FALSE(a4) << TypeName<T>();
    EXPECT_FALSE(a5) << TypeName<T>();

    EXPECT_TRUE(b0) << TypeName<T>();
    EXPECT_FALSE(b1) << TypeName<T>();
    EXPECT_FALSE(b2) << TypeName<T>();
    EXPECT_FALSE(b3) << TypeName<T>();
    EXPECT_FALSE(b4) << TypeName<T>();

    EXPECT_TRUE(c0) << TypeName<T>();
    EXPECT_FALSE(c1) << TypeName<T>();
    EXPECT_FALSE(c2) << TypeName<T>();
    EXPECT_FALSE(c3) << TypeName<T>();

    EXPECT_TRUE(i0) << TypeName<T>();
    EXPECT_FALSE(i1) << TypeName<T>();
    EXPECT_FALSE(i2) << TypeName<T>();

    EXPECT_TRUE(ni0) << TypeName<T>();
    EXPECT_FALSE(ni1) << TypeName<T>();

    EXPECT_FALSE(f0) << TypeName<T>();
    EXPECT_TRUE(f1) << TypeName<T>();
    EXPECT_FALSE(f2) << TypeName<T>();
    EXPECT_TRUE(f3) << TypeName<T>();
    EXPECT_TRUE(f4) << TypeName<T>();
    EXPECT_TRUE(f5) << TypeName<T>();

    {
        constexpr bool t0 = goblib::math::equal_tolerance(a,a);
        constexpr bool t1 = goblib::math::equal_tolerance(a,b);
        constexpr bool t2 = goblib::math::equal_tolerance(a,c);
        constexpr bool t3 = goblib::math::equal_tolerance(a,inf);
        constexpr bool t4 = goblib::math::equal_tolerance(a,n_inf);
        constexpr bool t5 = goblib::math::equal_tolerance(a,T_NAN);
        constexpr bool t6 = goblib::math::equal_tolerance((T)-1.41421356, (T)-1.414, (T)0.000214);
        constexpr bool t7 = goblib::math::equal_tolerance((T)-1.41421356, (T)-1.414, (T)0.000213);
        constexpr bool t8 = goblib::math::equal_tolerance(T_NAN,T_NAN);

        EXPECT_TRUE(t0) << TypeName<T>();
        EXPECT_FALSE(t1) << TypeName<T>();
        EXPECT_TRUE(t2) << TypeName<T>();
        EXPECT_FALSE(t3) << TypeName<T>();
        EXPECT_FALSE(t4) << TypeName<T>();
        EXPECT_FALSE(t5) << TypeName<T>();
        EXPECT_TRUE(t6) << TypeName<T>();
        EXPECT_FALSE(t7) << TypeName<T>();
        EXPECT_FALSE(t8) << TypeName<T>();
    }

    {
        constexpr bool t0 = goblib::math::not_equal_tolerance(a,a);
        constexpr bool t1 = goblib::math::not_equal_tolerance(a,b);
        constexpr bool t2 = goblib::math::not_equal_tolerance(a,c);
        constexpr bool t3 = goblib::math::not_equal_tolerance(a,inf);
        constexpr bool t4 = goblib::math::not_equal_tolerance(a,n_inf);
        constexpr bool t5 = goblib::math::not_equal_tolerance(a,T_NAN);
        constexpr bool t6 = goblib::math::not_equal_tolerance((T)-1.41421356, (T)-1.414, (T)0.000214);
        constexpr bool t7 = goblib::math::not_equal_tolerance((T)-1.41421356, (T)-1.414, (T)0.000213);
        constexpr bool t8 = goblib::math::not_equal_tolerance(T_NAN,T_NAN);

        EXPECT_FALSE(t0) << TypeName<T>();
        EXPECT_TRUE(t1) << TypeName<T>();
        EXPECT_FALSE(t2) << TypeName<T>();
        EXPECT_TRUE(t3) << TypeName<T>();
        EXPECT_TRUE(t4) << TypeName<T>();
        EXPECT_TRUE(t5) << TypeName<T>();
        EXPECT_FALSE(t6) << TypeName<T>();
        EXPECT_TRUE(t7) << TypeName<T>();
        EXPECT_TRUE(t8) << TypeName<T>();
    }
}
//
}

TEST(Math, compare)
{
    test_compare_i<int>();
    test_compare_i<long>();
    test_compare_i<long long int>();
    test_compare_i<unsigned int>();
    test_compare_i<unsigned long>();
    test_compare_i<unsigned long long int>();

    test_compare<float>();
    test_compare<double>();
    test_compare<long double>();
}

TEST(Math, radian)
{
    // Wrap 360 degree.
    EXPECT_NEAR(  0.0f,  goblib::math::wrapDeg360(   0.0f), tolerance);
    EXPECT_NEAR(  0.0f,  goblib::math::wrapDeg360( 360.0f), tolerance);

    EXPECT_NEAR( 90.1f,  goblib::math::wrapDeg360(  90.1f), tolerance);
    EXPECT_NEAR(180.2f,  goblib::math::wrapDeg360( 180.2f), tolerance);
    EXPECT_NEAR(270.3f,  goblib::math::wrapDeg360( 270.3f), tolerance);
    EXPECT_NEAR(  0.4f,  goblib::math::wrapDeg360( 360.4f), tolerance);
    EXPECT_NEAR( 90.5f,  goblib::math::wrapDeg360( 450.5f), tolerance);

    EXPECT_NEAR(269.4f,  goblib::math::wrapDeg360( -90.6f), tolerance);
    EXPECT_NEAR(179.3f,  goblib::math::wrapDeg360(-180.7f), tolerance);
    EXPECT_NEAR( 89.2f,  goblib::math::wrapDeg360(-270.8f), tolerance);
    EXPECT_NEAR(359.1f,  goblib::math::wrapDeg360(-360.9f), tolerance);
    EXPECT_NEAR(269.99f, goblib::math::wrapDeg360(-450.01f), tolerance);


    // Wrap 180 degree..
    EXPECT_NEAR(   0.0f,  goblib::math::wrapDeg180(   0.0f), tolerance);
    EXPECT_NEAR(-180.0f,  goblib::math::wrapDeg180(-180.0f), tolerance);
    EXPECT_NEAR(-180.0f,  goblib::math::wrapDeg180( 180.0f), tolerance);
    EXPECT_NEAR(   0.0f,  goblib::math::wrapDeg180( 360.0f), tolerance);

    EXPECT_NEAR(  90.1f,  goblib::math::wrapDeg180(  90.1f), tolerance);
    EXPECT_NEAR(-179.8f,  goblib::math::wrapDeg180( 180.2f), tolerance);
    EXPECT_NEAR( -89.7f,  goblib::math::wrapDeg180( 270.3f), tolerance);
    EXPECT_NEAR(   0.4f,  goblib::math::wrapDeg180( 360.4f), tolerance);
    EXPECT_NEAR(  90.5f,  goblib::math::wrapDeg180( 450.5f), tolerance);

    EXPECT_NEAR( -90.6f,  goblib::math::wrapDeg180( -90.6f), tolerance);
    EXPECT_NEAR( 179.3f,  goblib::math::wrapDeg180(-180.7f), tolerance);
    EXPECT_NEAR(  89.2f,  goblib::math::wrapDeg180(-270.8f), tolerance);
    EXPECT_NEAR(  -0.9f,  goblib::math::wrapDeg180(-360.9f), tolerance);
    EXPECT_NEAR( -90.01f, goblib::math::wrapDeg180(-450.01f), tolerance);

    // Radian to degree.
    constexpr float d_zero = goblib::math::rad2deg(0.0f);
    constexpr float d_one = goblib::math::rad2deg(1.0f);

    EXPECT_FLOAT_EQ(     0.0f, d_zero);    
    EXPECT_FLOAT_EQ(rad_deg_1, d_one);

    EXPECT_FLOAT_EQ(180.0f,  goblib::math::rad2deg(goblib::math::constants::pi_f));
    EXPECT_FLOAT_EQ(360.0f,  goblib::math::rad2deg(2.0f * goblib::math::constants::pi_f));
    EXPECT_FLOAT_EQ(-180.0f, goblib::math::rad2deg(-goblib::math::constants::pi_f));
    EXPECT_FLOAT_EQ(-360.0f, goblib::math::rad2deg(-2.0f * goblib::math::constants::pi_f));

    EXPECT_NEAR( 45.0f, goblib::math::rad2deg(deg_rad_1 *  45.0f), tolerance);
    //    printf("%f,%f,%f\n",   90.0f,goblib::math::rad2deg(deg_rad_1 *  90.0f), std::abs(90.0f - goblib::math::rad2deg(deg_rad_1 *  90.0f)));
    
    EXPECT_FLOAT_EQ(  90.0f, round(goblib::math::rad2deg(deg_rad_1 *  90.0f)));
    EXPECT_FLOAT_EQ( 180.0f, round(goblib::math::rad2deg(deg_rad_1 * 180.0f)));
    EXPECT_FLOAT_EQ( 270.0f, round(goblib::math::rad2deg(deg_rad_1 * 270.0f)));
    EXPECT_FLOAT_EQ( 360.0f, round(goblib::math::rad2deg(deg_rad_1 * 360.0f)));

    EXPECT_FLOAT_EQ( -45.0f,round(goblib::math::rad2deg(-deg_rad_1 *  45.0f)));
    EXPECT_FLOAT_EQ( -90.0f,round(goblib::math::rad2deg(-deg_rad_1 *  90.0f)));
    EXPECT_FLOAT_EQ(-180.0f,round(goblib::math::rad2deg(-deg_rad_1 * 180.0f)));
    EXPECT_FLOAT_EQ(-270.0f,round(goblib::math::rad2deg(-deg_rad_1 * 270.0f)));
    EXPECT_FLOAT_EQ(-360.0f,round(goblib::math::rad2deg(-deg_rad_1 * 360.0f)));

    // Degree to radian.
    constexpr float r_zero = goblib::math::deg2rad(0.0f);
    constexpr float r_one = goblib::math::deg2rad(rad_deg_1);

    EXPECT_FLOAT_EQ(0.0f, r_zero);
    EXPECT_FLOAT_EQ(1.0f, r_one);

    EXPECT_FLOAT_EQ(        goblib::math::constants::pi_f, goblib::math::deg2rad( 180.0f));
    EXPECT_FLOAT_EQ( 2.0f * goblib::math::constants::pi_f, goblib::math::deg2rad( 360.0f));
    EXPECT_FLOAT_EQ(       -goblib::math::constants::pi_f, goblib::math::deg2rad(-180.0f));
    EXPECT_FLOAT_EQ(-2.0f * goblib::math::constants::pi_f, goblib::math::deg2rad(-360.0f));
    
    EXPECT_NEAR( 45.0f * deg_rad_1, goblib::math::deg2rad( 45.0f), tolerance);
    EXPECT_NEAR( 90.0f * deg_rad_1, goblib::math::deg2rad( 90.0f), tolerance);
    EXPECT_NEAR(180.0f * deg_rad_1, goblib::math::deg2rad(180.0f), tolerance);
    EXPECT_NEAR(270.0f * deg_rad_1, goblib::math::deg2rad(270.0f), tolerance);
    EXPECT_NEAR(360.0f * deg_rad_1, goblib::math::deg2rad(360.0f), tolerance);

    EXPECT_NEAR( -45.0f * deg_rad_1, goblib::math::deg2rad( -45.0f), tolerance);
    EXPECT_NEAR( -90.0f * deg_rad_1, goblib::math::deg2rad( -90.0f), tolerance);
    EXPECT_NEAR(-180.0f * deg_rad_1, goblib::math::deg2rad(-180.0f), tolerance);
    EXPECT_NEAR(-270.0f * deg_rad_1, goblib::math::deg2rad(-270.0f), tolerance);
    EXPECT_NEAR(-360.0f * deg_rad_1, goblib::math::deg2rad(-360.0f), tolerance);

    EXPECT_FLOAT_EQ(goblib::math::wrapRad(-goblib::math::constants::pi2_f), 0.0f);
    EXPECT_FLOAT_EQ(goblib::math::wrapRad( goblib::math::constants::pi2_f), 0.0f);
    EXPECT_FLOAT_EQ(goblib::math::wrapRad( goblib::math::constants::pi_f),  -goblib::math::constants::pi_f);
    EXPECT_FLOAT_EQ(goblib::math::wrapRad(-goblib::math::constants::pi_f),  -goblib::math::constants::pi_f);

    EXPECT_FLOAT_EQ(goblib::math::wrapRad(-goblib::math::constants::pi2_f * 10.0f), 0.0f);
    EXPECT_FLOAT_EQ(goblib::math::wrapRad( goblib::math::constants::pi2_f * 10.0f), 0.0f);
    EXPECT_FLOAT_EQ(goblib::math::wrapRad( goblib::math::constants::pi_f * 3.0f),  -goblib::math::constants::pi_f);
    EXPECT_FLOAT_EQ(goblib::math::wrapRad(-goblib::math::constants::pi_f * 3.0f),  -goblib::math::constants::pi_f);

    

}

namespace
{
template<typename T> void test_normalize01()
{
    constexpr T inf = std::numeric_limits<T>::infinity();
    constexpr T n_inf = -std::numeric_limits<T>::infinity();

    constexpr T n0 = goblib::math::normalize01((T)-123.456, (T)-123.456, (T)+456.789);
    constexpr T n1 = goblib::math::normalize01((T) 456.789, (T)-123.456, (T)+456.789);
    constexpr T n2 = goblib::math::normalize01((T)0.0, (T)-123.456, (T)+456.789);

    constexpr T n3 = goblib::math::normalize01(n_inf, n_inf, (T)456.789);
    constexpr T n4 = goblib::math::normalize01((T)0.0, n_inf, (T)456.789);
    constexpr T n5 = goblib::math::normalize01((T)123.456, n_inf, (T)456.789);

    constexpr T n6 = goblib::math::normalize01((T)-123.456, (T)-123.456, inf);
    constexpr T n7 = goblib::math::normalize01((T)0, (T)-123.456, inf);
    constexpr T n8 = goblib::math::normalize01(inf, (T)-123.456, inf);

    EXPECT_FLOAT_EQ(n0, (T)0.0) << TypeName<T>();
    EXPECT_FLOAT_EQ(n1, (T)1.0) << TypeName<T>();
    EXPECT_FLOAT_EQ(n2, (T)123.456 / (T)(123.456 + 456.789)) << TypeName<T>();
    EXPECT_FLOAT_EQ((T)0.5, (T)(123.456 + 456.789) /2 / (T)(123.456 + 456.789)) << TypeName<T>();

    EXPECT_TRUE(std::isnan(n3)) << TypeName<T>();
    EXPECT_TRUE(std::isnan(n4)) << TypeName<T>();
    EXPECT_TRUE(std::isnan(n5)) << TypeName<T>();

    EXPECT_TRUE(std::isnan(n6)) << TypeName<T>();
    EXPECT_TRUE(std::isnan(n7)) << TypeName<T>();
    EXPECT_TRUE(std::isnan(n8)) << TypeName<T>();

#if 0
    // Assertion on compile.
    constexpr T T_NAN = std::numeric_limits<T>::quiet_NaN();
    constexpr T n9 = goblib::math::normalize01(T_NAN, (T)-123.456, (T)+456.789);
    constexpr T na = goblib::math::normalize01((T)-123.456, T_NAN, (T)+456.789);
    constexpr T nb = goblib::math::normalize01((T)-123.456, (T)-123.456, T_NAN);
    EXPECT_TRUE(std::isnan(n9)) << TypeName<T>();
    EXPECT_TRUE(std::isnan(na)) << TypeName<T>();
    EXPECT_TRUE(std::isnan(nb)) << TypeName<T>();
#endif
}

template<typename T> void test_normalize01_i()
{
    constexpr T maximum = std::numeric_limits<T>::max();
    constexpr T lowest = std::numeric_limits<T>::lowest();

    constexpr auto n0 = goblib::math::normalize01((T)-123, (T)-123, (T)+456);
    constexpr auto n1 = goblib::math::normalize01((T) 456, (T)-123, (T)+456);
    constexpr auto n2 = goblib::math::normalize01((T) 0  , (T)-123, (T)+456);

    constexpr auto n3 = goblib::math::normalize01(lowest, lowest, (T)456);
    constexpr auto n4 = goblib::math::normalize01((T)0.0, lowest, (T)456);
    constexpr auto n5 = goblib::math::normalize01((T)456, lowest, (T)456);

    constexpr auto n6 = goblib::math::normalize01((T)-123, (T)-123, maximum);
    constexpr auto n7 = goblib::math::normalize01((T)0, (T)-123, maximum);
    constexpr auto n8 = goblib::math::normalize01(maximum, (T)-123, maximum);

    using rtype = decltype(n0);
    EXPECT_FLOAT_EQ(n0, (rtype)0.0) << TypeName<T>();
    EXPECT_FLOAT_EQ(n1, (rtype)1.0) << TypeName<T>();
    EXPECT_FLOAT_EQ(n2, (rtype)123 / (rtype)(123 + 456)) << TypeName<T>();
    EXPECT_FLOAT_EQ((rtype)0.5, (rtype)(123 + 456) /2 / (rtype)(123 + 456)) << TypeName<T>();

    EXPECT_FLOAT_EQ((rtype)0.0, n3) << TypeName<T>();
    EXPECT_FLOAT_EQ((rtype)0.99999976, n4) << TypeName<T>();
    EXPECT_FLOAT_EQ((rtype)1.0, n5) << TypeName<T>();
    EXPECT_FLOAT_EQ((rtype)0.0, n6) << TypeName<T>();
    constexpr float n7_a = (0 - -123) / ((float)maximum - -123);
    EXPECT_FLOAT_EQ((rtype)n7_a, n7) << TypeName<T>();
    EXPECT_GE((rtype)n7, (rtype)0);
    EXPECT_LE((rtype)n7, (rtype)1);
    EXPECT_FLOAT_EQ((rtype)1.0, n8) << TypeName<T>();
}
//
}

TEST(Math, normalize)
{
    test_normalize01_i<int>();
    test_normalize01_i<long>();
    test_normalize01_i<long long int>();

    test_normalize01<float>();
    test_normalize01<double>();
    test_normalize01<long double>();
}

TEST(Math, power)
{
    EXPECT_FALSE(goblib::math::is_powerof2(0));
    EXPECT_TRUE(goblib::math::is_powerof2(1));
    EXPECT_TRUE(goblib::math::is_powerof2(2));
    EXPECT_FALSE(goblib::math::is_powerof2(3));

    constexpr auto b = goblib::math::is_powerof2(-1);
    EXPECT_FALSE(b);

    EXPECT_TRUE(goblib::math::is_powerof2(2147483648));
    EXPECT_TRUE(goblib::math::is_powerof2(2147483648U));
    EXPECT_TRUE(goblib::math::is_powerof2(2147483648UL));
    EXPECT_TRUE(goblib::math::is_powerof2(2147483648LL));
    EXPECT_TRUE(goblib::math::is_powerof2(2147483648ULL));

    EXPECT_TRUE(goblib::math::is_powerof2(1099511627776LL));


    constexpr auto p0 = goblib::math::roundUpPow2(0);
    constexpr auto p1 = goblib::math::roundUpPow2(1);
    constexpr auto p2 = goblib::math::roundUpPow2(2);
    constexpr auto p3 = goblib::math::roundUpPow2(3);
    constexpr auto p4 = goblib::math::roundUpPow2(4);
    constexpr auto p5 = goblib::math::roundUpPow2(5);
    constexpr auto p6 = goblib::math::roundUpPow2(6);
    constexpr auto p7 = goblib::math::roundUpPow2(7);
    constexpr auto p8 = goblib::math::roundUpPow2(8);
    constexpr auto p9 = goblib::math::roundUpPow2(9);

    EXPECT_EQ(0U, p0);
    EXPECT_EQ(1U, p1);
    EXPECT_EQ(2U, p2);
    EXPECT_EQ(4U, p3);
    EXPECT_EQ(4U, p4);
    EXPECT_EQ(8U, p5);
    EXPECT_EQ(8U, p6);
    EXPECT_EQ(8U, p7);
    EXPECT_EQ(8U, p8);
    EXPECT_EQ(16U, p9);
    EXPECT_EQ(32U, goblib::math::roundUpPow2(24));
    EXPECT_EQ(32U, goblib::math::roundUpPow2(31));
    EXPECT_EQ(32U, goblib::math::roundUpPow2(32));
    EXPECT_EQ(64U, goblib::math::roundUpPow2(33));
    EXPECT_EQ(1024U, goblib::math::roundUpPow2(789));
    EXPECT_EQ(4U, goblib::math::roundUpPow2((char)260));
    EXPECT_EQ(0U, goblib::math::roundUpPow2((unsigned)-1));
    EXPECT_EQ(4294967296U, goblib::math::roundUpPow2<int64_t>((uint32_t)(-1) - 1));
    EXPECT_EQ(0U, goblib::math::roundUpPow2(-1));
    EXPECT_EQ(0U, goblib::math::roundUpPow2(-256));

    EXPECT_EQ(0U, goblib::math::nextPow2(-100));
    EXPECT_EQ(1U, goblib::math::nextPow2(0));
    EXPECT_EQ(2U, goblib::math::nextPow2(1));
    EXPECT_EQ(4U, goblib::math::nextPow2(2));
    EXPECT_EQ(4U, goblib::math::nextPow2(3));
    EXPECT_EQ(8U, goblib::math::nextPow2(4));
    EXPECT_EQ(32U, goblib::math::nextPow2(24));
    EXPECT_EQ(32U, goblib::math::nextPow2(31));
    EXPECT_EQ(64U, goblib::math::nextPow2(32));
    EXPECT_EQ(64U, goblib::math::nextPow2(33));
    EXPECT_EQ(1024U, goblib::math::nextPow2(789));
}

namespace
{
template<typename T> void test_sign()
{
    constexpr T s0 = goblib::math::sign((T)0);
    constexpr T s1 = goblib::math::sign((T)+1);
    constexpr T s2 = goblib::math::sign((T)-1);
    constexpr T s3 = goblib::math::sign(std::numeric_limits<T>::max());
    constexpr T s4 = goblib::math::sign(std::numeric_limits<T>::lowest());
    
    constexpr T s5 = goblib::math::sign( std::numeric_limits<T>::infinity());
    constexpr T s6 = goblib::math::sign(-std::numeric_limits<T>::infinity());
    constexpr T s7 = goblib::math::sign( std::numeric_limits<T>::quiet_NaN());

    EXPECT_FLOAT_EQ(s0, (T) 0.0) << TypeName<T>();
    EXPECT_FLOAT_EQ(s1, (T) 1.0) << TypeName<T>();
    EXPECT_FLOAT_EQ(s2, (T)-1.0) << TypeName<T>();
    EXPECT_FLOAT_EQ(s3, (T) 1.0) << TypeName<T>();
    EXPECT_FLOAT_EQ(s4, (T)-1.0) << TypeName<T>();

    EXPECT_FLOAT_EQ(s5, (T) 1.0) << TypeName<T>();
    EXPECT_FLOAT_EQ(s6, (T)-1.0) << TypeName<T>();
    EXPECT_FLOAT_EQ(s7, (T) 0.0) << TypeName<T>();
}
//
}

TEST(Math, sign)
{
    constexpr int s0 = goblib::math::sign(0);
    constexpr int s1 = goblib::math::sign(1);
    constexpr int s2 = goblib::math::sign(-1);
    constexpr int s3 = goblib::math::sign(std::numeric_limits<int>::max());
    constexpr int s4 = goblib::math::sign(std::numeric_limits<int>::lowest());

    EXPECT_EQ(s0, 0);
    EXPECT_EQ(s1, 1);
    EXPECT_EQ(s2, -1);
    EXPECT_EQ(s3, 1);
    EXPECT_EQ(s4, -1);
    
    constexpr unsigned int u0 = goblib::math::sign(0U);
    constexpr unsigned int u1 = goblib::math::sign(1U);
    constexpr unsigned int u2 = goblib::math::sign(-1U);
    constexpr unsigned int u3 = goblib::math::sign(std::numeric_limits<unsigned int>::max());
    constexpr unsigned int u4 = goblib::math::sign(std::numeric_limits<unsigned int>::lowest());

    EXPECT_EQ(u0, 0U);
    EXPECT_EQ(u1, 1U);
    EXPECT_EQ(u2, 1U);
    EXPECT_EQ(u3, 1U);
    EXPECT_EQ(u4, 0U);

    test_sign<float>();
    test_sign<double>();
    test_sign<long double>();
}
