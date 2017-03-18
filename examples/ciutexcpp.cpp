/**
 * @file    ciutexcpp.cpp
 * @brief   test code of C++ language for unit test framework of C
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @date    2017-03-14
 * @copyright GNU Lesser General Public License v2.0.
 */
//#define CIUT_ENABLED 1 /**< user defined, a global macro defined to 1 to active the unit test code */
#define CIUT_PLACE_MAIN 1 /**< user defined, a local macro defined to 1 to place main() inside a c file, use once */
#define CIUT_HANDLE_SIGSEGV 1 /**< user defined in ciut main, set to 0 if you want to dig into where is the segmentation fault by gdb */
#include <ciut.h>

