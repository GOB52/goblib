/*
  main for test.
*/
#include <gtest/gtest.h>
#include <gob_macro.hpp> // GOBLIB_WEAK

GOBLIB_WEAK int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    (void)RUN_ALL_TESTS();
    // Always return zero-code and allow PlatformIO to parse results
    return 0;
}
