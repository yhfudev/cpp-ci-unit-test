/**
 * \file    uclog.h
 * \brief   An universal log framework for C and C++
 * \author  Yunhui Fu (yhfudev@gmail.com)
 * \version 1.0
 * \date    2017-12-06
 * \copyright MIT License
 */
#ifndef _UNI_C_LOG_H
#define _UNI_C_LOG_H 1

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif
#include <stdio.h>
#include <string.h>
#include <sys/types.h> // off64_t

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


#ifdef _WIN32
#define syslog(...)
#define openlog(...)
#define MYLOG_INIT_2SYSLOG() MYLOG_INIT_2STDERR()
enum _syslog_error_t {
    LOG_EMERG,
    LOG_ALERT,
    LOG_CRIT,
    LOG_ERR,
    LOG_WARNING,
    LOG_NOTICE,
    LOG_INFO,
    LOG_DEBUG,
};
#define LOG_PID 1

#else // WIN32
#include <syslog.h>

// redirect stderr to syslog:
static char const *priov[] = {
    "EMERG:",   "ALERT:",  "CRIT:", "ERR:", "WARNING:", "NOTICE:", "INFO:", "DEBUG:"
};

static ssize_t writer(void *cookie, char const *data, size_t leng)
{
    (void)cookie;
    int     p = LOG_DEBUG, len;
    if (NULL == data) {
        return 0;
    }
    do len = strlen(priov[p]);
    while (memcmp(data, priov[p], len) && --p >= 0);

    if (p < 0) p = LOG_INFO;
    else data += len, leng -= len;
    while (*data == ' ') ++data, --leng;

    syslog(p, "%.*s", (int)leng, data);
    return leng;
}

#if defined(__FreeBSD__)
// freebsd
#define fsyslog(pfp) *(pfp) = fwopen(NULL, writer)
#else
static ssize_t noop1(void* a, char* b, size_t c){return 0;}
static int noop3(void* a, off64_t* b, int c) {return 0;}
static int noop4(void* a) {return 0;}

static cookie_io_functions_t log_fns = {
    noop1, writer, noop3, noop4,
};

#define fsyslog(pfp) setvbuf(*(pfp) = fopencookie(NULL, "w", log_fns), NULL, _IOLBF, 0);
#endif
// example:
// void fsyslog(FILE **pfp)
// fsyslog(&stderr);
// fprintf(stderr, "ALERT: this is a test alert message!\n");

#define MYLOG_INIT_2SYSLOG() fsyslog(&stdlog)
#endif // WIN32

#define MYLOG_INIT_2STDERR() stdlog=stderr

extern FILE * stdlog; //= NULL;
#define MYLOG_GLOBAL_INIT FILE * stdlog = NULL
// log trace: code trace for dev
#define MLT(fmt, ...) fprintf(stdlog, "DEBUG:"  "[%s()]\t" fmt "\t{%d," __FILE__ "}\n", __func__, ##__VA_ARGS__, __LINE__)
// log debug: diagnostically
#define MLD(fmt, ...) fprintf(stdlog, "INFO:"   "[%s()]\t" fmt "\t{%d," __FILE__ "}\n", __func__, ##__VA_ARGS__, __LINE__)
// log info: general info
#define MLI(fmt, ...) fprintf(stdlog, "NOTICE:"  fmt "\n", ##__VA_ARGS__)
// log warning: potentially cause problem
#define MLW(fmt, ...) fprintf(stdlog, "WARNING:" fmt "\n", ##__VA_ARGS__)
// log error: function error
#define MLE(fmt, ...) fprintf(stdlog, "ERR:"     fmt "\n", ##__VA_ARGS__)
// log fatal: cause shutdown of service
#define MLF(fmt, ...) fprintf(stdlog, "EMERG:"   fmt "\n", ##__VA_ARGS__)
// Would you want the message to get a system administrator out of bed in the middle of the night? YES -- fatal, NO -- warning/error, errors need investigation
//
// example1: to syslog
// MYLOG_GLOBAL_INIT;
// ... main ...
// setlogmask (LOG_UPTO (LOG_NOTICE));
// openlog ("myapp", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
// MYLOG_INIT_2SYSLOG(); // OR: fsyslog(&stderr); MYLOG_INIT_2STDERR();
// MLT("the network port=%d", port);
// MLF("myapp-F-net-3013 network failed!");
// closelog ();
//
// example2: to console only
// MYLOG_GLOBAL_INIT;
// ... main ...
// MYLOG_INIT_2STDERR();
// MLT("the network port=%d", port);

