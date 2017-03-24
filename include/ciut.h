/**
 * @file    ciut.h
 * @brief   An unit test framework for C and C++
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @date    2017-03-14
 * @copyright MIT License
 */
#ifndef _CI_UNIT_TEST_H
#define _CI_UNIT_TEST_H 1

#include <stdio.h> // fopen()
#include <stddef.h> // size_t
#include <stdlib.h> // exit()
#include <string.h> // memset()
#include <time.h>   // localtime_r()
#include <stdarg.h>
#include <assert.h>


// user defined, a global macro defined to 1 to active the unit test code
//#define CIUT_ENABLED 1
// user defined, a local macro defined to 1 to store the library inside a c file, use once
//#define CIUT_PLACE_MAIN 1

#define CIUT_LOG_SUITE_START  0x01
#define CIUT_LOG_SUITE_END    0x02
#define CIUT_LOG_CASE_START   0x03
#define CIUT_LOG_CASE_ASSERT  0x04
#define CIUT_LOG_CASE_SUCCESS 0x05
#define CIUT_LOG_CASE_FAILED  0x06
#define CIUT_LOG_CASE_SKIPED  0x07
typedef void (* ciut_cb_log_t)(void *fp, int type, const char *msg, ...);

/**
 * @brief to record the test results
 */
typedef struct _ciut_suite_t {
    size_t cnt_total;      /**< the total number of tests */
    size_t cnt_failed;     /**< the number of failed tests */
    size_t cnt_skipped;     /**< the number of skipped tests */
    int flg_error;         /**< temp variable for indicating error */
    void * fp_log;         /**< the pointer for log */
    ciut_cb_log_t cb_log;  /**< the callback for log */
} ciut_suite_t;

#define _CIUT_FUNCTION_ARGS ciut_suite_t *psuite  /**< the arguments for test case functions */
typedef void (* ciut_functions_t)(_CIUT_FUNCTION_ARGS);

#define _CIUT_TC_MAGIC (0xeffcbeef)
/**
 * @brief config structure for each test case
 */
typedef struct _ciut_record_t {
    unsigned int magic;
    ciut_functions_t run;
    const char * file;
    int line;

    /* user specified area */
    const char * name;  /**< name of test case */
    const char * description; /**< description of this test case */
    int skip; /**< if skip this test case */
} ciut_record_t;


#ifdef __cplusplus
#include <exception>
#include <cstring>

#define CIUT_EXPAND(...) __VA_ARGS__

#define CIUT_ARG_LENGTH(...)                                                       \
    CIUT_EXPAND(CIUT_ARG_LENGTH_(__VA_ARGS__,                                        \
            63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, \
            44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, \
            25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, \
            5, 4, 3, 2, 1, 0))
#define CIUT_ARG_LENGTH_(_63, _62, _61, _60, _59, _58, _57, _56, _55, _54, _53,   \
            _52, _51, _50, _49, _48, _47, _46, _45, _44, _43, _42, _41, _40, _39, _38, \
            _37, _36, _35, _34, _33, _32, _31, _30, _29, _28, _27, _26, _25, _24, _23, \
            _22, _21, _20, _19, _18, _17, _16, _15, _14, _13, _12, _11, _10, _9, _8,   \
            _7, _6, _5, _4, _3, _2, _1, count, ...)    count

#define CIUT_APPLY_1(macro, ...)
#define CIUT_APPLY_2(macro, prefix, head, ...)     macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_1(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_3(macro, prefix, head, ...)     macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_2(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_4(macro, prefix, head, ...)     macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_3(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_5(macro, prefix, head, ...)     macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_4(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_6(macro, prefix, head, ...)     macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_5(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_7(macro, prefix, head, ...)     macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_6(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_8(macro, prefix, head, ...)     macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_7(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_9(macro, prefix, head, ...)     macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_8(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_10(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_9(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_11(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_10(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_12(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_11(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_13(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_12(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_14(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_13(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_15(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_14(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_16(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_15(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_17(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_16(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_18(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_17(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_19(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_18(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_20(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_19(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_21(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_20(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_22(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_21(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_23(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_22(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_24(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_23(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_25(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_24(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_26(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_25(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_27(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_26(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_28(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_27(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_29(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_28(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_30(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_29(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_31(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_30(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_32(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_31(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_33(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_32(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_34(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_33(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_35(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_34(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_36(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_35(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_37(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_36(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_38(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_37(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_39(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_38(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_40(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_39(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_41(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_40(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_42(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_41(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_43(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_42(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_44(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_43(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_45(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_44(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_46(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_45(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_47(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_46(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_48(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_47(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_49(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_48(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_50(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_49(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_51(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_50(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_52(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_51(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_53(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_52(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_54(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_53(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_55(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_54(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_56(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_55(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_57(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_56(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_58(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_57(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_59(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_58(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_60(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_59(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_61(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_60(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_62(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_61(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_63(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_62(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_64(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_63(macro, prefix, __VA_ARGS__))
#define CIUT_APPLY_65(macro, prefix, head, ...)    macro(prefix, head) CIUT_EXPAND(CIUT_APPLY_64(macro, prefix, __VA_ARGS__))

