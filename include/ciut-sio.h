/**
 * \file    ciut-sio.h
 * \brief   the CIUT functions for checking FILE output
 * \author  Yunhui Fu <yhfudev@gmail.com>
 * \version 1.0
 */
#ifndef _CIUT_SIO_H
#define _CIUT_SIO_H 1

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <assert.h>

#include "ucport.h"

#ifndef EXIT_FAILURE
#define EXIT_FAILURE -1
#endif // EXIT_FAILURE

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifdef _WIN32
#define ciut_check_output(...) (0)

#else // WIN32
#include <sys/wait.h> // wait()
#include <sys/stat.h> // mkfifo()

//int ciut_check_output(int (* cb_output)(FILE * outf, void * user_arg), void * user_arg, const char * output_expected);

#if 0
/**
 * \brief Create a anonymous pipe and return the pid and file pointers
 * \param fp_from: the file pointer for writing
 * \param fp_to: the file pointer for reading
 *
 * \return pid>=0 on successs, <0 on failed; child pid==0, and should the writing file pointer(fp_from), parent pid>0 and should use the reading file pointer.
 *
 */
static pid_t
create_file_anonpipe (FILE ** fp_from, FILE ** fp_to)
{
    FILE *fp_r = NULL;
    FILE *fp_w = NULL;
    int pfds[2] = {-1, -1};
    int flags = 0;
    pid_t pid;

    if (pipe(pfds) == -1) {
        perror(CIUT_LOGHDR "MAIN: pipe");
        return -1;
    }

    if ((pid = fork()) < 0) {
        perror(CIUT_LOGHDR "fork failed");
        goto err_create_apipe;
    }

    if (pid == (pid_t)0) {        /* CHILD process */
        /* issue fopen for write end of the fifo */

        fp_w = fdopen(pfds[1], "w");
        if (!fp_w) {
            perror(CIUT_LOGHDR "fdopen write stream:");
            goto err_create_apipe;
        }
        assert (NULL != fp_w);
        assert (NULL != fp_from);
        *fp_from = fp_w;

    } else  {                    /* PARENT  process */
        /* issue fopen for read */
        fprintf(stderr, CIUT_LOGHDR "open pipe for reading ...\n");

        fp_r = fdopen(pfds[0], "r");
        if (!fp_r) {
            perror(CIUT_LOGHDR "fdopen read stream:");
            goto err_create_apipe;
        }

        fprintf(stderr, CIUT_LOGHDR "change pipe flags ...\n");
        /* get current flag settings of file                      */
        if ((flags = fcntl(fileno(fp_r), F_GETFL)) == -1) {
            fprintf(stderr, CIUT_LOGHDR "fcntl returned -1\n");
            goto err_create_apipe;
        }
        /* clear O_NONBLOCK  and reset file flags                 */
        flags &= (O_NONBLOCK);
        if ((fcntl(fileno(fp_r), F_SETFL,flags)) == -1) {
            fprintf(stderr, CIUT_LOGHDR "fcntl returned -1\n");
            goto err_create_apipe;
        }
        assert (NULL != fp_r);
        assert (NULL != fp_to);
        *fp_to = fp_r;
    }

    return pid;

err_create_apipe:
    if (NULL != fp_r) {
        fclose(fp_r);
    }
    if (NULL != fp_w) {
        fclose(fp_w);
    }
    if (pfds[0] >= 0) {
        close (pfds[0]);
    }
    if (pfds[1] >= 0) {
        close (pfds[1]);
    }

    return -1;
}

static int
destroy_file_anonpipe(pid_t mypid, FILE * fp_from, FILE * fp_to)
{
    FILE *fp_r = fp_to;
    FILE *fp_w = fp_from;
    int fd;

    pid_t cpid;
    int c_status;

    if (mypid == (pid_t)0) {        /* CHILD process */
        assert (NULL != fp_from);
        assert (NULL != fp_w);

        fd = fileno(fp_w);
        if (fclose(fp_w) != 0)  {
            fprintf(stderr, CIUT_LOGHDR "Fclose failed\n");
            fprintf(stderr, CIUT_LOGHDR "errno is %d\n", errno);
            return (-8);
        }
        close(fd);

    } else  {                    /* PARENT  process */
        assert (NULL != fp_to);
        assert (NULL != fp_r);

        cpid = wait(&c_status);
        if ((WIFEXITED(c_status) != 0) && (WEXITSTATUS(c_status) != 0)) {
            fprintf(stderr, CIUT_LOGHDR "child %d exited with code %d\n", cpid, WEXITSTATUS(c_status));
            return (-10);
        }
        fd = fileno(fp_r);

        if (fclose(fp_r) != 0) {
            fprintf(stderr, CIUT_LOGHDR "Fclose failed\N");
            fprintf(stderr, CIUT_LOGHDR "errno is %d\N", errno);
            return (-8);
        }
        close(fd);
    }
    return 0;
}
#endif // 0

