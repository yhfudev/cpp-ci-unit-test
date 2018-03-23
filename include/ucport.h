/**
 * \file    ucport.h
 * \brief   An universal library portable for C and C++
 * \author  Yunhui Fu (yhfudev@gmail.com)
 * \version 1.0
 * \date    2017-12-06
 * \copyright MIT License
 */
#ifndef _UNI_C_PORTING_H
#define _UNI_C_PORTING_H 1

#define CUIT_LOGHDR "[cuit] "

#include <stdlib.h>    /* size_t */
#include <sys/types.h> /* ssize_t pid_t off64_t */
#include <stdint.h> // uint8_t

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#ifndef fsync
#define fsync(fd) _commit(fd)
#endif
#ifndef u_int8_t
#define u_int8_t uint8_t
#define u_int16_t uint16_t
#define u_int32_t uint32_t
#define u_int64_t uint64_t
#endif

#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // inet_ntoa()
#endif


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

////////////////////////////////////////////////////////////////////////////////
// printf formats
#include <inttypes.h> /* for PRIdPTR PRIiPTR PRIoPTR PRIuPTR PRIxPTR PRIXPTR, SCNdPTR SCNiPTR SCNoPTR SCNuPTR SCNxPTR */
#ifndef PRIuSZ
#ifdef __WIN32__                /* or whatever */
#define PRIiSZ "Id"
#define PRIuSZ "Iu"
#else
#define PRIiSZ "zd"
#define PRIuSZ "zu"
#define PRIxSZ "zx"
#define SCNxSZ "zx"
#endif
#define PRIiOFF PRIx64 /*"lld"*/
#define PRIuOFF PRIx64 /*"llu"*/
#endif // PRIuSZ

////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* _UNI_C_PORTING_H */