#define CIUT_APPLY__(macro, prefix, n, ...)        CIUT_EXPAND(CIUT_APPLY_ ## n(macro, prefix, __VA_ARGS__, ))
#define CIUT_APPLY_(macro, n, prefix, ...)         CIUT_EXPAND(CIUT_APPLY__(macro, prefix, n, __VA_ARGS__))
#define CIUT_APPLY(macro, ...)                     CIUT_EXPAND(CIUT_APPLY_(macro, CIUT_ARG_LENGTH(__VA_ARGS__), __VA_ARGS__))

#define CIUT_ADD_PREFIX_ONCE(prefix, Field)        prefix Field;
#define CIUT_ADD_PREFIX(...) \
    CIUT_EXPAND(CIUT_APPLY(CIUT_ADD_PREFIX_ONCE, __VA_ARGS__))

# define CIUT_SET_STRUCT(type, ...)                  \
    []() -> type {                                   \
        type t;                                      \
        std::memset(&t, 0, sizeof (t));              \
        CIUT_EXPAND(CIUT_ADD_PREFIX(t, __VA_ARGS__)) \
        return t;                                    \
    } ()
#else
    #define CIUT_SET_STRUCT(Type, ...)    { __VA_ARGS__ }
#endif

#if defined(CIUT_ENABLED) && (CIUT_ENABLED == 1)

    #define _CIUT_COMBINE_2ITEM0( a, b ) a##_##b
    #define _CIUT_COMBINE_2ITEM( a, b ) _CIUT_COMBINE_2ITEM0( a, b )

    #if defined(__COUNTER__)
        #define _CIUT_CREATE_NAME(prefix) _CIUT_COMBINE_2ITEM(prefix, __COUNTER__)
    #else
        #define _CIUT_CREATE_NAME(prefix) _CIUT_COMBINE_2ITEM(prefix, __LINE__)
    #endif

    #define CIUT_FUNC_NAME(basename) _CIUT_COMBINE_2ITEM(ciut_function, basename)
    #define CIUT_RECD_NAME(basename) _CIUT_COMBINE_2ITEM(ciut_record, basename)

    #define _CIUT_FUNC_TITLE(basename) void CIUT_FUNC_NAME(basename) (_CIUT_FUNCTION_ARGS)

    #ifdef __APPLE__
        #define _CIUT_TC_SECTION __attribute__ ((used, section ("__DATA, ._ciut_record_t"), aligned(1)))
    #else
        #define _CIUT_TC_SECTION __attribute__ ((used, section ("._ciut_record_t"), aligned(1)))
    #endif

    #define _CIUT_TC_STRUCT(basename, ...) \
        static ciut_record_t CIUT_RECD_NAME(basename) _CIUT_TC_SECTION = CIUT_SET_STRUCT(ciut_record_t, \
            .magic = _CIUT_TC_MAGIC, \
            .run   = CIUT_FUNC_NAME(basename), \
            .file  = __FILE__, \
            .line  = __LINE__, \
            .name  = #basename, \
            .description = "", \
            .skip  = 0, \
            __VA_ARGS__ \
            )

    #define _CIUT_TEST_CASE1_WITH_NAME0(basename, ...) \
        static _CIUT_FUNC_TITLE(basename); \
        _CIUT_TC_STRUCT(basename, __VA_ARGS__); \
        static _CIUT_FUNC_TITLE(basename)

    #define _CIUT_TEST_CASE1_WITH_NAME( ... ) _CIUT_TEST_CASE1_WITH_NAME0( __VA_ARGS__ )
    #define CIUT_TEST_CASE1(...) _CIUT_TEST_CASE1_WITH_NAME(_CIUT_CREATE_NAME(auto), __VA_ARGS__)

    #define CIUT_TEST_CASE2(basename, ...) _CIUT_TEST_CASE1_WITH_NAME(basename, __VA_ARGS__)

    #define CIUT_LOG(fmt, ...) \
        assert (NULL != psuite->cb_log); \
        psuite->cb_log(psuite->fp_log, CIUT_LOG_CASE_ASSERT, "[%s():%d:%s] " fmt "\n", __FUNCTION__, __LINE__, __FILE__,  __VA_ARGS__)

    #define CIUT_SECTION(msg)
    #define CIUT_ASSERT(a) if(!(a)) { \
        psuite->flg_error = 1; \
        CIUT_LOG ("ASSERT: %s", #a); \
        return; \
    }

    #if defined(CIUT_PLACE_MAIN) && (CIUT_PLACE_MAIN == 1)

    inline static void ciut_cb_log_null(void *fp, int type, const char *msg, ...)
    {
    }

    inline static void ciut_cb_log_plaintext(void *fp, int type, const char *msg, ...)
    {
        switch (type) {
        case CIUT_LOG_CASE_SUCCESS:
            fprintf((FILE *)fp, "PASS: %s\n", msg);
            break;
        case CIUT_LOG_CASE_SKIPED:
            fprintf((FILE *)fp, "SKIP: %s\n", msg);
            break;
        case CIUT_LOG_CASE_FAILED:
            fprintf((FILE *)fp, "FAIL: %s\n", msg);
            break;
        case CIUT_LOG_CASE_ASSERT:
        {
            va_list args;
            va_start (args, msg);
            vfprintf((FILE *)fp, msg, args);
            va_end (args);
        }
            break;
        case CIUT_LOG_CASE_START:
            break;
        case CIUT_LOG_SUITE_START:
            fprintf((FILE *)fp, "Test Suite: %s\n", msg);
            break;
        case CIUT_LOG_SUITE_END:
            break;
        }
    }

    #include <sys/time.h> // gettimeofday()

