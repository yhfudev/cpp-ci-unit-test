/**
 * \file    test-lang.c
 * \brief   test code of C language for unit test framework of C
 * \author  Yunhui Fu (yhfudev@gmail.com)
 * \version 1.0
 * \date    2017-03-14
 * \copyright MIT License
 */
#include <stdio.h>
#include <string.h>

/*#define CIUT_ENABLED 1 */ /**< user defined, a global macro defined to 1 to active the unit test code */

#if defined(CIUT_ENABLED) && (CIUT_ENABLED == 1)
#include <ciut.h>

TEST_CASE( .name="require-micro" ) {
    REQUIRE(0 == 0);
}

TEST_CASE( .name="float-equation", .description="Test float point equation." ) {

    SECTION("test float plus") {
        CIUT_DBL_EQUAL(0.1 + 0.2, 0.3);
        CIUT_DBL_EQUAL(0.0, 0.0 + 0.0);
    }
    SECTION("test float minus") {
        CIUT_DBL_EQUAL(0.3 - 0.2, 0.1);
        CIUT_DBL_EQUAL(1.0, 1.3 - 0.3);
    }
    SECTION("test float div") {
        CIUT_DBL_EQUAL(3000000.0 / 3, 1000000.0);
        CIUT_DBL_EQUAL(300000.0 / 3, 100000.0);
        CIUT_DBL_EQUAL(30000.0 / 3, 10000.0);
        CIUT_DBL_EQUAL(3000.0 / 3, 1000.0);
        CIUT_DBL_EQUAL(300.0 / 3, 100.0);
        CIUT_DBL_EQUAL(30.0 / 3, 10.0);
        CIUT_DBL_EQUAL(3.0 / 3, 1.0);
        CIUT_DBL_EQUAL(0.3 / 3, 0.1);
        CIUT_DBL_EQUAL(0.03 / 3, 0.01);
        CIUT_DBL_EQUAL(0.003 / 3, 0.001);
        CIUT_DBL_EQUAL(0.0003 / 3, 0.0001);
        CIUT_DBL_EQUAL(0.00003 / 3, 0.00001);
        CIUT_DBL_EQUAL(0.000003 / 3, 0.000001);
        CIUT_DBL_EQUAL(0.0000003 / 3, 0.0000001);
        CIUT_DBL_EQUAL(0.00000003 / 3, 0.00000001);
    }
}

TEST_CASE( .name="function-macro", .description="Test the __FUNCTION__ macro." ) {
    SECTION("test func") {
        CIUT_LOG ("function: %s", __FUNCTION__);
        REQUIRE(0 == strcmp(__FUNCTION__, "ciut_function_auto_2"));
    }
}

#if defined(__COUNTER__)
TEST_CASE( .name="counter-micro", .description="Test if __COUNTER__ increased." ) {
    SECTION("test counter") {
        int c;
        c = __COUNTER__;
        REQUIRE(c + 1 == __COUNTER__);
        CIUT_LOG ("counter? %d", __COUNTER__);
    }
}
#endif // __COUNTER__

#endif /* CIUT_ENABLED */


