/**
 * @file    ucport.h
 * @brief   An universal library portable for C and C++
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @date    2017-12-06
 * @copyright MIT License
 */
#ifndef _UNI_C_PORTING_H
#define _UNI_C_PORTING_H 1

#include <stdlib.h>    /* size_t */
#include <sys/types.h> /* ssize_t */

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

////////////////////////////////////////////////////////////////////////////////
// printf formats
#include <inttypes.h> /* for PRIdPTR PRIiPTR PRIoPTR PRIuPTR PRIxPTR PRIXPTR, SCNdPTR SCNiPTR SCNoPTR SCNuPTR SCNxPTR */
#ifdef __WIN32__                /* or whatever */
#define PRIiSZ "ld"
#define PRIuSZ "Iu"
#else
#define PRIiSZ "zd"
#define PRIuSZ "zu"
#define PRIxSZ "zx"
#define SCNxSZ "zx"
#endif
#define PRIiOFF PRIx64 /*"lld"*/
#define PRIuOFF PRIx64 /*"llu"*/

////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* _UNI_C_PORTING_H */