#if _WIN32
    // struct tm * localtime_r(const time_t *timep, struct tm *result);
    #define localtime_r(timep, result) ((localtime_s(result, timep) == 0) ? result : NULL)
#endif

    /**
     * @brief log the message in xml
     * @param fp : the FILE pointer
     * @param type : the log type, one of CIUT_LOG_SUCCESS/FAILED/ASSERT
     * @param msg : the log message
     *
     */
    inline static void ciut_cb_log_xml(void *fp, int type, const char *msg, ...)
    {
        //struct timeval tv; gettimeofday(&tv, NULL);
        static char tbuf[sizeof ("yyyy-mm-dd hh:mm:ss.mmmmmm") + 2] = { 0 };
        struct timeval tv;
        struct tm now;
        time_t valtm;
        int len;

        if (0 == gettimeofday(&tv, NULL)) {
            valtm = tv.tv_sec;
            if (localtime_r(&valtm, &now) != NULL) {
                strftime(tbuf, sizeof(tbuf), "%Y-%m-%d %H:%M:%S", &now);
                len = strlen (tbuf);
                snprintf (tbuf + len, sizeof(tbuf) - len, ".%06lu", tv.tv_usec);
            }
        }

        assert(NULL != fp);
        switch (type) {
        case CIUT_LOG_CASE_SUCCESS:
            fprintf((FILE *)fp, "    <timeend>%s</timeend>\n", tbuf);
            fprintf((FILE *)fp, "    <result>successed</result>\n  </testcase>\n");
            break;
        case CIUT_LOG_CASE_SKIPED:
            fprintf((FILE *)fp, "    <timeend>%s</timeend>\n", tbuf);
            fprintf((FILE *)fp, "    <result>skipped</result>\n  </testcase>\n");
            break;
        case CIUT_LOG_CASE_FAILED:
            fprintf((FILE *)fp, "    <timeend>%s</timeend>\n", tbuf);
            fprintf((FILE *)fp, "    <result>failed</result>\n  </testcase>\n");
            break;
        case CIUT_LOG_CASE_ASSERT:
            fprintf((FILE *)fp, "    <output><![CDATA[");
        {
            va_list args;
            va_start (args, msg);
            vfprintf((FILE *)fp, msg, args);
            va_end (args);
        }
            fprintf((FILE *)fp, "]]></output>\n");
            break;
        case CIUT_LOG_CASE_START:
            fprintf((FILE *)fp, "  <testcase>\n    <desc>%s</desc>\n", msg);
            fprintf((FILE *)fp, "    <timestart>%s</timestart>\n", tbuf);
            break;
        case CIUT_LOG_SUITE_START:
            fprintf((FILE *)fp, "<testsuite>\n  <desc>%s</desc>\n", msg);
            fprintf((FILE *)fp, "  <timestart>%s</timestart>\n", tbuf);
            break;
        case CIUT_LOG_SUITE_END:
            fprintf((FILE *)fp, "  <timeend>%s</timeend>\n", tbuf);
            fprintf((FILE *)fp, "</testsuite>\n");
            break;
        }
    }

    inline static void usage(char *progname)
    {
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "    %s <options>\n", progname);
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "    -h            show help\n");
        fprintf(stderr, "    -l            list of test cases\n");
        fprintf(stderr, "    -f <filter>   the filter string\n");
        fprintf(stderr, "    -t <title>    unit test title\n");
        fprintf(stderr, "    -x <xml file> log to XML file\n");
        fprintf(stderr, "    -c | -        log to stdout\n");
        fprintf(stderr, "    \n");
    }

    /**
     * @brief the filer function
     */
    inline static char filter_match (const char *pattern, const char * desc)
    {
        if (NULL == pattern) {
            return 1;
        }
        assert (NULL != desc);
        if (NULL != strstr(desc, pattern)) {
            return 1;
        }
        return 0;
    }

