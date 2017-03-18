/**
 * @file    test-sigsegv.c
 * @brief   test code of SIGSEGV for unit test framework of C
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @date    2017-03-17
 * @copyright GNU Lesser General Public License v2.0.
 */

#define CIUT_ENABLED 1 /**< user defined, a global macro defined to 1 to active the unit test code */
#include <ciut.h>

#if defined(CIUT_ENABLED) && (CIUT_ENABLED == 1)
TEST_CASE( .description="Test SIGSEGV.", .skip=1 ) {
    SECTION("test func") {
        char *p = NULL;
        REQUIRE(*p == 1);
    }
}
#endif /* CIUT_ENABLED */

