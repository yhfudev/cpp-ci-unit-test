/**
 * \file    test-exception.cpp
 * \brief   test code of C++ exception for unit test framework of C
 * \author  Yunhui Fu (yhfudev@gmail.com)
 * \version 1.0
 * \date    2017-03-17
 * \copyright MIT License
 */
//#define CIUT_ENABLED 1 /**< user defined, a global macro defined to 1 to active the unit test code */

#if defined(CIUT_ENABLED) && (CIUT_ENABLED == 1)
#include <exception>      // std::exception
#include <ciut.h>

TEST_CASE( .name="cpp-throw", .description="Test C++ exception.", .skip=1 ) {
    SECTION("test func") {
        throw "exception test";
    }
}
TEST_CASE( .name="cpp-throw-std", .description="Test C++ std::exception.", .skip=1 ) {
    SECTION("test func") {
        struct ooops : std::exception {
            const char* what() const noexcept {return "std::exception!\n";}
        };
        ooops e;
        std::exception s;
        std::exception* p = &e;
        throw e;
        //throw s;
        //throw *p;
    }
}
#endif /* CIUT_ENABLED */