// bash example: Redirect STDOUT/STDERR into syslog
// # exec > >(logger -p user.info) 2> >(logger -p user.warn)

// std::clog to syslog: https://stackoverflow.com/questions/2638654/redirect-c-stdclog-to-syslog-on-unix
//   or https://github.com/yurial/libsyslogpp.git
// syslog to file: https://stackoverflow.com/questions/10952515/c-c-syslog-to-custom-file-not-var-log-syslog-but-var-log-mylog-ubuntu-12

// localized log prefix format:
// APP-S-CODE or APP-S-SUB-CODE, with respectively:
// APP: your application name on 3 letters
// S: severity on 1 letter (ie D: debug, I: info…)
// SUB: the sub part of the application this code pertains to
// CODE: a numeric code specific to the error in question
// sample: 2013-01-12 17:49:37,656 [T1] INFO  c.d.g.UserRequest  User plays {'user':1334563, 'card':'4 of spade', 'game':23425656}

// for java, use slf4j and logback.
// ref: http://www.masterzen.fr/2013/01/13/the-10-commandments-of-logging/

#ifdef __cplusplus
}
#endif // __cplusplus

// C++ part
#ifdef __cplusplus
#include <ostream>

/**
 * \brief stream for log
 * samples:
 *   std::clog.rdbuf(new Syslog::sbuf("foo", LOG_LOCAL0));
 *   std::clog << Syslog::Notice << "myprog-E-net-0003 test log message" << std::endl;
 *   std::clog << "myprog-E-net-0003 the default is debug level" << std::endl;
 */
namespace Syslog {
    /** \brief the priorities of the logs */
    enum Priority {
        Emergent    = LOG_EMERG,   ///< system is unusable
        Alert       = LOG_ALERT,   ///< action must be taken immediately
        Critical    = LOG_CRIT,    ///< critical conditions
        Error       = LOG_ERR,     ///< error conditions
        Warning     = LOG_WARNING, ///< warning conditions
        Notice      = LOG_NOTICE,  ///< normal, but significant, condition
        Infomation  = LOG_INFO,    ///< informational message
        Debug       = LOG_DEBUG,   ///< debug-level message
    };

    std::ostream& operator<< (std::ostream& os, const Syslog::Priority& log_priority);

class sbuf: public std::basic_streambuf<char, std::char_traits<char> > {
public:

    explicit sbuf(std::string ident, int facility);

protected:
    int sync();
    int overflow(int c);

private:
    friend std::ostream& operator<< (std::ostream& os, const Syslog::Priority& log_priority);
    std::string buffer_;
    int facility_;
    int priority_;
    char ident_[50];
};

inline
sbuf::sbuf(std::string ident, int facility)
{
    facility_ = facility;
    priority_ = LOG_DEBUG;
    strncpy(ident_, ident.c_str(), sizeof(ident_));
    ident_[sizeof(ident_)-1] = '\0';

    openlog(ident_, LOG_PID, facility_);
}

inline int
sbuf::sync()
{
    if (buffer_.length()) {
        syslog(priority_, "%s", buffer_.c_str());
        buffer_.erase();
        priority_ = LOG_DEBUG; // default to debug for each message
    }
    return 0;
}

inline int
sbuf::overflow(int c)
{
    if (c != EOF) {
        buffer_ += static_cast<char>(c);
    } else {
        sync();
    }
    return c;
}

inline std::ostream &
operator<< (std::ostream& os, const Syslog::Priority& log_priority)
{
    static_cast<sbuf *>(os.rdbuf())->priority_ = (int)log_priority;
    return os;
}

};

#endif // __cplusplus

#endif /* _UNI_C_LOG_H */

