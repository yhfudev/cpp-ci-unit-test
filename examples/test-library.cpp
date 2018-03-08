/**
 * \file    test-library.cpp
 * \brief   test code of C++ language for unit test framework
 * \author  Yunhui Fu (yhfudev@gmail.com)
 * \version 1.0
 * \date    2017-12-06
 * \copyright MIT License
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h> // sleep
#include <stdlib.h>
#include <math.h>

/*#define CIUT_ENABLED 1 /**< user defined, a global macro defined to 1 to active the unit test code */

#if defined(CIUT_ENABLED) && (CIUT_ENABLED == 1)
#include <ciut.h>
#include <ciut-sio.h>

#define NUM_ARRAY(a) (sizeof(a)/sizeof((a)[0]))

TEST_CASE( .description="Test the time functions.", .skip=0 ) {

    SECTION("test timeval sub") {
        static struct timeval sub_data[] = {
            {0,   0}, {0,   0}, {0, 0},
            {0, 500}, {0, 500}, {0, 0},
            {1, 500}, {0, 500}, {1, 0},
        };
        size_t i;
        for (i = 0; i < NUM_ARRAY(sub_data); i += 3) {
            struct timeval ret;
            timeval_sub (&(sub_data[i + 0]), &(sub_data[i + 1]), &ret);
            CIUT_LOG("%" PRIuSZ ") %ld.%06ld - %ld.%06ld = ? %ld.%06ld", i / 3, sub_data[i + 0].tv_sec, sub_data[i + 0].tv_usec, sub_data[i + 1].tv_sec, sub_data[i + 1].tv_usec, sub_data[i + 2].tv_sec, sub_data[i + 2].tv_usec);
            REQUIRE((sub_data[i + 2].tv_sec == ret.tv_sec) && (sub_data[i + 2].tv_usec == ret.tv_usec));
        }
    }
}

#define msleep(microseconds) usleep((microseconds) * 1000);

TEST_CASE( .description="Test measure time.", .skip=0 ) {
    SECTION("test measure time") {
        size_t sleeptime = 1000;
        size_t ret_millisecond = 0;
        TMC_BEGIN();
        msleep(sleeptime);
        TMC_END(ret_millisecond);
        CIUT_LOG ("sleep milli seconds=%" PRIuSZ, ret_millisecond);
        REQUIRE (abs((int)ret_millisecond - (int)sleeptime) <= sleeptime/10);
    }
}

MYLOG_GLOBAL_INIT;

TEST_CASE( .description="Test log.", .skip=0 ) {
    SECTION("test log") {
        MYLOG_INIT_2STDERR();
        MLT("the network port=%d", 1);
    }
}

#endif /* CIUT_ENABLED */


