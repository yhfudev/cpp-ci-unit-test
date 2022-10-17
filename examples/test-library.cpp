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
cb_ciut_check_output_sio (FILE * outf, void * user_arg)
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
    }
    return "(unknown error)";
}
TEST_CASE( .name="error-handler-errno", .description="Test error numbers.", .skip=0 ) {
    SECTION("Test error numbers") {
#define CASE(err) REQUIRE(0 == strcmp(err2cstr_cstd(err), #err))
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
        REQUIRE(0 == strcmp(err2cstr_cstd(-1), "(unknown error)"))
    }
}

TEST_CASE( .name="ciut-sio", .description="Test ciut sio.", .skip=0 ) {
    SECTION("test ciut_check_output") {
#define CSTR_TEST1 ""
        REQUIRE(0 == ciut_check_output(cb_ciut_check_output_sio, (void *)CSTR_TEST1, CSTR_TEST1));
#undef CSTR_TEST1
#define CSTR_TEST1 "this is my test string 1123243."
        REQUIRE(0 == ciut_check_output(cb_ciut_check_output_sio, (void *)CSTR_TEST1, CSTR_TEST1));
#undef CSTR_TEST1
    }
}

#if ! (defined(_WIN32) || defined(__CYGWIN__) || defined(__APPLE__))
TEST_CASE( .name="ciut-sio API", .description="Test ciut sio API.", .skip=0 ) {
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
#if 0
TEST_CASE( .name="ciut-sio-internal", .description="Test ciut sio internal function.", .skip=1 ) {
    SECTION("test read_and_compare function") {
#define CSTR_TEST "abcdefghijklmnopqrstuvwxyz"
#define FN_TEST_TMP "/tmp/tmp-test.txt"
        FILE *fp = NULL;
        // create test file
        fp = fopen(FN_TEST_TMP, "w");
        REQUIRE(NULL != fp);
        fprintf(fp, "%s", CSTR_TEST);
        fclose(fp);

        fp = fopen(FN_TEST_TMP, "r");
        //fseek (fp, 0, SEEK_SET);
        REQUIRE(0 == read_and_compare(fp, CSTR_TEST));
        REQUIRE(-1 == read_and_compare(fp, "1" CSTR_TEST));
        REQUIRE(1 == read_and_compare(fp, CSTR_TEST "1"));

        fclose(fp);
#undef FN_TEST_TMP
#undef CSTR_TEST
    }
}
#endif // 0
#endif // _WIN32

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

#include "uctime.h"
TEST_CASE( .name="ciut-uctime", .description="Test ciut uctime.", .skip=0 ) {
    SECTION("test gettimeofday") {
        struct timeval tv;
        struct timezone tz;
        memset(&tv, 0, sizeof(tv));
        memset(&tz, 0, sizeof(tz));
        REQUIRE(0 == gettimeofday(NULL, NULL));
        REQUIRE(0 == gettimeofday(&tv, NULL));
        REQUIRE(0 != tv.tv_sec);
        REQUIRE(0 != tv.tv_usec);
        //tz_minuteswest, tz_dsttime
        REQUIRE(0 == gettimeofday(NULL, &tz));

        memset(&tv, 0, sizeof(tv));
        memset(&tz, 0, sizeof(tz));
        REQUIRE(0 == gettimeofday(&tv, &tz));
        REQUIRE(0 != tv.tv_sec);
        REQUIRE(0 != tv.tv_usec);
    }
    SECTION("test timeval_sub") {
        // int timeval_sub(struct timeval * tva, struct timeval * tvb, struct timeval * ret)
        struct timeval tva;
        struct timeval tvb;
        struct timeval tvc;

        memset(&tva, 0, sizeof(tva));
        memset(&tvb, 0, sizeof(tvb));
        memset(&tvc, 0, sizeof(tvc));
        tva.tv_sec = 10;
        tvb.tv_sec = 1;
        REQUIRE(0 == timeval_sub(&tva, &tvb, &tvc));
        REQUIRE(9 == tvc.tv_sec);
        REQUIRE(0 == tvc.tv_usec);

        memset(&tva, 0, sizeof(tva));
        memset(&tvb, 0, sizeof(tvb));
        memset(&tvc, 0, sizeof(tvc));
        tva.tv_usec = 10;
        tvb.tv_usec = 1;
        REQUIRE(0 == timeval_sub(&tva, &tvb, &tvc));
        REQUIRE(0 == tvc.tv_sec);
        REQUIRE(9 == tvc.tv_usec);
    }
}

#endif /* CIUT_ENABLED */


