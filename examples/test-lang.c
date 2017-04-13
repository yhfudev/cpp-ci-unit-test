/**
 * @file    test-lang.c
 * @brief   test code of C language for unit test framework of C
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @date    2017-03-14
 * @copyright GNU Lesser General Public License v2.0.
 */
#include <stdio.h>
#include <string.h>

/*#define CIUT_ENABLED 1 /**< user defined, a global macro defined to 1 to active the unit test code */

#if defined(CIUT_ENABLED) && (CIUT_ENABLED == 1)
#include <ciut.h>

TEST_CASE() {
    REQUIRE(0 == 0);
}

TEST_CASE( .description="Test float point equation.", .skip=0 ) {
    CIUT_DBL_EQUAL(0.1 + 0.2, 0.3);
    CIUT_DBL_EQUAL(0.0, 0.0 + 0.0);
}

TEST_CASE( .description="Test the __FUNCTION__ macro.", .skip=0 ) {
    SECTION("test func") {
        CIUT_LOG ("function: %s", __FUNCTION__);
        REQUIRE(0 == strcmp(__FUNCTION__, "ciut_function_auto_2"));
    }
}

#if defined(__COUNTER__)
TEST_CASE( .description="Test if __COUNTER__ increased.", .skip=0 ) {
    SECTION("test counter") {
        int c;
        c = __COUNTER__;
        REQUIRE(c + 1 == __COUNTER__);
        CIUT_LOG ("counter? %d", __COUNTER__);
    }
}
#endif // __COUNTER__

#endif /* CIUT_ENABLED */


