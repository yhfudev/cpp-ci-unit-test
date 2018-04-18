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
#include <unistd.h> // link()

#include <iostream>

//#define CIUT_ENABLED 1 /**< user defined, a global macro defined to 1 to active the unit test code */

#if defined(CIUT_ENABLED) && (CIUT_ENABLED == 1)
#include <ciut.h>
#include <uclog.h>
#include <ciut-sio.h>

#define NUM_ARRAY(a) (sizeof(a)/sizeof((a)[0]))

TEST_CASE( .name="timeval", .description="Test the time functions." ) {

    SECTION("test timeval sub") {
        static struct timeval sub_data[] = {
            {0,   0}, {0,   0}, {0, 0},
            {0, 500}, {0, 500}, {0, 0},
            {1, 500}, {0, 500}, {1, 0},
            {1, 500}, {0, 501}, {0, 999999},
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

#if 0
TEST_CASE( .name="msleep", .description="Test measure time.", .skip=1 ) {
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
#endif

MYLOG_GLOBAL_INIT;

TEST_CASE( .name="log-MLT", .description="Test log.", .skip=0 ) {
    SECTION("test log") {
        MYLOG_INIT_2STDERR();
        MLT("the network port=%d", 1);
    }
}

static int
cb_cuit_check_output_sio (FILE * outf, void * user_arg)
{
    char * msg = (char *)user_arg;
    assert (NULL != msg);
    fprintf(outf, "%s", msg);
    return 0;
}

const char *
err2cstr_cstd(int err)
{
#define CASE(a) case (a): return #a
    switch(err) {
        CASE(EACCES);
        CASE(EEXIST);
        CASE(ELOOP);
        CASE(EMLINK);
        CASE(ENAMETOOLONG);
        CASE(ENOENT);
        CASE(ENOSPC);
        CASE(ENOTDIR);
        CASE(EPERM);
        CASE(EROFS);
        CASE(EXDEV);
        //CASE(EXDEV);
        //CASE(ELOOP);
        //CASE(ENAMETOOLONG);
    }
    return "(unknown error)";
}


TEST_CASE( .name="ciutsio", .description="Test ciut sio.", .skip=0 ) {
    SECTION("test cuit_check_output") {
#define CSTR_TEST1 ""
        REQUIRE(0 == cuit_check_output(cb_cuit_check_output_sio, (void *)CSTR_TEST1, CSTR_TEST1));
#undef CSTR_TEST1
#define CSTR_TEST1 "this is my test string 1123243."
        REQUIRE(0 == cuit_check_output(cb_cuit_check_output_sio, (void *)CSTR_TEST1, CSTR_TEST1));
#undef CSTR_TEST1
    }
}

#if 1
TEST_CASE( .name="ciutsio", .description="Test ciut sio.", .skip=0 ) {
    SECTION("test create_file_namepipe") {
        int ret;
        pid_t pid;
        // create a null file
        unlink(FN_FIFO);
        ret = symlink(FN_FIFO, FN_FIFO);
        //ret = mkfifo(FN_FIFO, S_IREAD | S_IEXEC);
        if (0 == ret) {
            FILE *fp_f = NULL;
            FILE *fp_t = NULL;
            pid = create_file_namepipe(&fp_f, &fp_t);
            CIUT_LOG("DEBUG: create_file_namepipe return %d", pid);
            if (pid >= 0) {
                destroy_file_namepipe(pid, fp_f, fp_t);
                if (pid == 0) {
                    CIUT_LOG("child exit", 0);
                    exit (0);
                }
            }
            unlink(FN_FIFO);

            REQUIRE(-1 == pid);
            REQUIRE(fp_f == NULL);
            REQUIRE(fp_t == NULL);
        } else {
            CIUT_LOG("ERR: link return %d: err=%d %s", ret, errno, err2cstr_cstd(errno));
            perror("ERR: link()");
        }
    }
}
#endif // 0

#if ! (defined(_WIN32) || defined(__CYGWIN__) || defined(__APPLE__))
// test uclog.h
TEST_CASE( .name="log-writer", .description="Test log writer.", .skip=0 ) {

    SECTION("test log writer") {
        REQUIRE(0 == writer(NULL, NULL, 0));
        REQUIRE(0 == writer(NULL, "", 0));
        REQUIRE(0 == writer(NULL, "abc", 0));
        REQUIRE(3 == writer(NULL, "abc", 3));
        REQUIRE(0 == noop1(NULL, NULL, 0));
        REQUIRE(0 == noop3(NULL, NULL, 0));
        REQUIRE(0 == noop4(NULL));
    }

    SECTION("test C++ log buffer") {
        std::clog.rdbuf(new Syslog::sbuf("foo", LOG_LOCAL0));
        std::clog << Syslog::Notice << "myprog-E-net-0003 test log message" << std::endl;
        std::clog << "myprog-E-net-0003 the default is debug level" << std::endl;
    }

    // example1: to syslog
    SECTION("test C log syslog") {
        setlogmask (LOG_UPTO (LOG_NOTICE));
        openlog ("myapp", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
        MYLOG_INIT_2SYSLOG(); // OR: fsyslog(&stderr); MYLOG_INIT_2STDERR();
        MLT("the network port=%d", 1);
        MLF("myapp-F-net-3013 network failed!");
        closelog ();
    }
    SECTION("test C log stderr") {
        MYLOG_INIT_2STDERR();
        MLT("the network port=%d", 1);
    }

}
#endif

#endif /* CIUT_ENABLED */