#define FN_FIFO "cuit-io-namedfifo"

/**
 * \brief Create a named pipe and return the pid and file pointers
 * \param fp_from: the file pointer for writing
 * \param fp_to: the file pointer for reading
 *
 * \return pid>=0 on successs, <0 on failed; child pid==0, and should the writing file pointer(fp_from), parent pid>0 and should use the reading file pointer.
 *
 */
static pid_t
create_file_namepipe (FILE ** fp_from, FILE ** fp_to)
{
    FILE *fp_r = NULL;
    FILE *fp_w = NULL;
    int flags = 0;
    pid_t pid;
    int ret;

    fprintf(stderr, CIUT_LOGHDR "create name pipe '%s' ...\n", FN_FIFO);
    //unlink(FN_FIFO);
    ret = mkfifo(FN_FIFO, S_IRWXU);
    //if (0 != ret) {
    //    fprintf(stderr, CIUT_LOGHDR "unable to create name pipe '%s' ...\n", FN_FIFO);
    //    return -1;
    //}

    if ((pid = fork()) < 0) {
        perror(CIUT_LOGHDR "fork failed");
        goto err_create_npipe;
    }

    if (pid == (pid_t)0) {        /* CHILD process */
        /* issue fopen for write end of the fifo */
        fprintf(stderr, CIUT_LOGHDR "child open pipe '%s' for writing ...\n", FN_FIFO);
        fp_w = fopen(FN_FIFO, "w");
        if (!fp_w) {
            perror(CIUT_LOGHDR "child fdopen write stream");
            exit(EXIT_FAILURE);
        }
        assert (NULL != fp_w);
        assert (NULL != fp_from);
        *fp_from = fp_w;
    } else  {                    /* PARENT  process */
        /* issue fopen for read */

        fprintf(stderr, CIUT_LOGHDR "parent open pipe '%s' for reading ...\n", FN_FIFO);
        fp_r = fopen(FN_FIFO, "r");
        if (!fp_r) {
            perror(CIUT_LOGHDR "parent fdopen read stream");
            goto err_create_npipe;
        }
        assert (NULL != fp_r);

        fprintf(stderr, CIUT_LOGHDR "parent change pipe '%s' flags ...\n", FN_FIFO);
        /* get current flag settings of file                      */
        if ((flags = fcntl(fileno(fp_r), F_GETFL)) == -1) {
            fprintf(stderr, CIUT_LOGHDR "parent fcntl returned -1 for %s\n", FN_FIFO);
            goto err_create_npipe;
        }
        assert (NULL != fp_r);

        /* clear O_NONBLOCK  and reset file flags                 */
        flags &= (O_NONBLOCK);
        if ((fcntl(fileno(fp_r), F_SETFL,flags)) == -1) {
            fprintf(stderr, CIUT_LOGHDR "parent fcntl returned -1 for %s\n", FN_FIFO);
            goto err_create_npipe;
        }
        assert (NULL != fp_r);
        assert (NULL != fp_to);
        *fp_to = fp_r;
    }

    return pid;

err_create_npipe:
    remove(FN_FIFO);
    if (NULL != fp_r) {
        fclose(fp_r);
    }
    if (NULL != fp_w) {
        fclose(fp_w);
    }

    return -1;
}

