/**
 * @file    uctime.h
 * @brief   An universal time lib for C and C++
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @date    2017-12-06
 * @copyright MIT License
 */
#ifndef _UNI_C_TIME_H
#define _UNI_C_TIME_H 1

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <syslog.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

////////////////////////////////////////////////////////////////////////////////
// time
#if defined(_WIN32) || defined(__TURBOC__)

#if defined(_WIN32)
#include <windows.h>
#include <sys/timeb.h>

// struct tm * localtime_r(const time_t *timep, struct tm *result);
#define localtime_r(timep, result) ((localtime_s(result, timep) == 0) ? result : NULL)
#endif

//#if !defined(__GNUC__) && !defined(_WINSOCKAPI_)        // for WIN32 winsock[2].h
#if !defined(_WINSOCKAPI_)        // for WIN32 winsock[2].h
struct timeval
{
    long tv_sec;
    long tv_usec;
};
#endif

#ifndef HAVE_STRUCT_TIMEZONE
struct timezone
{
    int tz_minuteswest;         /* minutes west of Greenwich */
    int tz_dsttime;             /* type of dst correction */
};
#define HAVE_STRUCT_TIMEZONE 1
#endif

//int gettimeofday (struct timeval *tvp, struct timezone *tzp);

inline int
gettimeofday (struct timeval *tvp, struct timezone *tzp)
{
    if (tvp) {
        struct _timeb timebuffer;

        _ftime (&timebuffer);
        tvp->tv_sec = timebuffer.time;
        tvp->tv_usec = timebuffer.millitm * 1000;
    }
    if (tzp) {
        TIME_ZONE_INFORMATION tz;
        GetTimeZoneInformation (&tz);
        tzp->tz_minuteswest = tz.Bias;
        tzp->tz_dsttime = tz.StandardBias != tz.Bias;
    }
    return 0;
}

#else // linux
#include <sys/time.h>
#endif // _WIN32, __TURBOC__


#if __cplusplus
//extern int timeval_sub (struct timeval *ptv1, struct timeval *ptv2, struct timeval *ptv_ret_1sub2);

/**
 * @brief get the difference between two 'struct timeval'
 * @param tva the timeval to be substracted
 * @param tvb the substract value
 * @param ret the return value
 * @return 0 on success
 */
inline
int timeval_sub(struct timeval * tva, struct timeval * tvb, struct timeval * ret)
{
    assert (NULL != tva);
    assert (NULL != tvb);
    assert (NULL != ret);
    assert ((0 <= tva->tv_usec) && (tva->tv_usec < 1000000));
    assert ((0 <= tvb->tv_usec) && (tvb->tv_usec < 1000000));

    ret->tv_sec = tva->tv_sec - tvb->tv_sec;
    ret->tv_usec = tva->tv_usec - tvb->tv_usec;
    if (ret->tv_usec < 0) {
        ret->tv_usec += 1000000;
        ret->tv_sec --;
    }
    assert ((0 <= ret->tv_usec) && (ret->tv_usec < 1000000));
    return 0;
}
#else
#define timeval_sub(tva, tvb, ret) \
{ \
    assert (NULL != (tva)); \
    assert (NULL != (tvb)); \
    assert (NULL != (ret)); \
    assert ((0 <= (tva)->tv_usec) && ((tva)->tv_usec < 1000000)); \
    assert ((0 <= (tvb)->tv_usec) && ((tvb)->tv_usec < 1000000)); \
    (ret)->tv_sec = (tva)->tv_sec - (tvb)->tv_sec; \
    (ret)->tv_usec = (tva)->tv_usec - (tvb)->tv_usec; \
    if ((ret)->tv_usec < 0) { \
        (ret)->tv_usec += 1000000; \
        (ret)->tv_sec --; \
    } \
    assert ((0 <= (ret)->tv_usec) && ((ret)->tv_usec < 1000000)); \
}
#endif

#define timeval2millisec(ptv) ((ptv)->tv_sec * 1000 + ((ptv)->tv_usec / 1000) + (((ptv)->tv_usec % 1000)>499?1:0))

////////////////////////////////////////////////////////////////////////////////
// measure the run time:

#if 1
#define TMC_BEGIN() { struct timeval tmc_tmp_starttime; gettimeofday (&tmc_tmp_starttime, NULL)
#define TMC_END(ret_millisecond) { \
    struct timeval tmp1; \
    struct timeval tmp2; \
    gettimeofday (&tmp1, NULL); \
    timeval_sub (&tmp1, &tmc_tmp_starttime, &tmp2); \
    (ret_millisecond) = timeval2millisec(&tmp2); \
    /*printf("start=%d.%06d; tmp1=%d.%06d; tmp2=%d.%06d; millisec=%d\n", tmc_tmp_starttime.tv_sec, tmc_tmp_starttime.tv_usec, tmp1.tv_sec, tmp1.tv_usec, tmp2.tv_sec, tmp2.tv_usec, (ret_millisecond)); /**/ \
}}
#endif

#ifndef TMC_BEGIN
#define TMC_BEGIN()
#define TMC_END(ret_millisecond)
#endif

////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* _UNI_C_TIME_H */

