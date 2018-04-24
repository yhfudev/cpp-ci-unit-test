/**
 * \file    test-sio.c
 * \brief   test code of ciut-sio
 * \author  Yunhui Fu (yhfudev@gmail.com)
 * \version 1.0
 * \date    2018-03-14
 * \copyright MIT License
 */
#include <stdio.h>
#include <string.h>

/*#define CIUT_ENABLED 1 */ /**< user defined, a global macro defined to 1 to active the unit test code */

#ifdef _WIN32
/* win32 won't support named pipe */

#else /* _WIN32 */
#if defined(CIUT_ENABLED) && (CIUT_ENABLED == 1)
#include <ciut.h>

static int test_stub_fork(void);
static pid_t test_stub_wait(int *status);
static FILE * test_stub_fopen(const char *fn, const char * mode);
static int test_stub_fclose(FILE *fp);
static ssize_t test_stub_getline(char **lineptr, size_t *n, FILE *stream);
static int test_stub_mkfifo(const char *pathname, mode_t mode);
static int test_stub_fcntl(int fd, int cmd, ... );

static void * test_stub_malloc(size_t size);
static int test_stub_strcmp(const char *s1, const char *s2);
//static int test_stub_strncmp(const char *s1, const char *s2, size_t n);
static int test_stub_remove(const char *pathname);

static void * g_ret_malloc = NULL;
static void *
test_stub_malloc(size_t size)
{
    if (NULL == g_ret_malloc) {
        return g_ret_malloc;
    }
    return malloc(size);
}

// test stub function replacements
#define malloc      test_stub_malloc
#define strcmp      test_stub_strcmp
//#define strncmp     test_stub_strncmp

#define mkfifo(a,b) test_stub_mkfifo(a,b)
#define unlink(a)   (0)
#define fork        test_stub_fork
#define fopen       test_stub_fopen
#define getline     test_stub_getline
#define fcntl(fd,...) test_stub_fcntl(fd,##__VA_ARGS__)
#define fileno(a)   (0)
#define remove      test_stub_remove
#define fclose      test_stub_fclose
#define exit(a)     return (a)
#define wait        test_stub_wait
#undef WIFEXITED
#define WIFEXITED(status) ((status) & 0x80)
#undef WEXITSTATUS
#define WEXITSTATUS(status) ((status) & 0x7F)
#include "ciut-sio.h"


static int g_ret_fork = 0;
static int
test_stub_fork(void)
{
    return g_ret_fork;
}

static pid_t g_ret_wait = -1;
static pid_t
test_stub_wait(int *status)
{
    if (NULL == status) {
        return -1;
    }
    *status = 0;
    if (0 != g_ret_wait) {
        *status = 0x80 | 0x0F;
    }
    return g_ret_wait;
}

static int
test_stub_mkfifo(const char *pathname, mode_t mode)
{
    return 0;
}

static int g_ret_remove = -1;
static int
test_stub_remove(const char *pathname)
{
    return g_ret_remove;
}

static int g_ret_fcntl_get = 0;
static int g_ret_fcntl_set = 0;
static int
test_stub_fcntl(int fd, int cmd, ... /* arg */ )
{
    va_list args;
    int flags;
    int ret = -1;

    va_start(args, cmd);
    if (F_GETFL == cmd) {
        ret = g_ret_fcntl_get;
    } else if (F_SETFL == cmd) {
        flags = va_arg(args, int);
        ret = g_ret_fcntl_set;
    }
    va_end(args);
    return ret;
}

static int g_ret_fopen = 0; // -1 -- return NULL, 0 -- return stdin, 1 -- return stdout, 2 -- return stderr
static FILE *
test_stub_fopen(const char *fn, const char * mode)
{
    switch(g_ret_fopen) {
    case 0: return stdin;
    case 1: return stdout;
    case 2: return stderr;
    }
    return NULL;
}

static int g_ret_fclose = 0;
static int
test_stub_fclose(FILE *fp)
{
    if (NULL == fp) {
        return -1;
    }
    return g_ret_fclose;
}

static ssize_t g_ret_getline = 0;
static ssize_t
test_stub_getline(char **lineptr, size_t *n, FILE *stream)
{
    if (g_ret_getline) {
        g_ret_getline = 0;
        return 1;
    }
    return 0;
}

static int g_ret_strcmp = 0;
static int
test_stub_strcmp(const char *s1, const char *s2)
{
    return g_ret_strcmp;
}

#if 0
static int g_ret_strncmp = 0;
static int
test_stub_strncmp(const char *s1, const char *s2, size_t n)
{
    return g_ret_strncmp;
}
#endif