static int
destroy_file_namepipe(pid_t mypid, FILE * fp_from, FILE * fp_to)
{
    FILE *fp_r = fp_to;
    FILE *fp_w = fp_from;

    pid_t cpid;
    int c_status;

    if (mypid == (pid_t)0) {        /* CHILD process */

        assert (NULL != fp_from);
        assert (NULL != fp_w);
        if (fclose(fp_w) != 0)  {
            fprintf(stderr, CIUT_LOGHDR "Fclose failed for %s\n", FN_FIFO);
            fprintf(stderr, CIUT_LOGHDR "errno is %d\n", errno);
            return (-8);
        }

    } else  {                    /* PARENT  process */
        assert (NULL != fp_to);
        assert (NULL != fp_r);
        cpid = wait(&c_status);
        if ((WIFEXITED(c_status) != 0) && (WEXITSTATUS(c_status) != 0)) {
            fprintf(stderr, CIUT_LOGHDR "child %d exited with code %d\n", cpid, WEXITSTATUS(c_status));
            return -10;
        }

        assert (NULL != fp_to);
        assert (NULL != fp_r);
        if (fclose(fp_r) != 0) {
            fprintf(stderr, CIUT_LOGHDR "Fclose failed for %s\n", FN_FIFO);
            fprintf(stderr, CIUT_LOGHDR "errno is %d\n", errno);
            return (-8);
        }

        if (remove(FN_FIFO) != 0) {
            fprintf(stderr, CIUT_LOGHDR "remove failed for %s\n", FN_FIFO);
            fprintf(stderr, CIUT_LOGHDR "errno is %d\n", errno);
            return (-9);
        }
    }
    return 0;
}

// return 0 on success
static int
read_and_compare(FILE *fp, const char * expected)
{
    int ret;
    char * buf_all = NULL;
    char * buffer = NULL;
    size_t szbuf = 0;

    assert (NULL != fp);
    assert (NULL != expected);

    szbuf = strlen(expected) + 20;
    if (szbuf < 1000) {
        szbuf = 1000;
    }
    buffer = (char *)malloc(szbuf);
    if (NULL == buffer) {
        return -1;
    }
    buf_all = (char *)malloc(szbuf * 2);
    if (NULL == buf_all) {
        free (buffer);
        return -1;
    }
    buf_all[0] = 0;

    ret = getline ( &buffer, &szbuf, fp );
    while ( ret > 0 ) {
        strcat(buf_all, buffer);
        assert (strlen(buf_all) < szbuf * 2);
        ret = getline ( &buffer, &szbuf, fp );
    }
    free (buffer);

    ret = strcmp(buf_all, expected);
    if (0 != ret) {
        fprintf(stderr, CIUT_LOGHDR "[%s()] Error in check output: ret=%d\n", __func__, ret);
        fprintf(stderr, CIUT_LOGHDR "[%s()] expected string:\n'%s'\n", __func__, expected);
        fprintf(stderr, CIUT_LOGHDR "[%s()] recv string:\n'%s'\n", __func__, buf_all);
    }

    free (buf_all);
    return ret;
}

/**
 * \brief check if the output of the function is the same as pre-defined
 * \param cb_output: the callback function to to generate the result and save to a FILE
 * \param user_arg: the input for the callback
 * \param output_expected: the expected generated output of the callback function
 * \return 0 on success
 */
static int
ciut_check_output(int (* cb_output)(FILE * outf, void * user_arg), void * user_arg, const char * output_expected)
{
    pid_t pid;
    FILE * fp_r = NULL;
    FILE * fp_w = NULL;
    int ret = 0;

    assert (NULL != cb_output);
    assert (NULL != output_expected);

#if 1
    pid_t (*cb_create_pipe)(FILE ** fp_from, FILE ** fp_to) = create_file_namepipe;
    int  (*cb_destroy_pipe)(pid_t mypid, FILE * fp_from, FILE * fp_to) = destroy_file_namepipe;
#else
    pid_t (*cb_create_pipe)(FILE ** fp_from, FILE ** fp_to) = create_file_anonpipe;
    int  (*cb_destroy_pipe)(pid_t mypid, FILE * fp_from, FILE * fp_to) = destroy_file_anonpipe;
#endif

    if ((pid = cb_create_pipe(&fp_w, &fp_r)) < 0) {
        perror(CIUT_LOGHDR "fork failed");
        return -1;
    }
    assert ((NULL != fp_w) || (NULL != fp_r));

    if (pid == (pid_t)0) {        /* CHILD process */
        /* issue fopen for write end of the fifo */

        fprintf(stderr, CIUT_LOGHDR "send ...\n");
        assert (NULL != fp_w);
        cb_output(fp_w, user_arg);

    } else  {                    /* PARENT  process */
        /* issue fopen for read */

        fprintf(stderr, CIUT_LOGHDR "recv ...\n");
        assert (NULL != fp_r);
        ret = read_and_compare(fp_r, output_expected);
    }

    assert ((NULL != fp_w) || (NULL != fp_r));
    cb_destroy_pipe(pid, fp_w, fp_r);
    if (pid == 0) {
        exit (0);
    }

    return ret;
}
#endif // WIN32

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* _CIUT_SIO_H */
