/**
 * @file    ciut-sio.h
 * @brief   the CIUT functions for checking FILE output
 * @author  Yunhui Fu <yhfudev@gmail.com>
 * @version 1.0
 */
#ifndef _CIUT_SIO_H
#define _CIUT_SIO_H 1

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h> // wait()
#include <sys/stat.h> // mkfifo()
#include <assert.h>

#ifndef EXIT_FAILURE
#define EXIT_FAILURE -1
#endif // EXIT_FAILURE

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//int cuit_check_output(int (* cb_output)(FILE * outf, void * user_arg), void * user_arg, const char * output_expected);

/**
 * @brief Create a anonymous pipe and return the pid and file pointers
 * @param fp_from: the file pointer for writing
 * @param fp_to: the file pointer for reading
 *
 * @return pid>=0 on successs, <0 on failed; child pid==0, and should the writing file pointer(fp_from), parent pid>0 and should use the reading file pointer.
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

    assert (NULL != fp_from);
    assert (NULL != fp_to);

    if (pipe(pfds) == -1) {
        perror("MAIN: pipe");
        return -1;
    }

    if ((pid = fork()) < 0) {
        perror("fork failed");
        goto err_create_apipe;
    }

    if (pid == (pid_t)0) {        /* CHILD process */
        /* issue fopen for write end of the fifo */

        fp_w = fdopen(pfds[1], "w");
        if (!fp_w) {
            perror("fdopen write stream:");
            goto err_create_apipe;
        }

    } else  {                    /* PARENT  process */
        /* issue fopen for read */
        fprintf(stderr, "open pipe for reading ...\n");

        fp_r = fdopen(pfds[0], "r");
        if (!fp_r) {
            perror("fdopen read stream:");
            goto err_create_apipe;
        }

        fprintf(stderr, "change pipe flags ...\n");
        /* get current flag settings of file                      */
        if ((flags = fcntl(fileno(fp_r), F_GETFL)) == -1) {
            printf("fcntl returned -1\n");
            goto err_create_apipe;
        }
        /* clear O_NONBLOCK  and reset file flags                 */
        flags &= (O_NONBLOCK);
        if ((fcntl(fileno(fp_r), F_SETFL,flags)) == -1) {
            printf("\nfcntl returned -1");
            goto err_create_apipe;
        }
    }

    *fp_from = fp_w;
    *fp_to = fp_r;
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

static void
destroy_file_anonpipe(pid_t mypid, FILE * fp_from, FILE * fp_to)
{
    FILE *fp_r = fp_to;
    FILE *fp_w = fp_from;
    int fp;

    pid_t cpid;
    int c_status;

    if (mypid == (pid_t)0) {        /* CHILD process */
        fp = fileno(fp_r);
        if (fclose(fp_w) != 0)  {
            printf("\nFclose failed");
            printf("\nerrno is %d", errno);
            exit(8);
        }
        close(fp);

    } else  {                    /* PARENT  process */

        cpid = wait(&c_status);
        if ((WIFEXITED(c_status) !=0) && (WEXITSTATUS(c_status) !=0)) {
            printf("\nchild exited with code %d",WEXITSTATUS(c_status));
            exit(10);
        }
        fp = fileno(fp_r);

        if (fclose(fp_r) != 0) {
            printf("\nFclose failed");
            printf("\nerrno is %d", errno);
            exit(8);
        }
        close(fp);
    }
}

#define FN_FIFO "myfifo"

/**
 * @brief Create a named pipe and return the pid and file pointers
 * @param fp_from: the file pointer for writing
 * @param fp_to: the file pointer for reading
 *
 * @return pid>=0 on successs, <0 on failed; child pid==0, and should the writing file pointer(fp_from), parent pid>0 and should use the reading file pointer.
 *
 */