#ifndef CIUT_HANDLE_SIGSEGV
    #define CIUT_HANDLE_SIGSEGV 1
#endif
#if (CIUT_HANDLE_SIGSEGV == 1)
    #include <setjmp.h>
    #include <signal.h>
    static jmp_buf jbuf_run; /**< jump buffer for running test cases */
    static void sighandler_seg(int signum)
    {
        //fprintf(stderr, "[SIGNAL %d: %s]\n", signum, sys_siglist[signum]);
        longjmp(jbuf_run, 1);
    }
#endif

    CIUT_TEST_CASE2(test_case_placeholder, .name="placeholder", .description="A place holder for internal use", .skip=1){} // the place holder

    inline static int ciut_main(size_t argc, char * argv[])
    {
        char msgbuf[200];
        const char * title = "Unit Test";
        const char * filter = NULL;
        const char * fn_xml = NULL;
        char flg_list = 0;
        size_t i;
        ciut_suite_t suite;
        ciut_suite_t *psuite = &suite;

        assert (psuite);
        memset (psuite, 0, sizeof(*psuite));
        psuite->cb_log = ciut_cb_log_null;

        for (i = 1; i < argc; i ++) {
            #define CHK_IDX(i) if (i >= argc) { fprintf(stderr, "Error in arguments. Use argument '-h' to show help.\n"); exit(1); }
            if (0 == strcmp("-h", argv[i])) {
                usage(argv[0]);
                exit(0);
            } else if (0 == strcmp("-l", argv[i])) {
                flg_list = 1;
            } else if (0 == strcmp("-t", argv[i])) {
                i ++;
                CHK_IDX(i);
                title = argv[i];
            } else if (0 == strcmp("-f", argv[i])) {
                i ++;
                CHK_IDX(i);
                filter = argv[i];
            } else if (0 == strcmp("-x", argv[i])) {
                i ++;
                CHK_IDX(i);
                fn_xml = argv[i];
            } else if ((0 == strcmp("-", argv[i])) || (0 == strcmp("-c", argv[i]))) {
                fn_xml = NULL;
                psuite->fp_log = stdout;
                psuite->cb_log = ciut_cb_log_plaintext;
            } else {
                fprintf(stderr, "Error: unknown parameter %s.\n", argv[i]);
                exit(1);
            }
        }
        if (flg_list) {
            fn_xml = NULL;
        }

        if (NULL != fn_xml) {
            if ((NULL != psuite->fp_log) && (stdout != psuite->fp_log)) {
                fclose((FILE *)psuite->fp_log);
            }
            psuite->fp_log = fopen(fn_xml, "w+");
            if (NULL == psuite->fp_log) {
                fprintf(stderr, "Error in open file %s.\n", fn_xml);
                exit(1);
            }
            psuite->cb_log = ciut_cb_log_xml;
            fprintf(stdout, "Created XML file %s.\n", fn_xml);
        }

        assert (psuite);
        assert (psuite->cb_log);
        psuite->cb_log(psuite->fp_log, CIUT_LOG_SUITE_START, title);

        /* find the test structures */
        ciut_record_t * ctc_begin;
        ciut_record_t * ctc_end;
        ciut_record_t * ctc_cur;
        for (ctc_begin = &CIUT_RECD_NAME(test_case_placeholder); ctc_begin->magic == _CIUT_TC_MAGIC; ctc_begin --);
        ctc_begin ++;
        for (ctc_end = &CIUT_RECD_NAME(test_case_placeholder); ctc_end->magic == _CIUT_TC_MAGIC; ctc_end ++);

#if (CIUT_HANDLE_SIGSEGV == 1)
        signal(SIGSEGV, sighandler_seg);
#endif
        for (ctc_cur = ctc_begin; ctc_cur < ctc_end; ctc_cur ++) {
            assert (ctc_cur);
            assert (ctc_cur->run);
            assert (ctc_cur->name);
            if (ctc_cur == &CIUT_RECD_NAME(test_case_placeholder)) {
                continue;
            }
            psuite->cnt_total ++;
            psuite->flg_error = 0;
            psuite->cb_log(psuite->fp_log, CIUT_LOG_CASE_START, ctc_cur->name);

            if (((filter == NULL) && ctc_cur->skip) || (0 == filter_match(filter, ctc_cur->name)) ) {
                psuite->cnt_skipped ++;
                snprintf(msgbuf, sizeof(msgbuf), "skip %s at (%d:%s)", ctc_cur->name, ctc_cur->line, ctc_cur->file);
                psuite->cb_log(psuite->fp_log, CIUT_LOG_CASE_ASSERT, msgbuf);
                psuite->cb_log(psuite->fp_log, CIUT_LOG_CASE_SKIPED, ctc_cur->name);
                continue;
            }

            if (flg_list) {
                // list the item
                fprintf (stdout, "% 3d) %s -- %s\n", psuite->cnt_total, ctc_cur->name, ctc_cur->description);
                continue;
            }

#if (CIUT_HANDLE_SIGSEGV == 1)
            if (setjmp(jbuf_run) == 0) {
#if __cplusplus
            try {
#endif
#endif

                ctc_cur->run(psuite);

#if (CIUT_HANDLE_SIGSEGV == 1)
#if __cplusplus
            } catch (const std::exception &e) {
                psuite->flg_error = 1;
                snprintf(msgbuf, sizeof(msgbuf), "C++ exception at (%d:%s): %s", ctc_cur->line, ctc_cur->file, e.what());
                psuite->cb_log(psuite->fp_log, CIUT_LOG_CASE_ASSERT, msgbuf);
            } catch(...) {
                psuite->flg_error = 1;
                snprintf(msgbuf, sizeof(msgbuf), "C++ exception at (%d:%s)", ctc_cur->line, ctc_cur->file);
                psuite->cb_log(psuite->fp_log, CIUT_LOG_CASE_ASSERT, msgbuf);
            }
#endif
            } else {
                // from longjmp()
                psuite->flg_error = 1;
                snprintf(msgbuf, sizeof(msgbuf), "Sigmentation fault at (%d:%s)", ctc_cur->line, ctc_cur->file);
                psuite->cb_log(psuite->fp_log, CIUT_LOG_CASE_ASSERT, msgbuf);
            }
#endif

            if (psuite->flg_error) {
                snprintf(msgbuf, sizeof(msgbuf), "%s at (%d:%s)", ctc_cur->name, ctc_cur->line, ctc_cur->file);
                psuite->cb_log(psuite->fp_log, CIUT_LOG_CASE_FAILED, msgbuf);
                psuite->cnt_failed ++;
            } else {
                snprintf(msgbuf, sizeof(msgbuf), "%s at (%d:%s)", ctc_cur->name, ctc_cur->line, ctc_cur->file);
                psuite->cb_log(psuite->fp_log, CIUT_LOG_CASE_SUCCESS, msgbuf);
            }
        }

        psuite->cb_log(psuite->fp_log, CIUT_LOG_SUITE_END, title);
        if (! flg_list) {
            fprintf(stdout, "Results:\n");
            fprintf(stdout, "    total cases: %lu\n", psuite->cnt_total);
            fprintf(stdout, "   passed cases: %lu\n", psuite->cnt_total - psuite->cnt_failed - psuite->cnt_skipped);
            fprintf(stdout, "  skipped cases: %lu\n", psuite->cnt_skipped);
            fprintf(stdout, "   failed cases: %lu\n", psuite->cnt_failed);
        }
        if (NULL != psuite->fp_log) {
            fclose((FILE *)psuite->fp_log);
        }
        return (psuite->cnt_failed > 0);
    }

    int main(int argc, char * argv[])
    {
        return ciut_main(argc, argv);
    }
    #endif /* CIUT_PLACE_MAIN */

#else
    #define CIUT_TEST_CASE1(msg)
    #define CIUT_SECTION(msg)
    #define CIUT_ASSERT(a) assert(a)
    #define CIUT_LOG(msg)
#endif

#define TEST_CASE               CIUT_TEST_CASE1
#define SECTION(msg)            CIUT_SECTION(msg)
#define REQUIRE(a)              CIUT_ASSERT(a)

#endif /* _CI_UNIT_TEST_H */