TEST_CASE( .name="ciut-sio", .description="Test the sio internal functions." ) {
    FILE *fp_from = NULL;
    FILE *fp_to = NULL;
    SECTION("test create_file_namepipe") {
        g_ret_fork = -1;
        g_ret_fopen = -1;
        g_ret_fclose = -1;
        g_ret_fcntl_get = -1;
        g_ret_fcntl_set = -1;

        CIUT_LOG ("test %s", "fork error");
        g_ret_fork = -1;
        //REQUIRE(-1 == create_file_namepipe(NULL,     NULL));
        //REQUIRE(-1 == create_file_namepipe(&fp_from, NULL));
        //REQUIRE(-1 == create_file_namepipe(NULL,     &fp_to));
        REQUIRE(-1 == create_file_namepipe(&fp_from,  &fp_to));

        CIUT_LOG ("test %s", "child");
        g_ret_fork = 0;
        g_ret_fopen = -1;
        //REQUIRE(-1 == create_file_namepipe(NULL,     NULL));
        //REQUIRE(-1 == create_file_namepipe(&fp_from, NULL));
        //REQUIRE(-1 == create_file_namepipe(NULL,     &fp_to));
        REQUIRE(EXIT_FAILURE == create_file_namepipe(&fp_from, &fp_to));
        g_ret_fork = 0;
        g_ret_fopen = 0;
        REQUIRE(g_ret_fork == create_file_namepipe(&fp_from, &fp_to));

        CIUT_LOG ("test %s", "parent");
        g_ret_fork = 1;
        g_ret_fopen = -1;
        //REQUIRE(-1 == create_file_namepipe(NULL,     NULL));
        //REQUIRE(-1 == create_file_namepipe(&fp_from, NULL));
        //REQUIRE(-1 == create_file_namepipe(NULL,     &fp_to));
        REQUIRE(-1 == create_file_namepipe(&fp_from, &fp_to));
        g_ret_fork = 1;
        g_ret_fopen = 0;
        g_ret_fcntl_get = -1;
        g_ret_fcntl_set = -1;
        REQUIRE(-1 == create_file_namepipe(&fp_from, &fp_to));
        g_ret_fork = 1;
        g_ret_fopen = 1;
        g_ret_fcntl_get = 0;
        g_ret_fcntl_set = -1;
        REQUIRE(-1 == create_file_namepipe(&fp_from, &fp_to));
        g_ret_fork = 1;
        g_ret_fopen = 2;
        g_ret_fcntl_get = 0;
        g_ret_fcntl_set = 0;
        REQUIRE(g_ret_fork == create_file_namepipe(&fp_from, &fp_to));
    }
    SECTION("test destroy_file_namepipe") {
        g_ret_fork = -1;
        g_ret_fopen = -1;
        g_ret_fclose = -1;
        g_ret_fcntl_get = -1;
        g_ret_fcntl_set = -1;
        g_ret_wait = -1;
        g_ret_remove = -1;

        CIUT_LOG ("test %s", "child");
        g_ret_fork = 0;
        g_ret_fclose = -1;
        REQUIRE(-8 == destroy_file_namepipe(g_ret_fork, &fp_from, &fp_to));
        g_ret_fork = 0;
        g_ret_fclose = 0;
        REQUIRE(0 == destroy_file_namepipe(g_ret_fork, &fp_from, &fp_to));

        CIUT_LOG ("test %s", "parent");
        g_ret_fork = 1;
        g_ret_wait = -1;
        REQUIRE(-10 == destroy_file_namepipe(g_ret_fork, &fp_from, &fp_to));
        g_ret_fork = 1;
        g_ret_wait = 0;
        g_ret_fclose = -1;
        REQUIRE(-8 == destroy_file_namepipe(g_ret_fork, &fp_from, &fp_to));
        g_ret_fork = 1;
        g_ret_wait = 0;
        g_ret_fclose = 0;
        g_ret_remove = -1;
        REQUIRE(-9 == destroy_file_namepipe(g_ret_fork, &fp_from, &fp_to));
        g_ret_fork = 1;
        g_ret_wait = 0;
        g_ret_fclose = 0;
        g_ret_remove = 0;
        REQUIRE(0 == destroy_file_namepipe(g_ret_fork, &fp_from, &fp_to));
    }

    SECTION("test read_and_compare parameters") {
        g_ret_strcmp = -1;
        g_ret_malloc = NULL;

        //REQUIRE(-1 == read_and_compare(NULL,  NULL));
        //REQUIRE(-1 == read_and_compare(stdin, NULL));
        //REQUIRE(-1 == read_and_compare(NULL,  ""));

        g_ret_malloc = NULL;
        REQUIRE(-1 == read_and_compare(stdin, ""));
        g_ret_malloc = 1;
        g_ret_getline = 0;
        REQUIRE(-1 == read_and_compare(stdin, ""));
        g_ret_malloc = 1;
        g_ret_getline = 1;
        g_ret_strcmp = -1;
        REQUIRE(-1 == read_and_compare(stdin, ""));
        g_ret_malloc = 1;
        g_ret_getline = 1;
        g_ret_strcmp = 0;
        REQUIRE(0 == read_and_compare(stdin, ""));
        g_ret_malloc = 1;
        g_ret_getline = 1;
        g_ret_strcmp = 1;
        REQUIRE(1 == read_and_compare(stdin, ""));
    }
}

#endif /* CIUT_ENABLED */
#endif /* _WIN32 */