static pid_t
create_file_namepipe (FILE ** fp_from, FILE ** fp_to)
{
    FILE *fp_r = NULL;
    FILE *fp_w = NULL;
    int flags = 0;
    pid_t pid;

    assert (NULL != fp_from);
    assert (NULL != fp_to);

    fprintf(stderr, "create name pipe '%s' ...\n", FN_FIFO);
    if (mkfifo(FN_FIFO, S_IRWXU) != 0) {
        fprintf(stderr, "Unable to create a fifo; errno=%d\n", errno);
        return -1;
    }

    if ((pid = fork()) < 0) {
        perror("fork failed");
        goto err_create_npipe;
    }

    if (pid == (pid_t)0) {        /* CHILD process */
        /* issue fopen for write end of the fifo */

        fprintf(stderr, "open pipe '%s' for writing ...\n", FN_FIFO);
        fp_w = fopen(FN_FIFO, "w");
        if (!fp_w) {
            perror("fdopen write stream:");
            exit(EXIT_FAILURE);
        }

    } else  {                    /* PARENT  process */
        /* issue fopen for read */

        fprintf(stderr, "open pipe '%s' for reading ...\n", FN_FIFO);
        fp_r = fopen(FN_FIFO, "r");
        if (!fp_r) {
            perror("fdopen read stream:");
            goto err_create_npipe;
        }
        fprintf(stderr, "change pipe '%s' flags ...\n", FN_FIFO);
        /* get current flag settings of file                      */
        if ((flags = fcntl(fileno(fp_r), F_GETFL)) == -1) {
            printf("fcntl returned -1 for %s\n", FN_FIFO);
            goto err_create_npipe;
        }
        /* clear O_NONBLOCK  and reset file flags                 */
        flags &= (O_NONBLOCK);
        if ((fcntl(fileno(fp_r), F_SETFL,flags)) == -1) {
            printf("\nfcntl returned -1 for %s", FN_FIFO);
            goto err_create_npipe;
        }
    }

    *fp_from = fp_w;
    *fp_to = fp_r;
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

static void
destroy_file_namepipe(pid_t mypid, FILE * fp_from, FILE * fp_to)
{
    FILE *fp_r = fp_to;
    FILE *fp_w = fp_from;

    pid_t cpid;
    int c_status;

    if (mypid == (pid_t)0) {        /* CHILD process */

        if (fclose(fp_w) != 0)  {
            printf("\nFclose failed for %s", FN_FIFO);
            printf("\nerrno is %d", errno);
            exit(8);
        }

    } else  {                    /* PARENT  process */

        cpid = wait(&c_status);
        if ((WIFEXITED(c_status) !=0) && (WEXITSTATUS(c_status) !=0)) {
            printf("\nchild exited with code %d",WEXITSTATUS(c_status));
            exit(10);
        }

        if (fclose(fp_r) != 0) {
            printf("\nFclose failed for %s", FN_FIFO);
            printf("\nerrno is %d", errno);
            exit(8);
        }
        if (remove(FN_FIFO) != 0) {
            printf("\nremove failed for %s", FN_FIFO);
            printf("\nerrno is %d", errno);
            exit(9);
        }
    }
}

// return 0 on success
static int
read_and_compare(FILE *fp, const char * expected)
{
    int ret;
    char buf_all[1000] = "";
    char * buffer = NULL;
    size_t szbuf = 0;
    szbuf = 1000;
    buffer = (char *) malloc (szbuf);
    if (NULL == buffer) {
        return -1;
    }
    ret = getline ( &buffer, &szbuf, fp );
    while ( ret > 0 ) {
        strcat(buf_all, buffer);
        ret = getline ( &buffer, &szbuf, fp );
    }
    free (buffer);
    fprintf(stderr, "[test] recv string:\n%s", buf_all);

    return strcmp(expected, buf_all);
}

/**
 * @brief check if the output of the function is the same as pre-defined
 * @param cb_output: the callback function to to generate the result and save to a FILE
 * @param user_arg: the input for the callback
 * @param output_expected: the expected generated output of the callback function
 * return 0 on success
 */
static int
cuit_check_output(int (* cb_output)(FILE * outf, void * user_arg), void * user_arg, const char * output_expected)
{
    pid_t pid;
    FILE * fp_r = NULL;
    FILE * fp_w = NULL;
    int ret = 0;

    assert (NULL != cb_output);
    assert (NULL != output_expected);

#if 1
    pid_t (*cb_create_pipe)(FILE ** fp_from, FILE ** fp_to) = create_file_namepipe;
    void  (*cb_destroy_pipe)(pid_t mypid, FILE * fp_from, FILE * fp_to) = destroy_file_namepipe;
#else
    pid_t (*cb_create_pipe)(FILE ** fp_from, FILE ** fp_to) = create_file_anonpipe;
    void  (*cb_destroy_pipe)(pid_t mypid, FILE * fp_from, FILE * fp_to) = destroy_file_anonpipe;
#endif

    if ((pid = cb_create_pipe(&fp_w, &fp_r)) < 0) {
        perror("fork failed");
        return -1;
    }

    if (pid == (pid_t)0) {        /* CHILD process */
        /* issue fopen for write end of the fifo */

        fprintf(stderr, "send ...\n");
        cb_output(fp_w, user_arg);

    } else  {                    /* PARENT  process */
        /* issue fopen for read */

        fprintf(stderr, "recv ...\n");
        ret = read_and_compare(fp_r, output_expected);
    }

    cb_destroy_pipe(pid, fp_w, fp_r);
    if (pid == 0) {
        exit (0);
    }

    return ret;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* _CIUT_SIO_H */
