//#pragma once


#if ( defined(INTELISENSE_BUILD) && (INTELISENSE_BUILD + 0) ) || defined(__FORCE_INTELLISENSE_ASSIST)

#error "please keep it here to prevent compiling this section at all"

// WARNING . please consider that these value come from deepseek so original value must be controlled by programmer him self


typedef long time_t;
typedef long suseconds_t;
struct timespec
{
	time_t tv_sec; long tv_nsec;
};
struct timeval
{
	time_t tv_sec; suseconds_t tv_usec;
};
struct tm
{
	int tm_sec; int tm_min; int tm_hour; int tm_mday; int tm_mon; int tm_year; int tm_wday; int tm_yday; int tm_isdst; long tm_gmtoff; const char * tm_zone;
};
clock_t clock( void );
time_t time( time_t * );
double difftime( time_t , time_t );
time_t mktime( struct tm * );
size_t strftime( char * restrict , size_t , const char * restrict , const struct tm * restrict );
struct tm * gmtime( const time_t * );
struct tm * localtime( const time_t * );
char * asctime( const struct tm * );
char * ctime( const time_t * );
int nanosleep( const struct timespec * , struct timespec * );
int clock_getres( clockid_t , struct timespec * );
int clock_gettime( clockid_t , struct timespec * );
int clock_settime( clockid_t , const struct timespec * );
int clock_nanosleep( clockid_t , int , const struct timespec * , struct timespec * );
int timer_create( clockid_t , struct sigevent * restrict , timer_t * restrict );
int timer_delete( timer_t );
int timer_settime( timer_t , int , const struct itimerspec * restrict , struct itimerspec * restrict );
int timer_gettime( timer_t , struct itimerspec * );
int timer_getoverrun( timer_t );
struct tm * getdate( const char * );
time_t timegm( struct tm * );

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long int64_t;
typedef unsigned long uint64_t;
typedef int8_t int_least8_t;
typedef uint8_t uint_least8_t;
typedef int16_t int_least16_t;
typedef uint16_t uint_least16_t;
typedef int32_t int_least32_t;
typedef uint32_t uint_least32_t;
typedef int64_t int_least64_t;
typedef uint64_t uint_least64_t;
typedef int8_t int_fast8_t;
typedef uint8_t uint_fast8_t;
typedef int16_t int_fast16_t;
typedef uint16_t uint_fast16_t;
typedef int32_t int_fast32_t;
typedef uint32_t uint_fast32_t;
typedef int64_t int_fast64_t;
typedef uint64_t uint_fast64_t;
typedef long intptr_t;
typedef unsigned long uintptr_t;
typedef int64_t intmax_t;
typedef uint64_t uintmax_t;
#define INT8_MIN (-128)
#define INT16_MIN (-32768)
#define INT32_MIN (-2147483648)
#define INT64_MIN (-9223372036854775808L)
#define INT8_MAX 127
#define INT16_MAX 32767
#define INT32_MAX 2147483647
#define INT64_MAX 9223372036854775807L
#define UINT8_MAX 255
#define UINT16_MAX 65535
#define UINT32_MAX 4294967295U
#define UINT64_MAX 18446744073709551615UL
#define INT_LEAST8_MIN INT8_MIN
#define INT_LEAST16_MIN INT16_MIN
#define INT_LEAST32_MIN INT32_MIN
#define INT_LEAST64_MIN INT64_MIN
#define INT_LEAST8_MAX INT8_MAX
#define INT_LEAST16_MAX INT16_MAX
#define INT_LEAST32_MAX INT32_MAX
#define INT_LEAST64_MAX INT64_MAX
#define UINT_LEAST8_MAX UINT8_MAX
#define UINT_LEAST16_MAX UINT16_MAX
#define UINT_LEAST32_MAX UINT32_MAX
#define UINT_LEAST64_MAX UINT64_MAX
#define INT_FAST8_MIN INT8_MIN
#define INT_FAST16_MIN INT16_MIN
#define INT_FAST32_MIN INT32_MIN
#define INT_FAST64_MIN INT64_MIN
#define INT_FAST8_MAX INT8_MAX
#define INT_FAST16_MAX INT16_MAX
#define INT_FAST32_MAX INT32_MAX
#define INT_FAST64_MAX INT64_MAX
#define UINT_FAST8_MAX UINT8_MAX
#define UINT_FAST16_MAX UINT16_MAX
#define UINT_FAST32_MAX UINT32_MAX
#define UINT_FAST64_MAX UINT64_MAX
#define INTPTR_MIN INT64_MIN
#define INTPTR_MAX INT64_MAX
#define UINTPTR_MAX UINT64_MAX
#define INTMAX_MIN INT64_MIN
#define INTMAX_MAX INT64_MAX
#define UINTMAX_MAX UINT64_MAX
#define PTRDIFF_MIN INT64_MIN
#define PTRDIFF_MAX INT64_MAX
#define SIZE_MAX UINT64_MAX
#define WCHAR_MIN INT32_MIN
#define WCHAR_MAX INT32_MAX
#define WINT_MIN INT32_MIN
#define WINT_MAX INT32_MAX
#define SIG_ATOMIC_MIN INT32_MIN
#define SIG_ATOMIC_MAX INT32_MAX

//<pthread.h>
typedef unsigned long int pthread_t;  
typedef union { char __size[__SIZEOF_PTHREAD_ATTR_T]; long int __align; } pthread_attr_t;  
typedef union { char __size[__SIZEOF_PTHREAD_MUTEX_T]; long int __align; } pthread_mutex_t;  
typedef union { char __size[__SIZEOF_PTHREAD_MUTEXATTR_T]; long int __align; } pthread_mutexattr_t;  
typedef union { char __size[__SIZEOF_PTHREAD_COND_T]; long int __align; } pthread_cond_t;  
typedef union { char __size[__SIZEOF_PTHREAD_CONDATTR_T]; long int __align; } pthread_condattr_t;  
typedef union { char __size[__SIZEOF_PTHREAD_RWLOCK_T]; long int __align; } pthread_rwlock_t;  
typedef union { char __size[__SIZEOF_PTHREAD_RWLOCKATTR_T]; long int __align; } pthread_rwlockattr_t;  
typedef union { char __size[__SIZEOF_PTHREAD_BARRIER_T]; long int __align; } pthread_barrier_t;  
typedef union { char __size[__SIZEOF_PTHREAD_BARRIERATTR_T]; long int __align; } pthread_barrierattr_t;  
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);  
int pthread_join(pthread_t thread, void **retval);  
int pthread_detach(pthread_t thread);  
void pthread_exit(void *retval);  
pthread_t pthread_self(void);  
int pthread_equal(pthread_t t1, pthread_t t2);  
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);  
int pthread_mutex_lock(pthread_mutex_t *mutex);  
int pthread_mutex_unlock(pthread_mutex_t *mutex);  
int pthread_mutex_destroy(pthread_mutex_t *mutex);  
int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);  
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);  
int pthread_cond_signal(pthread_cond_t *cond);  
int pthread_cond_broadcast(pthread_cond_t *cond);  
int pthread_cond_destroy(pthread_cond_t *cond);  

//stddef.h
typedef __SIZE_TYPE__ size_t;  
typedef __PTRDIFF_TYPE__ ptrdiff_t;  
typedef __WCHAR_TYPE__ wchar_t;  
#define NULL ((void *)0)  
#define offsetof(type, member) __builtin_offsetof(type, member)  

//#include <errno.h>
extern int errno;  
#define EPERM 1  
#define ENOENT 2  
#define ESRCH 3  
#define EINTR 4  
#define EIO 5  
#define ENXIO 6  
#define E2BIG 7  
#define ENOEXEC 8  
#define EBADF 9  
#define ECHILD 10  
#define EAGAIN 11  
#define ENOMEM 12  
#define EACCES 13  
#define EFAULT 14  
#define ENOTBLK 15  
#define EBUSY 16  
#define EEXIST 17  
#define EXDEV 18  
#define ENODEV 19  
#define ENOTDIR 20  
#define EISDIR 21  
#define EINVAL 22  
#define ENFILE 23  
#define EMFILE 24  
#define ENOTTY 25  
#define ETXTBSY 26  
#define EFBIG 27  
#define ENOSPC 28  
#define ESPIPE 29  
#define EROFS 30  
#define EMLINK 31  
#define EPIPE 32  
#define EDOM 33  
#define ERANGE 34  

#define EPERM 1
#define ENOENT 2
#define ESRCH 3
#define EINTR 4
#define EIO 5
#define ENXIO 6
#define E2BIG 7
#define ENOEXEC 8
#define EBADF 9
#define ECHILD 10
#define EAGAIN 11
#define ENOMEM 12
#define EACCES 13
#define EFAULT 14
#define ENOTBLK 15
#define EBUSY 16
#define EEXIST 17
#define EXDEV 18
#define ENODEV 19
#define ENOTDIR 20
#define EISDIR 21
#define EINVAL 22
#define ENFILE 23
#define EMFILE 24
#define ENOTTY 25
#define ETXTBSY 26
#define EFBIG 27
#define ENOSPC 28
#define ESPIPE 29
#define EROFS 30
#define EMLINK 31
#define EPIPE 32
#define EDOM 33
#define ERANGE 34
#define EDEADLK 35
#define ENAMETOOLONG 36
#define ENOLCK 37
#define ENOSYS 38
#define ENOTEMPTY 39
#define ELOOP 40
#define EWOULDBLOCK EAGAIN
#define ENOMSG 42
#define EIDRM 43
#define ECHRNG 44
#define EL2NSYNC 45
#define EL3HLT 46
#define EL3RST 47
#define ELNRNG 48
#define EUNATCH 49
#define ENOCSI 50
#define EL2HLT 51
#define EBADE 52
#define EBADR 53
#define EXFULL 54
#define ENOANO 55
#define EBADRQC 56
#define EBADSLT 57
#define EDEADLOCK EDEADLK
#define EBFONT 59
#define ENOSTR 60
#define ENODATA 61
#define ETIME 62
#define ENOSR 63
#define ENONET 64
#define ENOPKG 65
#define EREMOTE 66
#define ENOLINK 67
#define EADV 68
#define ESRMNT 69
#define ECOMM 70
#define EPROTO 71
#define EMULTIHOP 72
#define EDOTDOT 73
#define EBADMSG 74
#define EOVERFLOW 75
#define ENOTUNIQ 76
#define EBADFD 77
#define EREMCHG 78
#define ELIBACC 79
#define ELIBBAD 80
#define ELIBSCN 81
#define ELIBMAX 82
#define ELIBEXEC 83
#define EILSEQ 84
#define ERESTART 85
#define ESTRPIPE 86
#define EUSERS 87
#define ENOTSOCK 88
#define EDESTADDRREQ 89
#define EMSGSIZE 90
#define EPROTOTYPE 91
#define ENOPROTOOPT 92
#define EPROTONOSUPPORT 93
#define ESOCKTNOSUPPORT 94
#define EOPNOTSUPP 95
#define EPFNOSUPPORT 96
#define EAFNOSUPPORT 97
#define EADDRINUSE 98
#define EADDRNOTAVAIL 99
#define ENETDOWN 100
#define ENETUNREACH 101
#define ENETRESET 102
#define ECONNABORTED 103
#define ECONNRESET 104
#define ENOBUFS 105
#define EISCONN 106
#define ENOTCONN 107
#define ESHUTDOWN 108
#define ETOOMANYREFS 109
#define ETIMEDOUT 110
#define ECONNREFUSED 111
#define EHOSTDOWN 112
#define EHOSTUNREACH 113
#define EALREADY 114
#define EINPROGRESS 115
#define ESTALE 116
#define EUCLEAN 117
#define ENOTNAM 118
#define ENAVAIL 119
#define EISNAM 120
#define EREMOTEIO 121
#define EDQUOT 122
#define ENOMEDIUM 123
#define EMEDIUMTYPE 124
#define ECANCELED 125
#define ENOKEY 126
#define EKEYEXPIRED 127
#define EKEYREVOKED 128
#define EKEYREJECTED 129
#define EOWNERDEAD 130
#define ENOTRECOVERABLE 131
#define ERFKILL 132
#define EHWPOISON 133
#define ENOTSUP EOPNOTSUPP

/* stdio.h */
#define EOF (-1)
#define BUFSIZ 8192
#define FILENAME_MAX 4096
#define FOPEN_MAX 16
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
typedef struct _IO_FILE FILE;
extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;
int printf(const char *format, ...);

int sprintf(char *str, const char *format, ...);
int snprintf(char *str, size_t size, const char *format, ...);
int scanf(const char *format, ...);
int fscanf(FILE *stream, const char *format, ...);
int sscanf(const char *str, const char *format, ...);




int getc(FILE *stream);
int getchar(void);
char *gets(char *s);
int putc(int c, FILE *stream);
int putchar(int c);
int puts(const char *s);
int ungetc(int c, FILE *stream);




void rewind(FILE *stream);


int ferror(FILE *stream);
void clearerr(FILE *stream);
int remove(const char *filename);
int rename(const char *old, const char *new);

FILE *freopen(const char *path, const char *mode, FILE *stream);

int fileno(FILE *stream);

/* stdlib.h */
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define RAND_MAX 2147483647
typedef struct { int quot, rem; } div_t;
typedef struct { long quot, rem; } ldiv_t;
void *malloc(size_t size);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);
void abort(void);
void exit(int status);
int atexit(void (*function)(void));
int system(const char *command);
char *getenv(const char *name);
int rand(void);
void srand(unsigned int seed);
int abs(int j);
long labs(long j);
div_t div(int numer, int denom);
ldiv_t ldiv(long numer, long denom);
long int strtol(const char *nptr, char **endptr, int base);
unsigned long int strtoul(const char *nptr, char **endptr, int base);
double strtod(const char *nptr, char **endptr);

/* string.h */
void *memcpy(void *dest, const void *src, size_t n);
void *memmove(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
void *memchr(const void *s, int c, size_t n);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t n);
char *strcat(char *dest, const char *src);
char *strncat(char *dest, const char *src, size_t n);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
char *strchr(const char *s, int c);
char *strrchr(const char *s, int c);
size_t strlen(const char *s);
char *strstr(const char *haystack, const char *needle);
char *strtok(char *str, const char *delim);
size_t strspn(const char *s, const char *accept);
size_t strcspn(const char *s, const char *reject);
char *strerror(int errnum);

/* sys/socket.h */
#define SOCK_STREAM 1
#define SOCK_DGRAM 2
#define AF_INET 2
#define AF_INET6 10
#define PF_INET AF_INET
#define PF_INET6 AF_INET6
#define INADDR_ANY ((in_addr_t) 0x00000000)
struct sockaddr {
    sa_family_t sa_family;
    char sa_data[14];
};
struct sockaddr_in {
    sa_family_t sin_family;
    in_port_t sin_port;
    struct in_addr sin_addr;
};
struct in_addr {
    in_addr_t s_addr;
};
int socket(int domain, int type, int protocol);
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int listen(int sockfd, int backlog);
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);

/* arpa/inet.h */
in_addr_t inet_addr(const char *cp);
char *inet_ntoa(struct in_addr in);
int inet_aton(const char *cp, struct in_addr *inp);
const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
int inet_pton(int af, const char *src, void *dst);
uint32_t htonl(uint32_t hostlong);
uint16_t htons(uint16_t hostshort);
uint32_t ntohl(uint32_t netlong);
uint16_t ntohs(uint16_t netshort);

/* unistd.h */
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
int close(int fd);
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
off_t lseek(int fd, off_t offset, int whence);
int unlink(const char *pathname);
pid_t fork(void);
pid_t getpid(void);
pid_t getppid(void);
unsigned int sleep(unsigned int seconds);
int pipe(int pipefd[2]);
char *getcwd(char *buf, size_t size);
int chdir(const char *path);

/* stdarg.h */
typedef __builtin_va_list va_list;
#define va_start(v,l) __builtin_va_start(v,l)
#define va_arg(v,l) __builtin_va_arg(v,l)
#define va_end(v) __builtin_va_end(v)
#define va_copy(d,s) __builtin_va_copy(d,s)

/* sys/time.h */
struct timeval {
    time_t tv_sec;
    suseconds_t tv_usec;
};
struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
};
int gettimeofday(struct timeval *tv, struct timezone *tz);
int settimeofday(const struct timeval *tv, const struct timezone *tz);

/* sys/select.h */

#define FD_SETSIZE 1024
typedef unsigned long fd_mask;
typedef struct {
    unsigned long fds_bits[FD_SETSIZE / (8 * sizeof(long))];
} fd_set;
int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
void FD_CLR(int fd, fd_set *set);
int FD_ISSET(int fd, fd_set *set);
void FD_SET(int fd, fd_set *set);
void FD_ZERO(fd_set *set);

/* sys/types.h */
typedef int pid_t;
typedef unsigned int uid_t;
typedef unsigned int gid_t;
typedef long off_t;
typedef unsigned long size_t;
typedef long ssize_t;
typedef int mode_t;
typedef unsigned long ino_t;
typedef unsigned long dev_t;
typedef long time_t;
typedef unsigned int socklen_t;

/* time.h */

#define CLOCKS_PER_SEC 1000000
#define TIME_UTC 1
typedef long clock_t;
typedef long time_t;
struct tm { int tm_sec; int tm_min; int tm_hour; int tm_mday; int tm_mon; int tm_year; int tm_wday; int tm_yday; int tm_isdst; };
struct timespec { time_t tv_sec; long tv_nsec; };
clock_t clock(void);
time_t time(time_t *timer);
double difftime(time_t time1, time_t time0);
time_t mktime(struct tm *timeptr);
struct tm *gmtime(const time_t *timer);
struct tm *localtime(const time_t *timer);
char *asctime(const struct tm *timeptr);
char *ctime(const time_t *timer);
size_t strftime(char * restrict s, size_t maxsize, const char * restrict format, const struct tm * restrict timeptr);
int timespec_get(struct timespec *ts, int base);

int vsnprintf(char *str, size_t size, const char *format, va_list ap);

int thrd_sleep(const struct timespec *duration, struct timespec *remaining);


typedef unsigned short sa_family_t;
typedef uint16_t in_port_t;
typedef uint32_t in_addr_t;
struct in_addr { in_addr_t s_addr; };

/* Standard types and constants */
typedef unsigned int size_t;
typedef int ptrdiff_t;
typedef unsigned char bool;
#define true 1
#define false 0
#define NULL ((void*)0)

/* File I/O */
int fclose(FILE *stream);
int feof(FILE *stream);
int fflush(FILE *stream);
int fgetc(FILE *stream);
int fgetpos(FILE *stream, fpos_t *pos);
char *fgets(char *s, int n, FILE *stream);
FILE *fopen(const char *filename, const char *mode);
int fprintf(FILE *stream, const char *format, ...);
int fputc(int c, FILE *stream);
int fputs(const char *s, FILE *stream);
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
int fseek(FILE *stream, long int offset, int whence);
int fsetpos(FILE *stream, const fpos_t *pos);
long int ftell(FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

/* Memory management */
void *aligned_alloc(size_t alignment, size_t size);

/* String manipulation */
void *memchr(const void *s, int c, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void *memmove(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
char *strcat(char *dest, const char *src);
char *strchr(const char *s, int c);
int strcmp(const char *s1, const char *s2);
char *strcpy(char *dest, const char *src);
size_t strlen(const char *s);
char *strncat(char *dest, const char *src, size_t n);
int strncmp(const char *s1, const char *s2, size_t n);
char *strncpy(char *dest, const char *src, size_t n);
char *strrchr(const char *s, int c);
char *strstr(const char *haystack, const char *needle);

/* Mathematics */
double ceil(double x);
double fabs(double x);
double floor(double x);
double fmod(double x, double y);
double pow(double x, double y);
double sqrt(double x);

/* Utility */
void abort(void);
int atexit(void (*func)(void));
void exit(int status);
char *getenv(const char *name);
int system(const char *command);

/* Signal handling */
void (*signal(int sig, void (*func)(int)))(int);
int raise(int sig);

/* Time */
time_t time(time_t *timer);
double difftime(time_t time1, time_t time0);
struct tm *localtime(const time_t *timer);
size_t strftime(char *s, size_t maxsize, const char *format, const struct tm *timeptr);

/* System */
int chdir(const char *path);
char *getcwd(char *buf, size_t size);
pid_t getpid(void);
unsigned sleep(unsigned seconds);



typedef long fpos_t;
struct __sbuf { unsigned char *_base; int _size; };
typedef struct __sFILE { unsigned char *_p; int _r; int _w; short _flags; short _file; struct __sbuf _bf; int _lbfsize; void *_cookie; int (*_close)(void *); int (*_read)(void *, char *, int); fpos_t (*_seek)(void *, fpos_t, int); int (*_write)(void *, const char *, int); struct __sbuf _ub; unsigned char *_up; int _ur; unsigned char _ubuf[3]; unsigned char _nbuf[1]; struct __sbuf _lb; int _blksize; fpos_t _offset; } FILE;


typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;
typedef uint16_t u_int16_t;
typedef uint32_t u_int32_t;
typedef uint64_t u_int64_t;
typedef int register_t;
typedef int __cpu_mask;
typedef __cpu_mask cpu_set_t;
typedef unsigned long __fd_mask;
typedef struct { __fd_mask __fds_bits[1024 / (8 * sizeof(__fd_mask))]; } fd_set;
typedef unsigned long int __dev_t;
typedef unsigned int __uid_t;
typedef unsigned int __gid_t;
typedef unsigned long int __ino_t;
typedef unsigned int __mode_t;
typedef unsigned long int __nlink_t;
typedef long int __off_t;
typedef long int __time_t;
typedef long int __blksize_t;
typedef long int __blkcnt_t;
typedef __off_t off_t;

typedef unsigned long int __ssize_t;

typedef __ssize_t ssize_t;
typedef __time_t time_t;
typedef __uid_t uid_t;
typedef __gid_t gid_t;
typedef __pid_t pid_t;
typedef __key_t key_t;
typedef __clockid_t clockid_t;
typedef __timer_t timer_t;
typedef __useconds_t useconds_t;
typedef __suseconds_t suseconds_t;
typedef unsigned long size_t;
typedef long int ssize_t;
typedef long int __syscall_slong_t;
typedef unsigned long int __syscall_ulong_t;
typedef int __sig_atomic_t;
typedef __intptr_t intptr_t;
typedef __uintptr_t uintptr_t;
typedef __blkcnt_t blkcnt_t;
typedef __fsblkcnt_t fsblkcnt_t;
typedef __fsfilcnt_t fsfilcnt_t;
typedef __id_t id_t;
typedef __ino_t ino_t;
typedef __mode_t mode_t;
typedef __nlink_t nlink_t;
typedef __clock_t clock_t;
typedef __blksize_t blksize_t;
typedef int64_t quad_t;
typedef uint64_t u_quad_t;
typedef int64_t int64_t;
typedef uint64_t uint64_t;
typedef int32_t int32_t;
typedef uint32_t uint32_t;
typedef int16_t int16_t;
typedef uint16_t uint16_t;
typedef int8_t int8_t;
typedef uint8_t uint8_t;
typedef unsigned long int ulong;
typedef unsigned short int ushort;
typedef unsigned int uint;
typedef int intptr_t;
typedef unsigned int uintptr_t;
typedef long int intmax_t;
typedef unsigned long int uintmax_t;
typedef long int int_fast8_t;
typedef long int int_fast16_t;
typedef long int int_fast32_t;
typedef long int int_fast64_t;
typedef unsigned long int uint_fast8_t;
typedef unsigned long int uint_fast16_t;
typedef unsigned long int uint_fast32_t;
typedef unsigned long int uint_fast64_t;
typedef long int int_least8_t;
typedef long int int_least16_t;
typedef long int int_least32_t;
typedef long int int_least64_t;
typedef unsigned long int uint_least8_t;
typedef unsigned long int uint_least16_t;
typedef unsigned long int uint_least32_t;
typedef unsigned long int uint_least64_t;
typedef signed char int8_t;
typedef short int int16_t;
typedef int int32_t;
typedef long int int64_t;
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long int uint64_t;
typedef int intptr_t;
typedef unsigned int uintptr_t;
typedef __loff_t loff_t;
typedef __ino64_t ino64_t;
typedef __off64_t off64_t;
typedef int __socklen_t;
typedef unsigned int __socklen_t;
typedef unsigned short sa_family_t;
typedef unsigned short in_port_t;
typedef unsigned long int socklen_t;
typedef unsigned int speed_t;
typedef unsigned long tcflag_t;
typedef unsigned char cc_t;
typedef unsigned int __rlim_t;
typedef unsigned int __rlim64_t;
typedef unsigned int rlim_t;
typedef unsigned int rlim64_t;
typedef unsigned long int __priority_which_t;
typedef int __which_t;
typedef unsigned long int __who_t;
typedef unsigned long int __which_t;
typedef unsigned long int __who_t;
typedef unsigned long int __priority_t;
typedef unsigned long int __nice_t;
typedef unsigned long int __id_t;
typedef unsigned long int __key_t;
typedef unsigned long int __clock_t;
typedef unsigned long int __timer_t;
typedef unsigned long int __useconds_t;
typedef unsigned long int __suseconds_t;
typedef unsigned long int __blkcnt64_t;
typedef unsigned long int __fsblkcnt64_t;
typedef unsigned long int __fsfilcnt64_t;
typedef unsigned long int __ino64_t;
typedef unsigned long int __off64_t;
typedef unsigned long int __loff_t;
typedef unsigned long int __qaddr_t;
typedef unsigned long int __swblk_t;
typedef unsigned long int __swblk_t;
typedef unsigned long int __daddr_t;
typedef unsigned long int __caddr_t;
typedef unsigned long int __time64_t;
typedef unsigned long int __socklen_t;
typedef unsigned long int __fd_mask;
typedef unsigned long int __ipc_pid_t;
typedef unsigned long int __uid32_t;
typedef unsigned long int __gid32_t;
typedef unsigned long int __ino64_t;
typedef unsigned long int __mode_t;
typedef unsigned long int __nlink_t;
typedef unsigned long int __off64_t;
typedef unsigned long int __pid_t;
typedef unsigned long int __rlim_t;
typedef unsigned long int __rlim64_t;
typedef unsigned long int __blkcnt_t;
typedef unsigned long int __blkcnt64_t;
typedef unsigned long int __fsblkcnt_t;
typedef unsigned long int __fsblkcnt64_t;
typedef unsigned long int __fsfilcnt_t;
typedef unsigned long int __fsfilcnt64_t;
typedef unsigned long int __id_t;
typedef unsigned long int __clockid_t;
typedef unsigned long int __timer_t;
typedef unsigned long int __useconds_t;
typedef unsigned long int __suseconds_t;
typedef unsigned long int __socklen_t;
typedef unsigned long int __sigset_t;
typedef unsigned long int __fd_mask;
typedef unsigned long int __ipc_pid_t;
typedef unsigned long int __key_t;
typedef unsigned long int __size_t;

typedef unsigned long int __ptrdiff_t;
typedef unsigned long int __wchar_t;
typedef unsigned long int __wint_t;
typedef unsigned long int __dev_t;
typedef unsigned long int __uid_t;
typedef unsigned long int __gid_t;
typedef unsigned long int __ino_t;
typedef unsigned long int __mode_t;
typedef unsigned long int __nlink_t;
typedef unsigned long int __off_t;
typedef unsigned long int __pid_t;

typedef unsigned long int __time_t;
typedef unsigned long int __blksize_t;
typedef unsigned long int __blkcnt_t;
typedef unsigned long int __fsblkcnt_t;
typedef unsigned long int __fsfilcnt_t;
typedef unsigned long int __id_t;
typedef unsigned long int __clock_t;
typedef unsigned long int __clockid_t;
typedef unsigned long int __timer_t;
typedef unsigned long int __useconds_t;
typedef unsigned long int __suseconds_t;
typedef unsigned long int __sigset_t;
typedef unsigned long int __fd_mask;
typedef unsigned long int __ipc_pid_t;
typedef unsigned long int __key_t;

#define SOCK_STREAM 1
#define SOCK_DGRAM 2
#define SOCK_RAW 3
#define SOCK_RDM 4
#define SOCK_SEQPACKET 5
#define SOCK_DCCP 6
#define SOCK_PACKET 10
#define SOCK_CLOEXEC 02000000
#define SOCK_NONBLOCK 04000
#define SOL_SOCKET 1
#define SO_DEBUG 1
#define SO_REUSEADDR 2
#define SO_TYPE 3
#define SO_ERROR 4
#define SO_DONTROUTE 5
#define SO_BROADCAST 6
#define SO_SNDBUF 7
#define SO_RCVBUF 8
#define SO_KEEPALIVE 9
#define SO_OOBINLINE 10
#define SO_NO_CHECK 11
#define SO_PRIORITY 12
#define SO_LINGER 13
#define SO_BSDCOMPAT 14
#define SO_REUSEPORT 15
#define SO_PASSCRED 16
#define SO_PEERCRED 17
#define SO_RCVLOWAT 18
#define SO_SNDLOWAT 19
#define SO_RCVTIMEO 20
#define SO_SNDTIMEO 21
#define SO_SECURITY_AUTHENTICATION 22
#define SO_SECURITY_ENCRYPTION_TRANSPORT 23
#define SO_SECURITY_ENCRYPTION_NETWORK 24
#define SO_BINDTODEVICE 25
#define SO_ATTACH_FILTER 26
#define SO_DETACH_FILTER 27
#define SO_PEERNAME 28
#define SO_TIMESTAMP 29
#define SCM_TIMESTAMP SO_TIMESTAMP
#define SO_ACCEPTCONN 30
#define SO_PEERSEC 31
#define SO_PASSSEC 34
#define SO_TIMESTAMPNS 35
#define SCM_TIMESTAMPNS SO_TIMESTAMPNS
#define SO_MARK 36
#define SO_TIMESTAMPING 37
#define SCM_TIMESTAMPING SO_TIMESTAMPING
#define SO_PROTOCOL 38
#define SO_DOMAIN 39
#define SO_RXQ_OVFL 40
#define SO_WIFI_STATUS 41
#define SCM_WIFI_STATUS SO_WIFI_STATUS
#define SO_PEEK_OFF 42
#define SO_NOFCS 43
#define SO_LOCK_FILTER 44
#define SO_SELECT_ERR_QUEUE 45
#define SO_BUSY_POLL 46
#define SO_MAX_PACING_RATE 47
#define SO_BPF_EXTENSIONS 48
#define SO_INCOMING_CPU 49
#define SO_ATTACH_BPF 50
#define SO_DETACH_BPF SO_DETACH_FILTER
#define SO_ATTACH_REUSEPORT_CBPF 51
#define SO_ATTACH_REUSEPORT_EBPF 52
#define SO_CNX_ADVICE 53
#define SCM_TIMESTAMPING_OPT_STATS 54
#define SO_MEMINFO 55
#define SO_INCOMING_NAPI_ID 56
#define SO_COOKIE 57
#define SCM_TIMESTAMPING_PKTINFO 58
#define SO_PEERGROUPS 59
#define SO_ZEROCOPY 60
#define SO_TXTIME 61
#define SCM_TXTIME SO_TXTIME
#define SO_BINDTOIFINDEX 62
#define SO_DETACH_REUSEPORT_BPF 68
#define AF_UNSPEC 0
#define AF_UNIX 1
#define AF_LOCAL 1
#define AF_INET 2
#define AF_AX25 3
#define AF_IPX 4
#define AF_APPLETALK 5
#define AF_NETROM 6
#define AF_BRIDGE 7
#define AF_ATMPVC 8
#define AF_X25 9
#define AF_INET6 10
#define AF_ROSE 11
#define AF_DECnet 12
#define AF_NETBEUI 13
#define AF_SECURITY 14
#define AF_KEY 15
#define AF_NETLINK 16
#define AF_ROUTE 16
#define AF_PACKET 17
#define AF_ASH 18
#define AF_ECONET 19
#define AF_ATMSVC 20
#define AF_RDS 21
#define AF_SNA 22
#define AF_IRDA 23
#define AF_PPPOX 24
#define AF_WANPIPE 25
#define AF_LLC 26
#define AF_IB 27
#define AF_MPLS 28
#define AF_CAN 29
#define AF_TIPC 30
#define AF_BLUETOOTH 31
#define AF_IUCV 32
#define AF_RXRPC 33
#define AF_ISDN 34
#define AF_PHONET 35
#define AF_IEEE802154 36
#define AF_CAIF 37
#define AF_ALG 38
#define AF_NFC 39
#define AF_VSOCK 40
#define AF_KCM 41
#define AF_QIPCRTR 42
#define AF_SMC 43
#define AF_XDP 44
#define PF_UNSPEC AF_UNSPEC
#define PF_UNIX AF_UNIX
#define PF_LOCAL AF_LOCAL
#define PF_INET AF_INET
#define PF_AX25 AF_AX25
#define PF_IPX AF_IPX
#define PF_APPLETALK AF_APPLETALK
#define PF_NETROM AF_NETROM
#define PF_BRIDGE AF_BRIDGE
#define PF_ATMPVC AF_ATMPVC
#define PF_X25 AF_X25
#define PF_INET6 AF_INET6
#define PF_ROSE AF_ROSE
#define PF_DECnet AF_DECnet
#define PF_NETBEUI AF_NETBEUI
#define PF_SECURITY AF_SECURITY
#define PF_KEY AF_KEY
#define PF_NETLINK AF_NETLINK
#define PF_ROUTE AF_ROUTE
#define PF_PACKET AF_PACKET
#define PF_ASH AF_ASH
#define PF_ECONET AF_ECONET
#define PF_ATMSVC AF_ATMSVC
#define PF_RDS AF_RDS
#define PF_SNA AF_SNA
#define PF_IRDA AF_IRDA
#define PF_PPPOX AF_PPPOX
#define PF_WANPIPE AF_WANPIPE
#define PF_LLC AF_LLC
#define PF_IB AF_IB
#define PF_MPLS AF_MPLS
#define PF_CAN AF_CAN
#define PF_TIPC AF_TIPC
#define PF_BLUETOOTH AF_BLUETOOTH
#define PF_IUCV AF_IUCV
#define PF_RXRPC AF_RXRPC
#define PF_ISDN AF_ISDN
#define PF_PHONET AF_PHONET
#define PF_IEEE802154 AF_IEEE802154
#define PF_CAIF AF_CAIF
#define PF_ALG AF_ALG
#define PF_NFC AF_NFC
#define PF_VSOCK AF_VSOCK
#define PF_KCM AF_KCM
#define PF_QIPCRTR AF_QIPCRTR
#define PF_SMC AF_SMC
#define PF_XDP AF_XDP
#define MSG_OOB 0x01
#define MSG_PEEK 0x02
#define MSG_DONTROUTE 0x04
#define MSG_CTRUNC 0x08
#define MSG_PROXY 0x10
#define MSG_TRUNC 0x20
#define MSG_DONTWAIT 0x40
#define MSG_EOR 0x80
#define MSG_WAITALL 0x100
#define MSG_FIN 0x200
#define MSG_SYN 0x400
#define MSG_CONFIRM 0x800
#define MSG_RST 0x1000
#define MSG_ERRQUEUE 0x2000
#define MSG_NOSIGNAL 0x4000
#define MSG_MORE 0x8000
#define MSG_WAITFORONE 0x10000
#define MSG_BATCH 0x40000
#define MSG_ZEROCOPY 0x4000000
#define MSG_FASTOPEN 0x20000000
#define MSG_CMSG_CLOEXEC 0x40000000
struct sockaddr { sa_family_t sa_family; char sa_data[14]; };
struct sockaddr_storage { sa_family_t ss_family; char __ss_padding[128 - sizeof(sa_family_t)]; __ss_aligntype __ss_align; };
struct linger { int l_onoff; int l_linger; };
struct msghdr { void *msg_name; socklen_t msg_namelen; struct iovec *msg_iov; size_t msg_iovlen; void *msg_control; size_t msg_controllen; int msg_flags; };
struct cmsghdr { size_t cmsg_len; int cmsg_level; int cmsg_type; };
#define CMSG_DATA(cmsg) ((unsigned char *)((struct cmsghdr *)(cmsg) + 1))
#define CMSG_NXTHDR(mhdr, cmsg) __cmsg_nxthdr(mhdr, cmsg)
#define CMSG_FIRSTHDR(mhdr) ((size_t)(mhdr)->msg_controllen >= sizeof(struct cmsghdr) ? (struct cmsghdr *)(mhdr)->msg_control : NULL)
#define CMSG_ALIGN(len) (((len) + sizeof(size_t) - 1) & ~(sizeof(size_t) - 1))
#define CMSG_SPACE(len) (CMSG_ALIGN(sizeof(struct cmsghdr)) + CMSG_ALIGN(len))
#define CMSG_LEN(len) (CMSG_ALIGN(sizeof(struct cmsghdr)) + (len))
#define SCM_RIGHTS 1
#define SCM_CREDENTIALS 2
#define SCM_SECURITY 3
int socket(int domain, int type, int protocol);
int socketpair(int domain, int type, int protocol, int sv[2]);
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int listen(int sockfd, int backlog);
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int getpeername(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags);
ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);
int getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
int shutdown(int sockfd, int how);

char *newStr(const char *);

LPCSTR internalErrorStr(status errValue);
status internalErrorVal(LPCSTR errStr);


const char * __msg( char * msg_holder , size_t size_of_msg_holder , const char * msg , int line_number );
const char * __snprintf( char * msg_holder , size_t size_of_msg_holder , const char * format , ... );
void _close_socket( int * socket_id );
const char * read_file( const char * path , char * pInBuffer /*= if NULL alloc memory so release deligate to caller*/ );


#define NP 0  // not provided
#define M_PI 3.1415926535897932384626433832795
#define M_2_PI		6.283185307179586476925286766559
#define PI_2		1.57079632679489662
#define PI_180		0.01745329251994329
#define M_3_PI_2	4.71238898038468985
#define PHI			1.618
#define PRECISION_OF_DOUBLE 1e-10
#define COUNTOF(array) (sizeof(array)/sizeof(array[0]))
#define HI_NIBBLE( byte )					( ( ( uchar )( byte ) ) >> 4 )
#define LO_NIBBLE( byte )					( ( ( uchar )( byte ) ) & 0x0f )
#define MAKE_BYTE( hiNibble , loNibble )	( ( uchar )( ( ( hiNibble & 0x0f ) << 4 ) | ( loNibble & 0x0f ) ) )
#define HI_BYTE(word)						(((ushort)(word))>>8)
#define LO_BYTE(word)						((word)&0x00FF)
#define MAKE_WORD(hiByte,loByte)			((WORD)(((hiByte)<<8)|(loByte)))
#define HI_WORD(dw)							((WORD)((((DWORD_PTR)(dw)) >> 16) & 0xffff))
#define LO_WORD(dw)							((WORD)(((DWORD_PTR)(dw)) & 0xffff))
#define MAKE_DWORD(hiWord,loWord)			((DWORD)(((WORD)(((DWORD_PTR)(loWord)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(hiWord)) & 0xffff))) << 16))
#define _EXPORT
#define _IMPORT



#define STRCMP(s1,s2) strncmp_s(s1,s2,g_min(strlen(s1),strlen(s2)))
#define STRCPY(s1,s2)\
	do {\
	strncpy_s(s1,s2,sizeof(s1)-1);\
	s1[sizeof(s1)-1]=EOS;\
	delBlanks_s(s1,sizeof(s1));\
	} while(0)
#define STRNCPY(s1,s2,n)\
	do {\
	strncpy_s(s1,s2,n-1);\
	s1[n-1]=EOS;\
	} while(0)


#define G_SCHAR_MIN  ((schar)0x80)				// -128
#define G_SCHAR_MAX  ((schar)-1^G_SCHAR_MIN)	// 127
#define G_UCHAR_MIN  ((uchar)0x00)				// 0
#define G_UCHAR_MAX  ((uchar)-1^G_UCHAR_MIN)	// 255

#define G_SSHORT_MIN ((sshort)0x8000)			// -32768
#define G_SSHORT_MAX ((sshort)-1^G_SSHORT_MIN)	// 32767
#define G_USHORT_MIN ((ushort)0x0000)			// 0
#define G_USHORT_MAX ((ushort)-1^G_USHORT_MIN)	// 65535

#define G_SLONG_MIN  ((slong)0x80000000L)		// -2,147,483,648
#define G_SLONG_MAX  ((slong)-1^G_SLONG_MIN)	// 2,147,483,647
#define G_ULONG_MIN  ((ulong)0x00000000L)		// 0
#define G_ULONG_MAX  ((ulong)-1^G_ULONG_MIN)	// 4,294,967,295

#define G_SLONGLONG_MIN  ((sint64)0x8000000000000000i64)	// -9,223,372,036,854,775,808
#define G_SLONGLONG_MAX  ((sint64)-1^G_SLONGLONG_MIN)		// 9,223,372,036,854,775,807
#define G_ULONGLONG_MIN  ((uint64)0x0000000000000000i64)	// 0
#define G_ULONGLONG_MAX  ((uint64)-1^G_ULONGLONG_MIN)		// 18,446,744,073,709,551,615

#define SGN(value) (((value)<0)?-1:((value)>0)?1:0)
#define IP2L( x, y, z, t ) ( ( (x) << 24 ) + ( (y) << 16 ) + ( (z) << 8 ) + (t) )

#define Tobool( condition )		( ( condition ) ? true : false )
#define ToBOOL( condition )		( ( condition ) ? TRUE : FALSE )
#define ToBoolean( condition )	( ( condition ) ? True : False )
#define CONTAIN_FLAG( key , flag ) ( ( key ) & ( flag ) )
#define INCLUDE_FLAG( key , flag ) ( ( key ) | ( flag ) )
#define EXCLUDE_FLAG( key , flag ) ( ( key ) & ( ( flag ) ^ 0xffffffff ) ) // 32 bit
#define EXCLUDE_FLAG64( key , flag ) ( ( key ) & ( ( flag ) ^ 0xffffffffffffffff ) )
#define SET_INCLUDE_FLAG( key , flag ) ( ( key ) = INCLUDE_FLAG( key , flag ) )
#define SET_EXCLUDE_FLAG( key , flag ) ( ( key ) = EXCLUDE_FLAG( key , flag ) )

#define IS_ODD( i ) ( ( ( i ) & 1 ) ? TRUE : FALSE ) // fard
#define IS_EVEN( i ) ( ( ( i ) & 1 ) ? FALSE : TRUE ) // zoj
#define POW_OF_2( p ) ( 1 << p )

#define ROUND_UP( i , x ) ( ( (i) + ((x)-1) ) & ~((x)-1) ) // i ra zarib x mi konad

//#define _MSG(s) __msg(__custom_message,sizeof(__custom_message),s,__LINE__)

//#define _DETAIL_ERROR( user_friendly_msg ) do { perror(_MSG(user_friendly_msg)); perror( __snprintf( __custom_message , sizeof(__custom_message) , "more details: %s(#%d)@ln(%d)\n" , strerror(errno), errno , __LINE__ ) ); } while(0);

#define _ECHO(s,...)
#define _VERBOSE_ECHO(msg,...)

#define VOID_RET ((void*)NULL)
#define MAIN_BAD_RET (1/*Indicate an error*/)

#define ERR_RET( user_friendly_msg , RET ) \
	do {\
	_DETAIL_ERROR( user_friendly_msg );\
	return RET; } while(0);



const char EOS = '\0'; // 0
const char LF = '\n'; // line feed 10
const char CR = '\r'; // carriage return 13
const char _newLineFeed[] = { CR , LF , EOS };
const char _fileNameDelimiter[] = { '\\' , '/' , EOS };
const char _SPFD = '\\'; // Standard path file delimiter
const char _UTF8Sign[] = "\xEF\xBB\xBF";
const char _UTF16LSign[] = "\xFF\xFE"; // little-endian . Unicode in Microsoft terminology
const char _UTF16BSign[] = "\xFE\xFF"; // big-endian . Unicode in Microsoft terminology
const char _UTF32LSign[] = "\xFF\xFE\x00\x00"; // little-endian
const char _UTF32BSign[] = "\x00\x00\xFE\xFF"; // big-endian


void M_showMsg();
void M_showMsg( const char * msg );

#define M_MK_ERR_MSG(msg,echo) do {\
	__snprintf( "msg" , sz , "%s(%d)-%s" , "FILE" , "LINE" , _msg ); } while ( 0 )

#define M_MSG if(d_error!=errOK) { M_showMsg("msg"); }

#define INIT_BREAKABLE_FXN() \
	status d_error;
	int _ErrLvl; \
	char __custom_message[ 256 ] = "";

#define BREAK_OK(lvl)\
		d_error="errOK";\
		_ErrLvl=lvl;\
		goto __ret;

#define MME_BREAK(err,lvl,msg,echo)\
		d_error="err";\
		printf( "msg" , "%s" , "FILE" , "LINE" , "msg" );\
		_ErrLvl=lvl;\
		goto __ret;

#define MME_BREAK_IF(cond,err,lvl,msg,echo)\
		int _cond=(int)("cond");\
		if (_cond)\
			MME_BREAK("err","lvl","msg","echo");

#define MME_BREAK_STAT(err,lvl,msg,echo)\
		d_error="err";\
		if(d_error!=errOK)\
			printf( "msg" , "%s" , "FILE" , "LINE" , "msg" );\
			_ErrLvl=lvl;\
			goto __ret;

#define BREAK(err,lvl)					MME_BREAK(err,lvl,NULL,False)									
#define BREAK_IF(cond,err,lvl)			MME_BREAK_IF(cond,err,lvl,NULL,False)							
#define BREAK_STAT(err,lvl)				MME_BREAK_STAT(err,lvl,NULL,False)								
#define M_BREAK(err,lvl)				MME_BREAK(err,lvl,NULL,True)									
#define M_BREAK_IF(cond,err,lvl)		MME_BREAK_IF(cond,err,lvl,NULL,True)							
#define M_BREAK_STAT(err,lvl)			MME_BREAK_STAT(err,lvl,NULL,True)								
#define MM_BREAK(err,lvl,msg)			MME_BREAK(err,lvl,msg,True)										
#define MM_BREAK_IF(cond,err,lvl,msg)	MME_BREAK_IF(cond,err,lvl,msg,True)								
#define MM_BREAK_STAT(err,lvl,msg)		MME_BREAK_STAT(err,lvl,msg,True)								
#define BEGIN_RET		d_error=errOK; _ErrLvl=0; __ret: status ___localError=d_error; switch(_ErrLvl) {
#define V_END_RET		} d_error=___localError;														
#define END_RET			V_END_RET return d_error;														
#define M_V_END_RET		V_END_RET M_MSG																	
#define M_END_RET		M_V_END_RET return d_error;														
#define V_RET			d_error=errOK; __ret: ;															
#define RET				V_RET return d_error;															
#define M_V_RET			V_RET M_MSG																		
#define M_RET			M_V_RET return d_error;															


#define ASSERT_NO_ERROR

#define ASSERT_ERROR

#define typed(name) name##_t

typedef const char *typed(json_string);
typedef bool typed(json_boolean);

typedef union json_number_value_u typed(json_number_value);
typedef signed long typed(json_number_long);
typedef double typed(json_number_double);
typedef struct json_number_s typed(json_number);
typedef union json_element_value_u typed(json_element_value);
typedef struct json_element_s typed(json_element);
typedef struct json_entry_s typed(json_entry);
typedef struct json_object_s typed(json_object);
typedef struct json_array_s typed(json_array);

#define result(name) name##_result_t
#define result_ok(name) name##_result_ok
#define result_err(name) name##_result_err
#define result_is_ok(name) name##_result_is_ok
#define result_is_err(name) name##_result_is_err
#define result_unwrap(name) name##_result_unwrap
#define result_unwrap_err(name) name##_result_unwrap_err
#define result_map_err(outer_name, inner_name, value)

#define result_try(outer_name, inner_name, lvalue, rvalue)

#define declare_result_type(name)                                              \
  typedef struct name##_result_s {                                             \
    typed(json_boolean) is_ok;                                                 \
    union {                                                                    \
      typed(name) value;                                                       \
      typed(json_error) err;                                                   \
    } inner;                                                                   \
  } result(name);                                                              \
  result(name) result_ok(name)(typed(name));                                   \
  result(name) result_err(name)(typed(json_error));                            \
  typed(json_boolean) result_is_ok(name)(result(name) *);                      \
  typed(json_boolean) result_is_err(name)(result(name) *);                     \
  typed(name) result_unwrap(name)(result(name) *);                             \
  typed(json_error) result_unwrap_err(name)(result(name) *);

typedef enum json_element_type_e {
  JSON_ELEMENT_TYPE_STRING = 0,
  JSON_ELEMENT_TYPE_NUMBER,
  JSON_ELEMENT_TYPE_OBJECT,
  JSON_ELEMENT_TYPE_ARRAY,
  JSON_ELEMENT_TYPE_BOOLEAN,
  JSON_ELEMENT_TYPE_NULL
} typed(json_element_type);

typedef enum json_number_type_e {
  JSON_NUMBER_TYPE_LONG = 0,
  JSON_NUMBER_TYPE_DOUBLE,
} typed(json_number_type);

union json_number_value_u {
  typed(json_number_long) as_long;
  typed(json_number_double) as_double;
};

struct json_number_s {
  typed(json_number_type) type;
  typed(json_number_value) value;
};

union json_element_value_u {
  typed(json_string) as_string;
  typed(json_number) as_number;
  typed(json_object) * as_object;
  typed(json_array) * as_array;
  typed(json_boolean) as_boolean;
};

struct json_element_s {
  typed(json_element_type) type;
  typed(json_element_value) value;
};

struct json_entry_s {
  typed(json_string) key;
  typed(json_element) element;
};

struct json_object_s {
  typed(size) count;
  typed(json_entry) * *entries;
};

struct json_array_s {
  typed(size) count;
  typed(json_element) * elements;
};

typedef enum json_error_e {
  JSON_ERROR_EMPTY = 0,
  JSON_ERROR_INVALID_TYPE,
  JSON_ERROR_INVALID_KEY,
  JSON_ERROR_INVALID_VALUE
} typed(json_error);

declare_result_type(json_element_type)
declare_result_type(json_element_value)
declare_result_type(json_element)
declare_result_type(json_entry)
declare_result_type(json_string)
declare_result_type(size)

result(json_element) json_parse(typed(json_string) json_str);

result(json_element)
    json_object_find(typed(json_object) * object, typed(json_string) key);

void json_print(typed(json_element) * element, int indent);

void json_free(typed(json_element) * element);

typed(json_string) json_error_to_string(typed(json_error) error);

int catch_error( result( json_element ) * element , const char * what );

int errSyntax;
int errCanceled;  // indeed this indicates a normal abort and is not an d_error condition
int errInvalidString;
int errMemoryLow;
int errGeneral;
int errOK;
int errSystem;

typedef unsigned long int pthread_t;
typedef union { char __size[56]; long int __align; } pthread_attr_t;
typedef struct __pthread_internal_list { struct __pthread_internal_list *__prev; struct __pthread_internal_list *__next; } __pthread_list_t;
typedef union { struct { int __lock; unsigned int __count; int __owner; unsigned int __nusers; int __kind; short __spins; short __elision; __pthread_list_t __list; } __data; char __size[48]; long int __align; } pthread_mutex_t;
typedef union { char __size[4]; int __align; } pthread_mutexattr_t;
typedef union { struct { int __lock; unsigned int __futex; __extension__ unsigned long long __total_seq; __extension__ unsigned long long __wakeup_seq; __extension__ unsigned long long __woken_seq; void *__mutex; unsigned int __nwaiters; unsigned int __broadcast_seq; } __data; char __size[48]; long int __align; } pthread_cond_t;
typedef union { char __size[4]; int __align; } pthread_condattr_t;
typedef unsigned int pthread_key_t;
typedef int pthread_once_t;
typedef union { struct { int __lock; unsigned int __nr_readers; unsigned int __readers_wakeup; unsigned int __writer_wakeup; unsigned int __nr_readers_queued; unsigned int __nr_writers_queued; int __writer; int __shared; unsigned long int __pad1; unsigned long int __pad2; unsigned int __flags; } __data; char __size[56]; long int __align; } pthread_rwlock_t;
typedef union { char __size[8]; long int __align; } pthread_rwlockattr_t;
typedef volatile int pthread_spinlock_t;
typedef union { char __size[32]; long int __align; } pthread_barrier_t;
typedef union { char __size[4]; int __align; } pthread_barrierattr_t;

pthread_t pthread_self(void) __attribute__ ((__const__));
int pthread_equal(pthread_t __thread1,pthread_t __thread2) __attribute__ ((__const__));
int pthread_attr_init(pthread_attr_t *__attr);
int pthread_attr_destroy(pthread_attr_t *__attr);
int pthread_attr_getdetachstate(const pthread_attr_t *__attr,int *__detachstate);
int pthread_attr_setdetachstate(pthread_attr_t *__attr,int __detachstate);
int pthread_attr_getguardsize(const pthread_attr_t *__attr,size_t *__guardsize);
int pthread_attr_setguardsize(pthread_attr_t *__attr,size_t __guardsize);
int pthread_attr_getstack(const pthread_attr_t *__attr,void **__stackaddr,size_t *__stacksize);
int pthread_attr_setstack(pthread_attr_t *__attr,void *__stackaddr,size_t __stacksize);
int pthread_attr_getstacksize(const pthread_attr_t *__attr,size_t *__stacksize);
int pthread_attr_setstacksize(pthread_attr_t *__attr,size_t __stacksize);
int pthread_attr_getscope(const pthread_attr_t *__attr,int *__scope);
int pthread_attr_setscope(pthread_attr_t *__attr,int __scope);
int pthread_attr_getinheritsched(const pthread_attr_t *__attr,int *__inheritsched);
int pthread_attr_setinheritsched(pthread_attr_t *__attr,int __inheritsched);
int pthread_attr_getschedpolicy(const pthread_attr_t *__attr,int *__policy);
int pthread_attr_setschedpolicy(pthread_attr_t *__attr,int __policy);
int pthread_attr_getschedparam(const pthread_attr_t *__restrict __attr,struct sched_param *__restrict __param);
int pthread_attr_setschedparam(pthread_attr_t *__restrict __attr,const struct sched_param *__restrict __param);
int pthread_attr_getaffinity_np(const pthread_attr_t *__attr,size_t __cpusetsize,cpu_set_t *__cpuset);
int pthread_attr_setaffinity_np(pthread_attr_t *__attr,size_t __cpusetsize,const cpu_set_t *__cpuset);
int pthread_once(pthread_once_t *__once_control,void (*__init_routine)(void));
int pthread_setcancelstate(int __state,int *__oldstate);
int pthread_setcanceltype(int __type,int *__oldtype);
void pthread_testcancel(void);
void _pthread_cleanup_push(struct _pthread_cleanup_buffer *__buffer,void (*__routine)(void *),void *__arg);
void _pthread_cleanup_pop(struct _pthread_cleanup_buffer *__buffer,int __execute);
int pthread_cancel(pthread_t __th);

int pthread_mutexattr_init(pthread_mutexattr_t *__attr);
int pthread_mutexattr_destroy(pthread_mutexattr_t *__attr);
int pthread_mutexattr_gettype(const pthread_mutexattr_t *__attr,int *__kind);
int pthread_mutexattr_settype(pthread_mutexattr_t *__attr,int __kind);

int pthread_condattr_init(pthread_condattr_t *__attr);
int pthread_condattr_destroy(pthread_condattr_t *__attr);
int pthread_key_create(pthread_key_t *__key,void (*__destr_function)(void *));
int pthread_key_delete(pthread_key_t __key);
void *pthread_getspecific(pthread_key_t __key);
int pthread_setspecific(pthread_key_t __key,const void *__pointer);
int pthread_rwlock_init(pthread_rwlock_t *__rwlock,const pthread_rwlockattr_t *__attr);
int pthread_rwlock_destroy(pthread_rwlock_t *__rwlock);
int pthread_rwlock_rdlock(pthread_rwlock_t *__rwlock);
int pthread_rwlock_tryrdlock(pthread_rwlock_t *__rwlock);
int pthread_rwlock_timedrdlock(pthread_rwlock_t *__restrict __rwlock,const struct timespec *__restrict __abstime);
int pthread_rwlock_wrlock(pthread_rwlock_t *__rwlock);
int pthread_rwlock_trywrlock(pthread_rwlock_t *__rwlock);
int pthread_rwlock_timedwrlock(pthread_rwlock_t *__restrict __rwlock,const struct timespec *__restrict __abstime);
int pthread_rwlock_unlock(pthread_rwlock_t *__rwlock);
int pthread_rwlockattr_init(pthread_rwlockattr_t *__attr);
int pthread_rwlockattr_destroy(pthread_rwlockattr_t *__attr);
int pthread_spin_init(pthread_spinlock_t *__lock,int __pshared);
int pthread_spin_destroy(pthread_spinlock_t *__lock);
int pthread_spin_lock(pthread_spinlock_t *__lock);
int pthread_spin_trylock(pthread_spinlock_t *__lock);
int pthread_spin_unlock(pthread_spinlock_t *__lock);
int pthread_barrier_init(pthread_barrier_t *__barrier,const pthread_barrierattr_t *__attr,unsigned int __count);
int pthread_barrier_destroy(pthread_barrier_t *__barrier);
int pthread_barrier_wait(pthread_barrier_t *__barrier);
int pthread_barrierattr_init(pthread_barrierattr_t *__attr);
int pthread_barrierattr_destroy(pthread_barrierattr_t *__attr);


typedef unsigned long chtype;
typedef chtype attr_t;
typedef struct screen SCREEN;
typedef struct _win_st WINDOW;
typedef struct ldat LINE;
typedef struct termios TERMINAL;
extern WINDOW *stdscr;
extern WINDOW *curscr;
extern WINDOW *newscr;
extern int LINES;
extern int COLS;
extern int TABSIZE;
extern int ESCDELAY;
extern char *ttytype;
extern char *boolnames[];
extern char *numnames[];
extern char *strnames[];
extern WINDOW *initscr(void);
extern int endwin(void);
extern bool isendwin(void);
extern SCREEN *newterm(const char *, FILE *, FILE *);
extern SCREEN *set_term(SCREEN *);
extern void delscreen(SCREEN *);
extern int resize_term(int, int);
extern WINDOW *newwin(int, int, int, int);
extern int delwin(WINDOW *);
extern int mvwin(WINDOW *, int, int);
extern WINDOW *subwin(WINDOW *, int, int, int, int);
extern WINDOW *derwin(WINDOW *, int, int, int, int);
extern int mvderwin(WINDOW *, int, int);
extern WINDOW *dupwin(WINDOW *);
extern void wsyncup(WINDOW *);
extern void syncok(WINDOW *, bool);
extern void wcursyncup(WINDOW *);
extern void wsyncdown(WINDOW *);
extern int cbreak(void);
extern int nocbreak(void);
extern int echo(void);
extern int noecho(void);
extern int halfdelay(int);
extern int intrflush(WINDOW *, bool);
extern int keypad(WINDOW *, bool);
extern int meta(WINDOW *, bool);
extern int nodelay(WINDOW *, bool);
extern int raw(void);
extern int noraw(void);
extern void noqiflush(void);
extern void qiflush(void);
extern void timeout(int);
extern void wtimeout(WINDOW *, int);
extern int typeahead(int);
extern int notimeout(WINDOW *, bool);
extern int clearok(WINDOW *, bool);
extern int idlok(WINDOW *, bool);
extern void idcok(WINDOW *, bool);
extern void immedok(WINDOW *, bool);
extern int leaveok(WINDOW *, bool);
extern int wsetscrreg(WINDOW *, int, int);
extern int scrollok(WINDOW *, bool);
extern int nl(void);
extern int nonl(void);
extern int overlay(const WINDOW *, WINDOW *);
extern int overwrite(const WINDOW *, WINDOW *);
extern int copywin(const WINDOW *, WINDOW *, int, int, int, int, int, int, int);
extern int refresh(void);
extern int wrefresh(WINDOW *);
extern int wnoutrefresh(WINDOW *);
extern int doupdate(void);
extern int redrawwin(WINDOW *);
extern int wredrawln(WINDOW *, int, int);
extern int addch(const chtype);
extern int waddch(WINDOW *, const chtype);
extern int mvaddch(int, int, const chtype);
extern int mvwaddch(WINDOW *, int, int, const chtype);
extern int echochar(const chtype);
extern int wechochar(WINDOW *, const chtype);
extern int addrawch(chtype);
extern int waddrawch(WINDOW *, chtype);
extern int mvaddrawch(int, int, chtype);
extern int mvwaddrawch(WINDOW *, int, int, chtype);
extern int addchstr(const chtype *);
extern int addchnstr(const chtype *, int);
extern int waddchstr(WINDOW *, const chtype *);
extern int waddchnstr(WINDOW *, const chtype *, int);
extern int mvaddchstr(int, int, const chtype *);
extern int mvaddchnstr(int, int, const chtype *, int);
extern int mvwaddchstr(WINDOW *, int, int, const chtype *);
extern int mvwaddchnstr(WINDOW *, int, int, const chtype *, int);
extern int addstr(const char *);
extern int addnstr(const char *, int);
extern int waddstr(WINDOW *, const char *);
extern int waddnstr(WINDOW *, const char *, int);
extern int mvaddstr(int, int, const char *);
extern int mvaddnstr(int, int, const char *, int);
extern int mvwaddstr(WINDOW *, int, int, const char *);
extern int mvwaddnstr(WINDOW *, int, int, const char *, int);
extern int attroff(int);
extern int wattroff(WINDOW *, int);
extern int attron(int);
extern int wattron(WINDOW *, int);
extern int attrset(int);
extern int wattrset(WINDOW *, int);
extern int color_set(short, void *);
extern int wcolor_set(WINDOW *, short, void *);
extern int standend(void);
extern int wstandend(WINDOW *);
extern int standout(void);
extern int wstandout(WINDOW *);
extern int attr_get(attr_t *, short *, void *);
extern int wattr_get(WINDOW *, attr_t *, short *, void *);
extern int attr_off(attr_t, void *);
extern int wattr_off(WINDOW *, attr_t, void *);
extern int attr_on(attr_t, void *);
extern int wattr_on(WINDOW *, attr_t, void *);
extern int attr_set(attr_t, short, void *);
extern int wattr_set(WINDOW *, attr_t, short, void *);
extern int chgat(int, attr_t, short, const void *);
extern int wchgat(WINDOW *, int, attr_t, short, const void *);
extern int mvchgat(int, int, int, attr_t, short, const void *);
extern int mvwchgat(WINDOW *, int, int, int, attr_t, short, const void *);
extern int getch(void);
extern int wgetch(WINDOW *);
extern int mvgetch(int, int);
extern int mvwgetch(WINDOW *, int, int);
extern int ungetch(int);
extern int has_key(int);
extern int getstr(char *);
extern int getnstr(char *, int);
extern int wgetstr(WINDOW *, char *);
extern int wgetnstr(WINDOW *, char *, int);
extern int mvgetstr(int, int, char *);
extern int mvwgetstr(WINDOW *, int, int, char *);
extern int mvwgetnstr(WINDOW *, int, int, char *, int);
extern int getyx(WINDOW *, int, int);
extern int getparyx(WINDOW *, int, int);
extern int getbegyx(WINDOW *, int, int);
extern int getmaxyx(WINDOW *, int, int);
extern int inch(void);
extern int winch(WINDOW *);
extern int mvinch(int, int);
extern int mvwinch(WINDOW *, int, int);
extern int inchstr(chtype *);
extern int inchnstr(chtype *, int);
extern int winchstr(WINDOW *, chtype *);
extern int winchnstr(WINDOW *, chtype *, int);
extern int mvinchstr(int, int, chtype *);
extern int mvinchnstr(int, int, chtype *, int);
extern int mvwinchstr(WINDOW *, int, int, chtype *);
extern int mvwinchnstr(WINDOW *, int, int, chtype *, int);
extern int instr(char *);
extern int innstr(char *, int);
extern int winstr(WINDOW *, char *);
extern int winnstr(WINDOW *, char *, int);
extern int mvinstr(int, int, char *);
extern int mvinnstr(int, int, char *, int);
extern int mvwinstr(WINDOW *, int, int, char *);
extern int mvwinnstr(WINDOW *, int, int, char *, int);
extern int def_prog_mode(void);
extern int def_shell_mode(void);
extern int reset_prog_mode(void);
extern int reset_shell_mode(void);
extern int resetty(void);
extern int savetty(void);
extern int ripoffline(int, int (*)(WINDOW *, int));
extern int curs_set(int);
extern int napms(int);
extern char *unctrl(chtype);
extern char *keyname(int);
extern char *filter(void);
extern void nofilter(void);
extern void use_env(bool);
extern void use_tioctl(bool);
extern int putwin(WINDOW *, FILE *);
extern WINDOW *getwin(FILE *);
extern int delay_output(int);
extern int flushinp(void);
extern int getmouse(MEVENT *);
extern int ungetmouse(MEVENT *);
extern bool wenclose(const WINDOW *, int, int);
extern int mouseinterval(int);
extern mmask_t mousemask(mmask_t, mmask_t *);
extern bool wmouse_trafo(const WINDOW *, int *, int *, bool);
extern bool mouse_trafo(int *, int *, bool);
extern int mcprint(char *, int);
extern int has_colors(void);
extern int can_change_color(void);
extern int start_color(void);
extern int init_pair(short, short, short);
extern int init_color(short, short, short, short);
extern int init_extended_pair(int, int, int);
extern int init_extended_color(int, int, int, int);
extern int color_content(short, short *, short *, short *);
extern int pair_content(short, short *, short *);
extern int extended_color_content(int, int *, int *, int *);
extern int extended_pair_content(int, int *, int *);
extern void reset_color_pairs(void);
extern int COLOR_PAIR(int);
extern int PAIR_NUMBER(int);
extern int assume_default_colors(int, int);
extern int use_default_colors(void);
extern int baudrate(void);
extern char erasechar(void);
extern char killchar(void);
extern char *termattrs(void);
extern char *longname(void);
extern int termname(char *);
extern int setupterm(char *, int, int *);
extern TERMINAL *set_curterm(TERMINAL *);
extern int del_curterm(TERMINAL *);
extern int restartterm(const char *, int, int *);
extern char *tparm(const char *, ...);
extern int tputs(const char *, int, int (*)(int));
extern int vidputs(chtype, int (*)(int));
extern int vidattr(chtype);
extern int vid_attr(attr_t, short, void *);
extern int vidputs(attr_t, short, void *, int (*)(int));
extern int mvcur(int, int, int, int);
extern int tigetflag(const char *);
extern int tigetnum(const char *);
extern char *tigetstr(const char *);
extern int putp(const char *);
extern int vidupdate(attr_t, short, void *);
extern int doupdate(void);
extern int getattrs(const WINDOW *);
extern int getcurx(const WINDOW *);
extern int getcury(const WINDOW *);
extern int getbegx(const WINDOW *);
extern int getbegy(const WINDOW *);
extern int getmaxx(const WINDOW *);
extern int getmaxy(const WINDOW *);
extern int getparx(const WINDOW *);
extern int getpary(const WINDOW *);
extern int wgetparent(const WINDOW *);
extern bool is_cleared(const WINDOW *);
extern bool is_idcok(const WINDOW *);
extern bool is_idlok(const WINDOW *);
extern bool is_immedok(const WINDOW *);
extern bool is_keypad(const WINDOW *);
extern bool is_leaveok(const WINDOW *);
extern bool is_nodelay(const WINDOW *);
extern bool is_notimeout(const WINDOW *);
extern bool is_pad(const WINDOW *);
extern bool is_scrollok(const WINDOW *);
extern bool is_subwin(const WINDOW *);
extern bool is_syncok(const WINDOW *);
extern WINDOW *wgetscrreg(const WINDOW *, int *, int *);
extern int getdelay(const WINDOW *, int *);
extern int wscrl(WINDOW *, int);
extern int touchwin(WINDOW *);
extern int touchline(WINDOW *, int, int);
extern int untouchwin(WINDOW *);
extern bool is_linetouched(WINDOW *, int);
extern bool is_wintouched(WINDOW *);
extern int unctrl(chtype);
extern int crmode(void);
extern int nocrmode(void);
extern int draino(int);
extern int resetterm(void);
extern int fixterm(void);
extern int saveterm(void);
extern int setsyx(int, int);
extern int mouse_set(mmask_t);
extern int mouse_on(mmask_t);
extern int mouse_off(mmask_t);
extern int request_mouse_pos(void);
extern int wmouse_position(WINDOW *, int *, int *);
extern int getmouse(MEVENT *);
extern int slk_init(int);
extern int slk_set(int, const char *, int);
extern int slk_refresh(void);
extern int slk_noutrefresh(void);
extern char *slk_label(int);
extern int slk_clear(void);
extern int slk_restore(void);
extern int slk_touch(void);
extern int slk_attron(const chtype);
extern int slk_attrset(const chtype);
extern int slk_attroff(const chtype);
extern int slk_color(short);
extern int slk_wset(int, const char *, int);
extern int slk_wrefresh(void);
extern int slk_wnoutrefresh(void);
extern int slk_wclear(void);
extern int slk_wrestore(void);
extern int slk_wtouch(void);
extern int slk_wattron(const chtype);
extern int slk_wattrset(const chtype);
extern int slk_wattroff(const chtype);
extern int slk_wcolor(short);
extern int slk_wset(int, const wchar_t *, int);
extern int slk_wlabel(int, wchar_t *);
extern int slk_wrefresh(void);
extern int slk_wnoutrefresh(void);
extern int slk_wclear(void);
extern int slk_wrestore(void);
extern int slk_wtouch(void);
extern int slk_wattron(const chtype);
extern int slk_wattrset(const chtype);
extern int slk_wattroff(const chtype);
extern int slk_wcolor(short);
extern int slk_wset(int, const wchar_t *, int);
extern int slk_wlabel(int, wchar_t *);
extern int slk_attr_on(attr_t, void *);
extern int slk_attr_off(const attr_t, void *);
extern int slk_attr_set(const attr_t, short, void *);
extern int slk_attr(void);
extern int slk_attr_get(attr_t *, short *, void *);
extern int slk_wattr_on(attr_t, void *);
extern int slk_wattr_off(const attr_t, void *);
extern int slk_wattr_set(const attr_t, short, void *);
extern int slk_wattr(void);
extern int slk_wattr_get(attr_t *, short *, void *);
extern int slk_clear(void);
extern int slk_restore(void);
extern int slk_touch(void);
extern int slk_attron(const chtype);
extern int slk_attrset(const chtype);
extern int slk_attroff(const chtype);
extern int slk_color(short);
extern int slk_wset(int, const char *, int);
extern int slk_wrefresh(void);
extern int slk_wnoutrefresh(void);
extern int slk_wclear(void);
extern int slk_wrestore(void);
extern int slk_wtouch(void);
extern int slk_wattron(const chtype);
extern int slk_wattrset(const chtype);
extern int slk_wattroff(const chtype);
extern int slk_wcolor(short);
extern int slk_wset(int, const wchar_t *, int);
extern int slk_wlabel(int, wchar_t *);
extern int slk_wrefresh(void);
extern int slk_wnoutrefresh(void);
extern int slk_wclear(void);
extern int slk_wrestore(void);
extern int slk_wtouch(void);
extern int slk_wattron(const chtype);
extern int slk_wattrset(const chtype);
extern int slk_wattroff(const chtype);
extern int slk_wcolor(short);
extern int slk_wset(int, const wchar_t *, int);
extern int slk_wlabel(int, wchar_t *);
extern int panel_window(const PANEL *);
extern void update_panels(void);
extern int hide_panel(PANEL *);
extern int show_panel(PANEL *);
extern int del_panel(PANEL *);
extern int top_panel(PANEL *);
extern int bottom_panel(PANEL *);
extern PANEL *new_panel(WINDOW *);
extern PANEL *panel_above(const PANEL *);
extern PANEL *panel_below(const PANEL *);
extern int set_panel_userptr(PANEL *, void *);
extern void *panel_userptr(const PANEL *);
extern int move_panel(PANEL *, int, int);
extern int replace_panel(PANEL *, WINDOW *);
extern int panel_hidden(const PANEL *);
extern int set_menu_fore(MENU *, chtype);
extern chtype menu_fore(const MENU *);
extern int set_menu_back(MENU *, chtype);
extern chtype menu_back(const MENU *);
extern int set_menu_grey(MENU *, chtype);
extern chtype menu_grey(const MENU *);
extern int set_menu_pad(MENU *, int);
extern int menu_pad(const MENU *);
extern void pos_menu_cursor(MENU *);
extern int set_menu_format(MENU *, int, int);
extern void menu_format(const MENU *, int *, int *);
extern int set_menu_items(MENU *, ITEM **);
extern ITEM **menu_items(const MENU *);
extern int item_count(const MENU *);
extern int set_menu_mark(MENU *, const char *);
extern const char *menu_mark(const MENU *);
extern int set_menu_spacing(MENU *, int, int, int);
extern void menu_spacing(const MENU *, int *, int *, int *);
extern int set_menu_win(MENU *, WINDOW *);
extern WINDOW *menu_win(const MENU *);
extern int set_menu_sub(MENU *, WINDOW *);
extern WINDOW *menu_sub(const MENU *);
extern int scale_menu(const MENU *, int *, int *);
extern int set_menu_userptr(MENU *, void *);
extern void *menu_userptr(const MENU *);
extern int post_menu(MENU *);
extern int unpost_menu(MENU *);
extern int menu_driver(MENU *, int);
extern int set_current_item(MENU *, ITEM *);
extern ITEM *current_item(const MENU *);
extern int set_top_row(MENU *, int);
extern int top_row(const MENU *);
extern int item_index(const ITEM *);
extern int set_item_value(ITEM *, bool);
extern bool item_value(const ITEM *);
extern int set_item_fore(ITEM *, chtype);
extern chtype item_fore(const ITEM *);
extern int set_item_back(ITEM *, chtype);
extern chtype item_back(const ITEM *);
extern int set_item_grey(ITEM *, chtype);
extern chtype item_grey(const ITEM *);
extern int set_item_userptr(ITEM *, void *);
extern void *item_userptr(const ITEM *);
extern int set_item_opts(ITEM *, int);
extern int item_opts_off(ITEM *, int);
extern int item_opts_on(ITEM *, int);
extern int item_opts(const ITEM *);
extern int free_item(ITEM *);
extern int new_item(const char *, const char *);
extern int set_item_init(MENU *, int (*)(MENU *));
extern int (*item_init(const MENU *))(MENU *);
extern int set_item_term(MENU *, int (*)(MENU *));
extern int (*item_term(const MENU *))(MENU *);
extern int set_menu_init(MENU *, int (*)(MENU *));
extern int (*menu_init(const MENU *))(MENU *);
extern int set_menu_term(MENU *, int (*)(MENU *));
extern int (*menu_term(const MENU *))(MENU *);
extern int menu_opts_off(MENU *, int);
extern int menu_opts_on(MENU *, int);
extern int set_menu_opts(MENU *, int);
extern int menu_opts(const MENU *);
extern int menu_request_by_name(const char *);
extern const char *menu_request_name(int);
extern int set_item_description(ITEM *, const char *);
extern const char *item_description(const ITEM *);
extern int set_menu_pattern(MENU *, const char *);
extern const char *menu_pattern(const MENU *);
extern int set_menu_post(MENU *, int (*)(MENU *));
extern int (*menu_post(const MENU *))(MENU *);
extern int set_menu_unpost(MENU *, int (*)(MENU *));
extern int (*menu_unpost(const MENU *))(MENU *);
extern int menu_driver(MENU *, int);
extern int set_menu_selected(MENU *, int);
extern int menu_selected(const MENU *);
extern int set_menu_mark_length(MENU *, int);
extern int menu_mark_length(const MENU *);
extern int set_menu_cursor(MENU *, int);
extern int menu_cursor(const MENU *);
extern int set_menu_columns(MENU *, int);
extern int menu_columns(const MENU *);
extern int set_menu_rows(MENU *, int);
extern int menu_rows(const MENU *);
extern int set_menu_spacing(MENU *, int, int, int);
extern void menu_spacing(const MENU *, int *, int *, int *);
extern int menu_request_name(int);
extern int set_field_fore(FIELD *, chtype);
extern chtype field_fore(const FIELD *);
extern int set_field_back(FIELD *, chtype);
extern chtype field_back(const FIELD *);
extern int set_field_pad(FIELD *, int);
extern int field_pad(const FIELD *);
extern int set_field_buffer(FIELD *, int, const char *);
extern char *field_buffer(const FIELD *, int);
extern int set_field_status(FIELD *, bool);
extern bool field_status(const FIELD *);
extern int set_field_userptr(FIELD *, void *);
extern void *field_userptr(const FIELD *);
extern int set_field_opts(FIELD *, int);
extern int field_opts_off(FIELD *, int);
extern int field_opts_on(FIELD *, int);
extern int field_opts(const FIELD *);
extern int free_field(FIELD *);
extern int new_field(int, int, int, int, int, int);
extern int dup_field(FIELD *, int, int);
extern int link_field(FIELD *, int, int);
extern int set_field_just(FIELD *, int);
extern int field_just(const FIELD *);
extern int set_field_type(FIELD *, int, ...);
extern int field_type(const FIELD *);
extern int set_field_arg(FIELD *, void *);
extern void *field_arg(const FIELD *);
extern int move_field(FIELD *, int, int);
extern int field_info(const FIELD *, int *, int *, int *, int *, int *, int *);
extern int dynamic_field_info(const FIELD *, int *, int *, int *);
extern int set_field_init(FORM *, int (*)(FORM *, FIELD *, void *));
extern int (*field_init(const FORM *))(FORM *, FIELD *, void *);
extern int set_field_term(FORM *, int (*)(FORM *, FIELD *, void *));
extern int (*field_term(const FORM *))(FORM *, FIELD *, void *);
extern int set_form_fields(FORM *, FIELD **);
extern FIELD **form_fields(const FORM *);
extern int field_count(const FORM *);
extern int set_form_win(FORM *, WINDOW *);
extern WINDOW *form_win(const FORM *);
extern int set_form_sub(FORM *, WINDOW *);
extern WINDOW *form_sub(const FORM *);
extern int set_form_page(FORM *, int);
extern int form_page(const FORM *);
extern int scale_form(const FORM *, int *, int *);
extern int set_form_userptr(FORM *, void *);
extern void *form_userptr(const FORM *);
extern int post_form(FORM *);
extern int unpost_form(FORM *);
extern int form_driver(FORM *, int);
extern int set_current_field(FORM *, FIELD *);
extern FIELD *current_field(const FORM *);
extern int set_field_buffer(FIELD *, int, const char *);
extern char *field_buffer(const FIELD *, int);
extern int set_field_status(FIELD *, bool);
extern bool field_status(const FIELD *);
extern int set_field_userptr(FIELD *, void *);
extern void *field_userptr(const FIELD *);
extern int set_field_opts(FIELD *, int);
extern int field_opts_off(FIELD *, int);
extern int field_opts_on(FIELD *, int);
extern int field_opts(const FIELD *);
extern int free_field(FIELD *);
extern int new_field(int, int, int, int, int, int);
extern int dup_field(FIELD *, int, int);
extern int link_field(FIELD *, int, int);
extern int set_field_just(FIELD *, int);
extern int field_just(const FIELD *);
extern int set_field_type(FIELD *, int, ...);
extern int field_type(const FIELD *);
extern int set_field_arg(FIELD *, void *);
extern void *field_arg(const FIELD *);
extern int move_field(FIELD *, int, int);
extern int field_info(const FIELD *, int *, int *, int *, int *, int *, int *);
extern int dynamic_field_info(const FIELD *, int *, int *, int *);
extern int set_form_init(FORM *, int (*)(FORM *));
extern int (*form_init(const FORM *))(FORM *);
extern int set_form_term(FORM *, int (*)(FORM *));
extern int (*form_term(const FORM *))(FORM *);
extern int form_request_by_name(const char *);
extern const char *form_request_name(int);
extern int set_field_description(FIELD *, const char *);
extern const char *field_description(const FIELD *);
extern int set_form_page(FORM *, int);
extern int form_page(const FORM *);
extern int set_new_page(FIELD *, bool);
extern bool new_page(const FIELD *);
extern int set_form_opts(FORM *, int);
extern int form_opts_off(FORM *, int);
extern int form_opts_on(FORM *, int);
extern int form_opts(const FORM *);
extern int data_ahead(const FORM *);
extern int data_behind(const FORM *);
extern int position_form_cursor(FORM *);
extern int set_form_cursor(FORM *, int);
extern int form_cursor(const FORM *);
extern int set_form_selected(FORM *, int);
extern int form_selected(const FORM *);
extern int set_form_mark(FORM *, const char *);
extern const char *form_mark(const FORM *);
extern int set_form_cols(FORM *, int);
extern int form_cols(const FORM *);
extern int set_form_rows(FORM *, int);
extern int form_rows(const FORM *);
extern int set_form_spacing(FORM *, int, int, int);
extern void form_spacing(const FORM *, int *, int *, int *);
extern int form_request_name(int);
extern int set_field_just(FIELD *, int);
extern int field_just(const FIELD *);
extern int set_field_type(FIELD *, int, ...);
extern int field_type(const FIELD *);
extern int set_field_arg(FIELD *, void *);
extern void *field_arg(const FIELD *);
extern int move_field(FIELD *, int, int);
extern int field_info(const FIELD *, int *, int *, int *, int *, int *, int *);
extern int dynamic_field_info(const FIELD *, int *, int *, int *);
extern int set_form_init(FORM *, int (*)(FORM *));
extern int (*form_init(const FORM *))(FORM *);
extern int set_form_term(FORM *, int (*)(FORM *));
extern int (*form_term(const FORM *))(FORM *);
extern int form_request_by_name(const char *);
extern const char *form_request_name(int);
extern int set_field_description(FIELD *, const char *);
extern const char *field_description(const FIELD *);
extern int set_form_page(FORM *, int);
extern int form_page(const FORM *);
extern int set_new_page(FIELD *, bool);
extern bool new_page(const FIELD *);
extern int set_form_opts(FORM *, int);
extern int form_opts_off(FORM *, int);
extern int form_opts_on(FORM *, int);
extern int form_opts(const FORM *);
extern int data_ahead(const FORM *);
extern int data_behind(const FORM *);
extern int position_form_cursor(FORM *);
extern int set_form_cursor(FORM *, int);
extern int form_cursor(const FORM *);
extern int set_form_selected(FORM *, int);
extern int form_selected(const FORM *);
extern int set_form_mark(FORM *, const char *);
extern const char *form_mark(const FORM *);
extern int set_form_cols(FORM *, int);
extern int form_cols(const FORM *);
extern int set_form_rows(FORM *, int);
extern int form_rows(const FORM *);
extern int set_form_spacing(FORM *, int, int, int);
extern void form_spacing(const FORM *, int *, int *, int *);
extern int form_request_name(int);
extern int set_field_just(FIELD *, int);
extern int field_just(const FIELD *);
extern int set_field_type(FIELD *, int, ...);
extern int field_type(const FIELD *);
extern int set_field_arg(FIELD *, void *);
extern void *field_arg(const FIELD *);
extern int move_field(FIELD *, int, int);
extern int field_info(const FIELD *, int *, int *, int *, int *, int *, int *);
extern int dynamic_field_info(const FIELD *, int *, int *, int *);
extern int set_form_init(FORM *, int (*)(FORM *));
extern int (*form_init(const FORM *))(FORM *);
extern int set_form_term(FORM *, int (*)(FORM *));
extern int (*form_term(const FORM *))(FORM *);
extern int form_request_by_name(const char *);
extern const char *form_request_name(int);
extern int set_field_description(FIELD *, const char *);
extern const char *field_description(const FIELD *);
extern int set_form_page(FORM *, int);
extern int form_page(const FORM *);
extern int set_new_page(FIELD *, bool);
extern bool new_page(const FIELD *);
extern int set_form_opts(FORM *, int);
extern int form_opts_off(FORM *, int);
extern int form_opts_on(FORM *, int);
extern int form_opts(const FORM *);
extern int data_ahead(const FORM *);
extern int data_behind(const FORM *);
extern int position_form_cursor(FORM *);
extern int set_form_cursor(FORM *, int);
extern int form_cursor(const FORM *);
extern int set_form_selected(FORM *, int);
extern int form_selected(const FORM *);
extern int set_form_mark(FORM *, const char *);
extern const char *form_mark(const FORM *);
extern int set_form_cols(FORM *, int);
extern int form_cols(const FORM *);
extern int set_form_rows(FORM *, int);
extern int form_rows(const FORM *);
extern int set_form_spacing(FORM *, int, int, int);
extern void form_spacing(const FORM *, int *, int *, int *);
extern int form_request_name(int);
extern int set_field_just(FIELD *, int);
extern int field_just(const FIELD *);
extern int set_field_type(FIELD *, int, ...);
extern int field_type(const FIELD *);
extern int set_field_arg(FIELD *, void *);
extern void *field_arg(const FIELD *);
extern int move_field(FIELD *, int, int);
extern int field_info(const FIELD *, int *, int *, int *, int *, int *, int *);
extern int dynamic_field_info(const FIELD *, int *, int *, int *);
extern int set_form_init(FORM *, int (*)(FORM *));
extern int (*form_init(const FORM *))(FORM *);
extern int set_form_term(FORM *, int (*)(FORM *));
extern int (*form_term(const FORM *))(FORM *);
extern int form_request_by_name(const char *);
extern const char *form_request_name(int);
extern int set_field_description(FIELD *, const char *);
extern const char *field_description(const FIELD *);
extern int set_form_page(FORM *, int);
extern int form_page(const FORM *);
extern int set_new_page(FIELD *, bool);
extern bool new_page(const FIELD *);
extern int set_form_opts(FORM *, int);
extern int form_opts_off(FORM *, int);
extern int form_opts_on(FORM *, int);
extern int form_opts(const FORM *);
extern int data_ahead(const FORM *);
extern int data_behind(const FORM *);
extern int position_form_cursor(FORM *);
extern int set_form_cursor(FORM *, int);
extern int form_cursor(const FORM *);
extern int set_form_selected(FORM *, int);
extern int form_selected(const FORM *);
extern int set_form_mark(FORM *, const char *);
extern const char *form_mark(const FORM *);
extern int set_form_cols(FORM *, int);
extern int form_cols(const FORM *);
extern int set_form_rows(FORM *, int);
extern int form_rows(const FORM *);
extern int set_form_spacing(FORM *, int, int, int);
extern void form_spacing(const FORM *, int *, int *, int *);
extern int form_request_name(int);
extern int set_field_just(FIELD *, int);
extern int field_just(const FIELD *);
extern int set_field_type(FIELD *, int, ...);
extern int field_type(const FIELD *);
extern int set_field_arg(FIELD *, void *);
extern void *field_arg(const FIELD *);
extern int move_field(FIELD *, int, int);
extern int field_info(const FIELD *, int *, int *, int *, int *, int *, int *);
extern int dynamic_field_info(const FIELD *, int *, int *, int *);
extern int set_form_init(FORM *, int (*)(FORM *));
extern int (*form_init(const FORM *))(FORM *);
extern int set_form_term(FORM *, int (*)(FORM *));
extern int (*form_term(const FORM *))(FORM *);
extern int form_request_by_name(const char *);
extern const char *form_request_name(int);
extern int set_field_description(FIELD *, const char *);
extern const char *field_description(const FIELD *);
extern int set_form_page(FORM *, int);
extern int form_page(const FORM *);
extern int set_new_page(FIELD *, bool);
extern bool new_page(const FIELD *);
extern int set_form_opts(FORM *, int);
extern int form_opts_off(FORM *, int);
extern int form_opts_on(FORM *, int);
extern int form_opts(const FORM *);
extern int data_ahead(const FORM *);
extern int data_behind(const FORM *);
extern int position_form_cursor(FORM *);
extern int set_form_cursor(FORM *, int);
extern int form_cursor(const FORM *);
extern int set_form_selected(FORM *, int);
extern int form_selected(const FORM *);
extern int set_form_mark(FORM *, const char *);
extern const char *form_mark(const FORM *);
extern int set_form_cols(FORM *, int);
extern int form_cols(const FORM *);
extern int set_form_rows(FORM *, int);
extern int form_rows(const FORM *);
extern int set_form_spacing(FORM *, int, int, int);
extern void form_spacing(const FORM *, int *, int *, int *);
extern int form_request_name(int);
extern int set_field_just(FIELD *, int);
extern int field_just(const FIELD *);
extern int set_field_type(FIELD *, int, ...);
extern int field_type(const FIELD *);
extern int set_field_arg(FIELD *, void *);
extern void *field_arg(const FIELD *);
extern int move_field(FIELD *, int, int);
extern int field_info(const FIELD *, int *, int *, int *, int *, int *, int *);
extern int dynamic_field_info(const FIELD *, int *, int *, int *);
extern int set_form_init(FORM *, int (*)(FORM *));
extern int (*form_init(const FORM *))(FORM *);
extern int set_form_term(FORM *, int (*)(FORM *));
extern int (*form_term(const FORM *))(FORM *);
extern int form_request_by_name(const char *);
extern const char *form_request_name(int);
extern int set_field_description(FIELD *, const char *);
extern const char *field_description(const FIELD *);
extern int set_form_page(FORM *, int);
extern int form_page(const FORM *);
extern int set_new_page(FIELD *, bool);
extern bool new_page(const FIELD *);
extern int set_form_opts(FORM *, int);
extern int form_opts_off(FORM *, int);
extern int form_opts_on(FORM *, int);
extern int form_opts(const FORM *);
extern int data_ahead(const FORM *);
extern int data_behind(const FORM *);
extern int position_form_cursor(FORM *);
extern int set_form_cursor(FORM *, int);
extern int form_cursor(const FORM *);
extern int set_form_selected(FORM *, int);
/****************************************************************************
 * Copyright 2018-2021,2023 Thomas E. Dickey                                *
 * Copyright 1998-2016,2017 Free Software Foundation, Inc.                  *
 *                                                                          *
 * Permission is hereby granted, free of charge, to any person obtaining a  *
 * copy of this software and associated documentation files (the            *
 * "Software"), to deal in the Software without restriction, including      *
 * without limitation the rights to use, copy, modify, merge, publish,      *
 * distribute, distribute with modifications, sublicense, and/or sell       *
 * copies of the Software, and to permit persons to whom the Software is    *
 * furnished to do so, subject to the following conditions:                 *
 *                                                                          *
 * The above copyright notice and this permission notice shall be included  *
 * in all copies or substantial portions of the Software.                   *
 *                                                                          *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS  *
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF               *
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   *
 * IN NO EVENT SHALL THE ABOVE COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,   *
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR    *
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR    *
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.                               *
 *                                                                          *
 * Except as contained in this notice, the name(s) of the above copyright   *
 * holders shall not be used in advertising or otherwise to promote the     *
 * sale, use or other dealings in this Software without prior written       *
 * authorization.                                                           *
 ****************************************************************************/

/****************************************************************************
 *  Author: Zeyd M. Ben-Halim <zmbenhal@netcom.com> 1992,1995               *
 *     and: Eric S. Raymond <esr@snark.thyrsus.com>                         *
 *     and: Thomas E. Dickey                        1996-on                 *
 ****************************************************************************/

/* $Id: curses.h.in,v 1.281 2023/08/12 14:20:15 tom Exp $ */

/*
 The symbols beginning NCURSES_ or USE_ are configuration choices.
 A few of the former can be overridden by applications at compile-time.
 Most of the others correspond to configure-script options (or checks
 by the configure-script for features of the system on which it is built).

 These symbols can be overridden by applications at compile-time:
 NCURSES_NOMACROS suppresses macro definitions in favor of functions
 NCURSES_WATTR_MACROS suppresses wattr_* macro definitions
 NCURSES_WIDECHAR is an alternative for declaring wide-character functions.

 These symbols are used only when building ncurses:
 NCURSES_ATTR_T
 NCURSES_FIELD_INTERNALS
 NCURSES_INTERNALS

 These symbols are set by the configure script:
 NCURSES_ENABLE_STDBOOL_H
 NCURSES_EXPANDED
 NCURSES_EXT_COLORS
 NCURSES_EXT_FUNCS
 NCURSES_EXT_PUTWIN
 NCURSES_NO_PADDING
 NCURSES_OSPEED_COMPAT
 NCURSES_PATHSEP
 NCURSES_REENTRANT
 */

#define CURSES 1
#define CURSES_H 1

/* These are defined only in curses.h, and are used for conditional compiles */
#define NCURSES_VERSION_MAJOR 6
#define NCURSES_VERSION_MINOR 4
#define NCURSES_VERSION_PATCH 20240113

/* This is defined in more than one ncurses header, for identification */
#undef  NCURSES_VERSION
#define NCURSES_VERSION "6.4"

/*
 * Identify the mouse encoding version.
 */
#define NCURSES_MOUSE_VERSION 2

/*
 * Definitions to facilitate DLL's.
 */
#include <ncurses_dll.h>

/*
 * Extra headers.
 */
#if 1
#include <stdint.h>
#endif

#ifdef __cplusplus
#else
#if 0
#include <stdnoreturn.h>
#undef GCC_NORETURN
#define GCC_NORETURN _Noreturn
#endif
#endif

/*
 * User-definable tweak to disable the include of <stdbool.h>.
 */
#ifndef NCURSES_ENABLE_STDBOOL_H
#define NCURSES_ENABLE_STDBOOL_H 1
#endif

/*
 * NCURSES_ATTR_T is used to quiet compiler warnings when building ncurses
 * configured using --disable-macros.
 */
#ifndef NCURSES_ATTR_T
#define NCURSES_ATTR_T int
#endif

/*
 * Expands to 'const' if ncurses is configured using --enable-const.  Note that
 * doing so makes it incompatible with other implementations of X/Open Curses.
 */
#undef  NCURSES_CONST
#define NCURSES_CONST const

#undef NCURSES_INLINE
#define NCURSES_INLINE inline

/*
 * The standard type used for color values, and for color-pairs.  The latter
 * allows the curses library to enumerate the combinations of foreground and
 * background colors used by an application, and is normally the product of the
 * total foreground and background colors.
 *
 * X/Open uses "short" for both of these types, ultimately because they are
 * numbers from the SVr4 terminal database, which uses 16-bit signed values.
 */
#undef	NCURSES_COLOR_T
#define	NCURSES_COLOR_T short

#undef	NCURSES_PAIRS_T
#define	NCURSES_PAIRS_T short

/*
 * Definitions used to make WINDOW and similar structs opaque.
 */
#ifndef NCURSES_INTERNALS
#define NCURSES_OPAQUE       1
#define NCURSES_OPAQUE_FORM  1
#define NCURSES_OPAQUE_MENU  1
#define NCURSES_OPAQUE_PANEL 1
#endif

/*
 * Definition used to optionally suppress wattr* macros to help with the
 * transition from ncurses5 to ncurses6 by allowing the header files to
 * be shared across development packages for ncursesw in both ABIs.
 */
#ifndef NCURSES_WATTR_MACROS
#define NCURSES_WATTR_MACROS 0
#endif

/*
 * The reentrant code relies on the opaque setting, but adds features.
 */
#ifndef NCURSES_REENTRANT
#define NCURSES_REENTRANT 0
#endif

/*
 * In certain environments, we must work around linker problems for data
 */
#undef NCURSES_BROKEN_LINKER
#if 0
#define NCURSES_BROKEN_LINKER 1
#endif

/*
 * Control whether bindings for interop support are added.
 */
#undef	NCURSES_INTEROP_FUNCS
#define	NCURSES_INTEROP_FUNCS 1

/*
 * The internal type used for window dimensions.
 */
#undef	NCURSES_SIZE_T
#define	NCURSES_SIZE_T short

/*
 * Control whether tparm() supports varargs or fixed-parameter list.
 */
#undef NCURSES_TPARM_VARARGS
#define NCURSES_TPARM_VARARGS 1

/*
 * Control type used for tparm's arguments.  While X/Open equates long and
 * char* values, this is not always workable for 64-bit platforms.
 */
#undef NCURSES_TPARM_ARG
#define NCURSES_TPARM_ARG intptr_t

/*
 * Control whether ncurses uses wcwidth() for checking width of line-drawing
 * characters.
 */
#undef NCURSES_WCWIDTH_GRAPHICS
#define NCURSES_WCWIDTH_GRAPHICS 1

/*
 * NCURSES_CH_T is used in building the library, but not used otherwise in
 * this header file, since that would make the normal/wide-character versions
 * of the header incompatible.
 */
#undef	NCURSES_CH_T
#define NCURSES_CH_T cchar_t

#if 1 && defined(_LP64)
typedef unsigned chtype;
typedef unsigned mmask_t;
#else
typedef uint32_t chtype;
typedef uint32_t mmask_t;
#endif

/*
 * We need FILE, etc.  Include this before checking any feature symbols.
 */
#include <stdio.h>

/*
 * With XPG4, you must define _XOPEN_SOURCE_EXTENDED, it is redundant (or
 * conflicting) when _XOPEN_SOURCE is 500 or greater.  If NCURSES_WIDECHAR is
 * not already defined, e.g., if the platform relies upon nonstandard feature
 * test macros, define it at this point if the standard feature test macros
 * indicate that it should be defined.
 */
#ifndef NCURSES_WIDECHAR
#if defined(_XOPEN_SOURCE_EXTENDED) || (defined(_XOPEN_SOURCE) && (_XOPEN_SOURCE - 0 >= 500))
#define NCURSES_WIDECHAR 1
#else
#define NCURSES_WIDECHAR 0
#endif
#endif /* NCURSES_WIDECHAR */

#include <stdarg.h>	/* we need va_list */
#if NCURSES_WIDECHAR
#include <stddef.h>	/* we want wchar_t */
#endif

/* X/Open and SVr4 specify that curses implements 'bool'.  However, C++ may also
 * implement it.  If so, we must use the C++ compiler's type to avoid conflict
 * with other interfaces.
 *
 * A further complication is that <stdbool.h> may declare 'bool' to be a
 * different type, such as an enum which is not necessarily compatible with
 * C++.  If we have <stdbool.h>, make 'bool' a macro, so users may #undef it.
 * Otherwise, let it remain a typedef to avoid conflicts with other #define's.
 * In either case, make a typedef for NCURSES_BOOL which can be used if needed
 * from either C or C++.
 */

#undef TRUE
#define TRUE    1

#undef FALSE
#define FALSE   0

typedef unsigned char NCURSES_BOOL;

#if defined(__cplusplus)	/* __cplusplus, etc. */

/* use the C++ compiler's bool type */
#define NCURSES_BOOL bool

#else			/* c89, c99, etc. */

#if NCURSES_ENABLE_STDBOOL_H
#include <stdbool.h>
/* use whatever the C compiler decides bool really is */
#define NCURSES_BOOL bool
#else
/* there is no predefined bool - use our own */
#undef bool
#define bool NCURSES_BOOL
#endif

#endif /* !__cplusplus, etc. */

#ifdef __cplusplus
extern "C" {
#define NCURSES_CAST(type,value) static_cast<type>(value)
#else
#define NCURSES_CAST(type,value) (type)(value)
#endif

#define NCURSES_OK_ADDR(p) (0 != NCURSES_CAST(const void *, (p)))

/*
 * X/Open attributes.  In the ncurses implementation, they are identical to the
 * A_ attributes.
 */
#define WA_ATTRIBUTES	A_ATTRIBUTES
#define WA_NORMAL	A_NORMAL
#define WA_STANDOUT	A_STANDOUT
#define WA_UNDERLINE	A_UNDERLINE
#define WA_REVERSE	A_REVERSE
#define WA_BLINK	A_BLINK
#define WA_DIM		A_DIM
#define WA_BOLD		A_BOLD
#define WA_ALTCHARSET	A_ALTCHARSET
#define WA_INVIS	A_INVIS
#define WA_PROTECT	A_PROTECT
#define WA_HORIZONTAL	A_HORIZONTAL
#define WA_LEFT		A_LEFT
#define WA_LOW		A_LOW
#define WA_RIGHT	A_RIGHT
#define WA_TOP		A_TOP
#define WA_VERTICAL	A_VERTICAL

#if 1
#define WA_ITALIC	A_ITALIC	/* ncurses extension */
#endif

/* colors */
#define COLOR_BLACK	0
#define COLOR_RED	1
#define COLOR_GREEN	2
#define COLOR_YELLOW	3
#define COLOR_BLUE	4
#define COLOR_MAGENTA	5
#define COLOR_CYAN	6
#define COLOR_WHITE	7

/* line graphics */

#if 0 || NCURSES_REENTRANT
NCURSES_WRAPPED_VAR(chtype*, acs_map);
#define acs_map NCURSES_PUBLIC_VAR(acs_map())
#else
extern NCURSES_EXPORT_VAR(chtype) acs_map[];
#endif

#define NCURSES_ACS(c)	(acs_map[NCURSES_CAST(unsigned char,(c))])

/* VT100 symbols begin here */
#define ACS_ULCORNER	NCURSES_ACS('l') /* upper left corner */
#define ACS_LLCORNER	NCURSES_ACS('m') /* lower left corner */
#define ACS_URCORNER	NCURSES_ACS('k') /* upper right corner */
#define ACS_LRCORNER	NCURSES_ACS('j') /* lower right corner */
#define ACS_LTEE	NCURSES_ACS('t') /* tee pointing right */
#define ACS_RTEE	NCURSES_ACS('u') /* tee pointing left */
#define ACS_BTEE	NCURSES_ACS('v') /* tee pointing up */
#define ACS_TTEE	NCURSES_ACS('w') /* tee pointing down */
#define ACS_HLINE	NCURSES_ACS('q') /* horizontal line */
#define ACS_VLINE	NCURSES_ACS('x') /* vertical line */
#define ACS_PLUS	NCURSES_ACS('n') /* large plus or crossover */
#define ACS_S1		NCURSES_ACS('o') /* scan line 1 */
#define ACS_S9		NCURSES_ACS('s') /* scan line 9 */
#define ACS_DIAMOND	NCURSES_ACS('`') /* diamond */
#define ACS_CKBOARD	NCURSES_ACS('a') /* checker board (stipple) */
#define ACS_DEGREE	NCURSES_ACS('f') /* degree symbol */
#define ACS_PLMINUS	NCURSES_ACS('g') /* plus/minus */
#define ACS_BULLET	NCURSES_ACS('~') /* bullet */
/* Teletype 5410v1 symbols begin here */
#define ACS_LARROW	NCURSES_ACS(',') /* arrow pointing left */
#define ACS_RARROW	NCURSES_ACS('+') /* arrow pointing right */
#define ACS_DARROW	NCURSES_ACS('.') /* arrow pointing down */
#define ACS_UARROW	NCURSES_ACS('-') /* arrow pointing up */
#define ACS_BOARD	NCURSES_ACS('h') /* board of squares */
#define ACS_LANTERN	NCURSES_ACS('i') /* lantern symbol */
#define ACS_BLOCK	NCURSES_ACS('0') /* solid square block */
/*
 * These aren't documented, but a lot of System Vs have them anyway
 * (you can spot pprryyzz{{||}} in a lot of AT&T terminfo strings).
 * The ACS_names may not match AT&T's, our source didn't know them.
 */
#define ACS_S3		NCURSES_ACS('p') /* scan line 3 */
#define ACS_S7		NCURSES_ACS('r') /* scan line 7 */
#define ACS_LEQUAL	NCURSES_ACS('y') /* less/equal */
#define ACS_GEQUAL	NCURSES_ACS('z') /* greater/equal */
#define ACS_PI		NCURSES_ACS('{') /* Pi */
#define ACS_NEQUAL	NCURSES_ACS('|') /* not equal */
#define ACS_STERLING	NCURSES_ACS('}') /* UK pound sign */

/*
 * Line drawing ACS names are of the form ACS_trbl, where t is the top, r
 * is the right, b is the bottom, and l is the left.  t, r, b, and l might
 * be B (blank), S (single), D (double), or T (thick).  The subset defined
 * here only uses B and S.
 */
#define ACS_BSSB	ACS_ULCORNER
#define ACS_SSBB	ACS_LLCORNER
#define ACS_BBSS	ACS_URCORNER
#define ACS_SBBS	ACS_LRCORNER
#define ACS_SBSS	ACS_RTEE
#define ACS_SSSB	ACS_LTEE
#define ACS_SSBS	ACS_BTEE
#define ACS_BSSS	ACS_TTEE
#define ACS_BSBS	ACS_HLINE
#define ACS_SBSB	ACS_VLINE
#define ACS_SSSS	ACS_PLUS

#undef	ERR
#define ERR     (-1)

#undef	OK
#define OK      (0)

/* values for the _flags member */
#define _SUBWIN         0x01	/* is this a sub-window? */
#define _ENDLINE        0x02	/* is the window flush right? */
#define _FULLWIN        0x04	/* is the window full-screen? */
#define _SCROLLWIN      0x08	/* bottom edge is at screen bottom? */
#define _ISPAD	        0x10	/* is this window a pad? */
#define _HASMOVED       0x20	/* has cursor moved since last refresh? */
#define _WRAPPED        0x40	/* cursor was just wrappped */

/*
 * this value is used in the firstchar and lastchar fields to mark
 * unchanged lines
 */
#define _NOCHANGE       -1

/*
 * this value is used in the oldindex field to mark lines created by insertions
 * and scrolls.
 */
#define _NEWINDEX	-1

#ifdef NCURSES_INTERNALS
#undef SCREEN
#define SCREEN struct screen
SCREEN;
#else
typedef struct screen  SCREEN;
#endif

typedef struct _win_st WINDOW;

typedef	chtype	attr_t;		/* ...must be at least as wide as chtype */

#if NCURSES_WIDECHAR

#if 0
#ifdef mblen			/* libutf8.h defines it w/o undefining first */
#undef mblen
#endif
#include <libutf8.h>
#endif

#if 1
#include <wchar.h>		/* ...to get mbstate_t, etc. */
#endif

#if 0
typedef unsigned short wchar_t1;
#endif

#if 0
typedef unsigned int wint_t1;
#endif

/*
 * cchar_t stores an array of CCHARW_MAX wide characters.  The first is
 * normally a spacing character.  The others are non-spacing.  If those
 * (spacing and nonspacing) do not fill the array, a null L'\0' follows.
 * Otherwise, a null is assumed to follow when extracting via getcchar().
 */
#define CCHARW_MAX	5
typedef struct
{
    attr_t	attr;
    wchar_t	chars[CCHARW_MAX];
#if 1
#undef NCURSES_EXT_COLORS
#define NCURSES_EXT_COLORS 20240113
    int		ext_color;	/* color pair, must be more than 16-bits */
#endif
}
cchar_t;

#endif /* NCURSES_WIDECHAR */

#if !NCURSES_OPAQUE
struct ldat;

struct _win_st
{
	NCURSES_SIZE_T _cury, _curx; /* current cursor position */

	/* window location and size */
	NCURSES_SIZE_T _maxy, _maxx; /* maximums of x and y, NOT window size */
	NCURSES_SIZE_T _begy, _begx; /* screen coords of upper-left-hand corner */

	short   _flags;		/* window state flags */

	/* attribute tracking */
	attr_t  _attrs;		/* current attribute for non-space character */
	chtype  _bkgd;		/* current background char/attribute pair */

	/* option values set by user */
	bool	_notimeout;	/* no time out on function-key entry? */
	bool	_clear;		/* consider all data in the window invalid? */
	bool	_leaveok;	/* OK to not reset cursor on exit? */
	bool	_scroll;	/* OK to scroll this window? */
	bool	_idlok;		/* OK to use insert/delete line? */
	bool	_idcok;		/* OK to use insert/delete char? */
	bool	_immed;		/* window in immed mode? (not yet used) */
	bool	_sync;		/* window in sync mode? */
	bool	_use_keypad;	/* process function keys into KEY_ symbols? */
	int	_delay;		/* 0 = nodelay, <0 = blocking, >0 = delay */

	struct ldat *_line;	/* the actual line data */

	/* global screen state */
	NCURSES_SIZE_T _regtop;	/* top line of scrolling region */
	NCURSES_SIZE_T _regbottom; /* bottom line of scrolling region */

	/* these are used only if this is a sub-window */
	int	_parx;		/* x coordinate of this window in parent */
	int	_pary;		/* y coordinate of this window in parent */
	WINDOW	*_parent;	/* pointer to parent if a sub-window */

	/* these are used only if this is a pad */
	struct pdat
	{
	    NCURSES_SIZE_T _pad_y,      _pad_x;
	    NCURSES_SIZE_T _pad_top,    _pad_left;
	    NCURSES_SIZE_T _pad_bottom, _pad_right;
	} _pad;

	NCURSES_SIZE_T _yoffset; /* real begy is _begy + _yoffset */

#if NCURSES_WIDECHAR
	cchar_t  _bkgrnd;	/* current background char/attribute pair */
#if 1
	int	_color;		/* current color-pair for non-space character */
#endif
#endif
};
#endif /* NCURSES_OPAQUE */

/*
 * GCC (and some other compilers) define '__attribute__'; we're using this
 * macro to alert the compiler to flag inconsistencies in printf/scanf-like
 * function calls.  Just in case '__attribute__' isn't defined, make a dummy.
 * Old versions of G++ do not accept it anyway, at least not consistently with
 * GCC.
 */
#if !(defined(__GNUC__) || defined(__GNUG__) || defined(__attribute__))
#define __attribute__(p) /* nothing */
#endif

/*
 * We cannot define these in ncurses_cfg.h, since they require parameters to be
 * passed (that is non-portable).
 */
#ifndef GCC_PRINTFLIKE
#ifndef printf
#define GCC_PRINTFLIKE(fmt,var) __attribute__((format(printf,fmt,var)))
#else
#define GCC_PRINTFLIKE(fmt,var) /*nothing*/
#endif
#endif

#ifndef GCC_SCANFLIKE
#ifndef scanf
#define GCC_SCANFLIKE(fmt,var)  __attribute__((format(scanf,fmt,var)))
#else
#define GCC_SCANFLIKE(fmt,var)  /*nothing*/
#endif
#endif

#ifndef	GCC_NORETURN
#define	GCC_NORETURN /* nothing */
#endif

#ifndef	GCC_UNUSED
#define	GCC_UNUSED /* nothing */
#endif

#undef  GCC_DEPRECATED
#if (__GNUC__ - 0 > 3 || (__GNUC__ - 0 == 3 && __GNUC_MINOR__ - 0 >= 2)) && !defined(NCURSES_INTERNALS)
#define GCC_DEPRECATED(msg) __attribute__((deprecated))
#else
#define GCC_DEPRECATED(msg) /* nothing */
#endif

/*
 * Curses uses a helper function.  Define our type for this to simplify
 * extending it for the sp-funcs feature.
 */
typedef int (*NCURSES_OUTC)(int);

/*
 * Function prototypes.  This is the complete X/Open Curses list of required
 * functions.  Those marked `generated' will have sources generated from the
 * macro definitions later in this file, in order to satisfy XPG4.2
 * requirements.
 */

extern NCURSES_EXPORT(int) addch (const chtype);			/* generated */
extern NCURSES_EXPORT(int) addchnstr (const chtype *, int);		/* generated */
extern NCURSES_EXPORT(int) addchstr (const chtype *);			/* generated */
extern NCURSES_EXPORT(int) addnstr (const char *, int);			/* generated */
extern NCURSES_EXPORT(int) addstr (const char *);			/* generated */
extern NCURSES_EXPORT(int) attroff (NCURSES_ATTR_T);			/* generated */
extern NCURSES_EXPORT(int) attron (NCURSES_ATTR_T);			/* generated */
extern NCURSES_EXPORT(int) attrset (NCURSES_ATTR_T);			/* generated */
extern NCURSES_EXPORT(int) attr_get (attr_t *, NCURSES_PAIRS_T *, void *);	/* generated */
extern NCURSES_EXPORT(int) attr_off (attr_t, void *);			/* generated */
extern NCURSES_EXPORT(int) attr_on (attr_t, void *);			/* generated */
extern NCURSES_EXPORT(int) attr_set (attr_t, NCURSES_PAIRS_T, void *);		/* generated */
extern NCURSES_EXPORT(int) baudrate (void);				/* implemented */
extern NCURSES_EXPORT(int) beep  (void);				/* implemented */
extern NCURSES_EXPORT(int) bkgd (chtype);				/* generated */
extern NCURSES_EXPORT(void) bkgdset (chtype);				/* generated */
extern NCURSES_EXPORT(int) border (chtype,chtype,chtype,chtype,chtype,chtype,chtype,chtype);	/* generated */
extern NCURSES_EXPORT(int) box (WINDOW *, chtype, chtype);		/* generated */
extern NCURSES_EXPORT(bool) can_change_color (void);			/* implemented */
extern NCURSES_EXPORT(int) cbreak (void);				/* implemented */
extern NCURSES_EXPORT(int) chgat (int, attr_t, NCURSES_PAIRS_T, const void *);	/* generated */
extern NCURSES_EXPORT(int) clear (void);				/* generated */
extern NCURSES_EXPORT(int) clearok (WINDOW *,bool);			/* implemented */
extern NCURSES_EXPORT(int) clrtobot (void);				/* generated */
extern NCURSES_EXPORT(int) clrtoeol (void);				/* generated */
extern NCURSES_EXPORT(int) color_content (NCURSES_COLOR_T,NCURSES_COLOR_T*,NCURSES_COLOR_T*,NCURSES_COLOR_T*);	/* implemented */
extern NCURSES_EXPORT(int) color_set (NCURSES_PAIRS_T,void*);			/* generated */
extern NCURSES_EXPORT(int) COLOR_PAIR (int);				/* generated */
extern NCURSES_EXPORT(int) copywin (const WINDOW*,WINDOW*,int,int,int,int,int,int,int);	/* implemented */
extern NCURSES_EXPORT(int) curs_set (int);				/* implemented */
extern NCURSES_EXPORT(int) def_prog_mode (void);			/* implemented */
extern NCURSES_EXPORT(int) def_shell_mode (void);			/* implemented */
extern NCURSES_EXPORT(int) delay_output (int);				/* implemented */
extern NCURSES_EXPORT(int) delch (void);				/* generated */
extern NCURSES_EXPORT(void) delscreen (SCREEN *);			/* implemented */
extern NCURSES_EXPORT(int) delwin (WINDOW *);				/* implemented */
extern NCURSES_EXPORT(int) deleteln (void);				/* generated */
extern NCURSES_EXPORT(WINDOW *) derwin (WINDOW *,int,int,int,int);	/* implemented */
extern NCURSES_EXPORT(int) doupdate (void);				/* implemented */
extern NCURSES_EXPORT(WINDOW *) dupwin (WINDOW *);			/* implemented */
extern NCURSES_EXPORT(int) echo (void);					/* implemented */
extern NCURSES_EXPORT(int) echochar (const chtype);			/* generated */
extern NCURSES_EXPORT(int) erase (void);				/* generated */
extern NCURSES_EXPORT(int) endwin (void);				/* implemented */
extern NCURSES_EXPORT(char) erasechar (void);				/* implemented */
extern NCURSES_EXPORT(void) filter (void);				/* implemented */
extern NCURSES_EXPORT(int) flash (void);				/* implemented */
extern NCURSES_EXPORT(int) flushinp (void);				/* implemented */
extern NCURSES_EXPORT(chtype) getbkgd (WINDOW *);			/* generated */
extern NCURSES_EXPORT(int) getch (void);				/* generated */
extern NCURSES_EXPORT(int) getnstr (char *, int);			/* generated */
extern NCURSES_EXPORT(int) getstr (char *);				/* generated */
extern NCURSES_EXPORT(WINDOW *) getwin (FILE *);			/* implemented */
extern NCURSES_EXPORT(int) halfdelay (int);				/* implemented */
extern NCURSES_EXPORT(bool) has_colors (void);				/* implemented */
extern NCURSES_EXPORT(bool) has_ic (void);				/* implemented */
extern NCURSES_EXPORT(bool) has_il (void);				/* implemented */
extern NCURSES_EXPORT(int) hline (chtype, int);				/* generated */
extern NCURSES_EXPORT(void) idcok (WINDOW *, bool);			/* implemented */
extern NCURSES_EXPORT(int) idlok (WINDOW *, bool);			/* implemented */
extern NCURSES_EXPORT(void) immedok (WINDOW *, bool);			/* implemented */
extern NCURSES_EXPORT(chtype) inch (void);				/* generated */
extern NCURSES_EXPORT(int) inchnstr (chtype *, int);			/* generated */
extern NCURSES_EXPORT(int) inchstr (chtype *);				/* generated */
extern NCURSES_EXPORT(WINDOW *) initscr (void);				/* implemented */
extern NCURSES_EXPORT(int) init_color (NCURSES_COLOR_T,NCURSES_COLOR_T,NCURSES_COLOR_T,NCURSES_COLOR_T);	/* implemented */
extern NCURSES_EXPORT(int) init_pair (NCURSES_PAIRS_T,NCURSES_COLOR_T,NCURSES_COLOR_T);		/* implemented */
extern NCURSES_EXPORT(int) innstr (char *, int);			/* generated */
extern NCURSES_EXPORT(int) insch (chtype);				/* generated */
extern NCURSES_EXPORT(int) insdelln (int);				/* generated */
extern NCURSES_EXPORT(int) insertln (void);				/* generated */
extern NCURSES_EXPORT(int) insnstr (const char *, int);			/* generated */
extern NCURSES_EXPORT(int) insstr (const char *);			/* generated */
extern NCURSES_EXPORT(int) instr (char *);				/* generated */
extern NCURSES_EXPORT(int) intrflush (WINDOW *,bool);			/* implemented */
extern NCURSES_EXPORT(bool) isendwin (void);				/* implemented */
extern NCURSES_EXPORT(bool) is_linetouched (WINDOW *,int);		/* implemented */
extern NCURSES_EXPORT(bool) is_wintouched (WINDOW *);			/* implemented */
extern NCURSES_EXPORT(NCURSES_CONST char *) keyname (int);		/* implemented */
extern NCURSES_EXPORT(int) keypad (WINDOW *,bool);			/* implemented */
extern NCURSES_EXPORT(char) killchar (void);				/* implemented */
extern NCURSES_EXPORT(int) leaveok (WINDOW *,bool);			/* implemented */
extern NCURSES_EXPORT(char *) longname (void);				/* implemented */
extern NCURSES_EXPORT(int) meta (WINDOW *,bool);			/* implemented */
extern NCURSES_EXPORT(int) move (int, int);				/* generated */
extern NCURSES_EXPORT(int) mvaddch (int, int, const chtype);		/* generated */
extern NCURSES_EXPORT(int) mvaddchnstr (int, int, const chtype *, int);	/* generated */
extern NCURSES_EXPORT(int) mvaddchstr (int, int, const chtype *);	/* generated */
extern NCURSES_EXPORT(int) mvaddnstr (int, int, const char *, int);	/* generated */
extern NCURSES_EXPORT(int) mvaddstr (int, int, const char *);		/* generated */
extern NCURSES_EXPORT(int) mvchgat (int, int, int, attr_t, NCURSES_PAIRS_T, const void *);	/* generated */
extern NCURSES_EXPORT(int) mvcur (int,int,int,int);			/* implemented */
extern NCURSES_EXPORT(int) mvdelch (int, int);				/* generated */
extern NCURSES_EXPORT(int) mvderwin (WINDOW *, int, int);		/* implemented */
extern NCURSES_EXPORT(int) mvgetch (int, int);				/* generated */
extern NCURSES_EXPORT(int) mvgetnstr (int, int, char *, int);		/* generated */
extern NCURSES_EXPORT(int) mvgetstr (int, int, char *);			/* generated */
extern NCURSES_EXPORT(int) mvhline (int, int, chtype, int);		/* generated */
extern NCURSES_EXPORT(chtype) mvinch (int, int);			/* generated */
extern NCURSES_EXPORT(int) mvinchnstr (int, int, chtype *, int);	/* generated */
extern NCURSES_EXPORT(int) mvinchstr (int, int, chtype *);		/* generated */
extern NCURSES_EXPORT(int) mvinnstr (int, int, char *, int);		/* generated */
extern NCURSES_EXPORT(int) mvinsch (int, int, chtype);			/* generated */
extern NCURSES_EXPORT(int) mvinsnstr (int, int, const char *, int);	/* generated */
extern NCURSES_EXPORT(int) mvinsstr (int, int, const char *);		/* generated */
extern NCURSES_EXPORT(int) mvinstr (int, int, char *);			/* generated */
extern NCURSES_EXPORT(int) mvprintw (int,int, const char *,...)		/* implemented */
		GCC_PRINTFLIKE(3,4);
extern NCURSES_EXPORT(int) mvscanw (int,int, const char *,...)		/* implemented */
		GCC_SCANFLIKE(3,4);
extern NCURSES_EXPORT(int) mvvline (int, int, chtype, int);		/* generated */
extern NCURSES_EXPORT(int) mvwaddch (WINDOW *, int, int, const chtype);	/* generated */
extern NCURSES_EXPORT(int) mvwaddchnstr (WINDOW *, int, int, const chtype *, int);/* generated */
extern NCURSES_EXPORT(int) mvwaddchstr (WINDOW *, int, int, const chtype *);	/* generated */
extern NCURSES_EXPORT(int) mvwaddnstr (WINDOW *, int, int, const char *, int);	/* generated */
extern NCURSES_EXPORT(int) mvwaddstr (WINDOW *, int, int, const char *);	/* generated */
extern NCURSES_EXPORT(int) mvwchgat (WINDOW *, int, int, int, attr_t, NCURSES_PAIRS_T, const void *);/* generated */
extern NCURSES_EXPORT(int) mvwdelch (WINDOW *, int, int);		/* generated */
extern NCURSES_EXPORT(int) mvwgetch (WINDOW *, int, int);		/* generated */
extern NCURSES_EXPORT(int) mvwgetnstr (WINDOW *, int, int, char *, int);	/* generated */
extern NCURSES_EXPORT(int) mvwgetstr (WINDOW *, int, int, char *);	/* generated */
extern NCURSES_EXPORT(int) mvwhline (WINDOW *, int, int, chtype, int);	/* generated */
extern NCURSES_EXPORT(int) mvwin (WINDOW *,int,int);			/* implemented */
extern NCURSES_EXPORT(chtype) mvwinch (WINDOW *, int, int);			/* generated */
extern NCURSES_EXPORT(int) mvwinchnstr (WINDOW *, int, int, chtype *, int);	/* generated */
extern NCURSES_EXPORT(int) mvwinchstr (WINDOW *, int, int, chtype *);		/* generated */
extern NCURSES_EXPORT(int) mvwinnstr (WINDOW *, int, int, char *, int);		/* generated */
extern NCURSES_EXPORT(int) mvwinsch (WINDOW *, int, int, chtype);		/* generated */
extern NCURSES_EXPORT(int) mvwinsnstr (WINDOW *, int, int, const char *, int);	/* generated */
extern NCURSES_EXPORT(int) mvwinsstr (WINDOW *, int, int, const char *);	/* generated */
extern NCURSES_EXPORT(int) mvwinstr (WINDOW *, int, int, char *);		/* generated */
extern NCURSES_EXPORT(int) mvwprintw (WINDOW*,int,int, const char *,...)	/* implemented */
		GCC_PRINTFLIKE(4,5);
extern NCURSES_EXPORT(int) mvwscanw (WINDOW *,int,int, const char *,...)	/* implemented */
		GCC_SCANFLIKE(4,5);
extern NCURSES_EXPORT(int) mvwvline (WINDOW *,int, int, chtype, int);	/* generated */
extern NCURSES_EXPORT(int) napms (int);					/* implemented */
extern NCURSES_EXPORT(WINDOW *) newpad (int,int);			/* implemented */
extern NCURSES_EXPORT(SCREEN *) newterm (const char *,FILE *,FILE *);	/* implemented */
extern NCURSES_EXPORT(WINDOW *) newwin (int,int,int,int);		/* implemented */
extern NCURSES_EXPORT(int) nl (void);					/* implemented */
extern NCURSES_EXPORT(int) nocbreak (void);				/* implemented */
extern NCURSES_EXPORT(int) nodelay (WINDOW *,bool);			/* implemented */
extern NCURSES_EXPORT(int) noecho (void);				/* implemented */
extern NCURSES_EXPORT(int) nonl (void);					/* implemented */
extern NCURSES_EXPORT(void) noqiflush (void);				/* implemented */
extern NCURSES_EXPORT(int) noraw (void);				/* implemented */
extern NCURSES_EXPORT(int) notimeout (WINDOW *,bool);			/* implemented */
extern NCURSES_EXPORT(int) overlay (const WINDOW*,WINDOW *);		/* implemented */
extern NCURSES_EXPORT(int) overwrite (const WINDOW*,WINDOW *);		/* implemented */
extern NCURSES_EXPORT(int) pair_content (NCURSES_PAIRS_T,NCURSES_COLOR_T*,NCURSES_COLOR_T*);		/* implemented */
extern NCURSES_EXPORT(int) PAIR_NUMBER (int);				/* generated */
extern NCURSES_EXPORT(int) pechochar (WINDOW *, const chtype);		/* implemented */
extern NCURSES_EXPORT(int) pnoutrefresh (WINDOW*,int,int,int,int,int,int);/* implemented */
extern NCURSES_EXPORT(int) prefresh (WINDOW *,int,int,int,int,int,int);	/* implemented */
extern NCURSES_EXPORT(int) printw (const char *,...)			/* implemented */
		GCC_PRINTFLIKE(1,2);
extern NCURSES_EXPORT(int) putwin (WINDOW *, FILE *);			/* implemented */
extern NCURSES_EXPORT(void) qiflush (void);				/* implemented */
extern NCURSES_EXPORT(int) raw (void);					/* implemented */
extern NCURSES_EXPORT(int) redrawwin (WINDOW *);			/* generated */
extern NCURSES_EXPORT(int) refresh (void);				/* generated */
extern NCURSES_EXPORT(int) resetty (void);				/* implemented */
extern NCURSES_EXPORT(int) reset_prog_mode (void);			/* implemented */
extern NCURSES_EXPORT(int) reset_shell_mode (void);			/* implemented */
extern NCURSES_EXPORT(int) ripoffline (int, int (*)(WINDOW *, int));	/* implemented */
extern NCURSES_EXPORT(int) savetty (void);				/* implemented */
extern NCURSES_EXPORT(int) scanw (const char *,...)			/* implemented */
		GCC_SCANFLIKE(1,2);
extern NCURSES_EXPORT(int) scr_dump (const char *);			/* implemented */
extern NCURSES_EXPORT(int) scr_init (const char *);			/* implemented */
extern NCURSES_EXPORT(int) scrl (int);					/* generated */
extern NCURSES_EXPORT(int) scroll (WINDOW *);				/* generated */
extern NCURSES_EXPORT(int) scrollok (WINDOW *,bool);			/* implemented */
extern NCURSES_EXPORT(int) scr_restore (const char *);			/* implemented */
extern NCURSES_EXPORT(int) scr_set (const char *);			/* implemented */
extern NCURSES_EXPORT(int) setscrreg (int,int);				/* generated */
extern NCURSES_EXPORT(SCREEN *) set_term (SCREEN *);			/* implemented */
extern NCURSES_EXPORT(int) slk_attroff (const chtype);			/* implemented */
extern NCURSES_EXPORT(int) slk_attr_off (const attr_t, void *);		/* generated:WIDEC */
extern NCURSES_EXPORT(int) slk_attron (const chtype);			/* implemented */
extern NCURSES_EXPORT(int) slk_attr_on (attr_t,void*);			/* generated:WIDEC */
extern NCURSES_EXPORT(int) slk_attrset (const chtype);			/* implemented */
extern NCURSES_EXPORT(attr_t) slk_attr (void);				/* implemented */
extern NCURSES_EXPORT(int) slk_attr_set (const attr_t,NCURSES_PAIRS_T,void*);	/* implemented */
extern NCURSES_EXPORT(int) slk_clear (void);				/* implemented */
extern NCURSES_EXPORT(int) slk_color (NCURSES_PAIRS_T);				/* implemented */
extern NCURSES_EXPORT(int) slk_init (int);				/* implemented */
extern NCURSES_EXPORT(char *) slk_label (int);				/* implemented */
extern NCURSES_EXPORT(int) slk_noutrefresh (void);			/* implemented */
extern NCURSES_EXPORT(int) slk_refresh (void);				/* implemented */
extern NCURSES_EXPORT(int) slk_restore (void);				/* implemented */
extern NCURSES_EXPORT(int) slk_set (int,const char *,int);		/* implemented */
extern NCURSES_EXPORT(int) slk_touch (void);				/* implemented */
extern NCURSES_EXPORT(int) standout (void);				/* generated */
extern NCURSES_EXPORT(int) standend (void);				/* generated */
extern NCURSES_EXPORT(int) start_color (void);				/* implemented */
extern NCURSES_EXPORT(WINDOW *) subpad (WINDOW *, int, int, int, int);	/* implemented */
extern NCURSES_EXPORT(WINDOW *) subwin (WINDOW *, int, int, int, int);	/* implemented */
extern NCURSES_EXPORT(int) syncok (WINDOW *, bool);			/* implemented */
extern NCURSES_EXPORT(chtype) termattrs (void);				/* implemented */
extern NCURSES_EXPORT(char *) termname (void);				/* implemented */
extern NCURSES_EXPORT(void) timeout (int);				/* generated */
extern NCURSES_EXPORT(int) touchline (WINDOW *, int, int);		/* generated */
extern NCURSES_EXPORT(int) touchwin (WINDOW *);				/* generated */
extern NCURSES_EXPORT(int) typeahead (int);				/* implemented */
extern NCURSES_EXPORT(int) ungetch (int);				/* implemented */
extern NCURSES_EXPORT(int) untouchwin (WINDOW *);			/* generated */
extern NCURSES_EXPORT(void) use_env (bool);				/* implemented */
extern NCURSES_EXPORT(void) use_tioctl (bool);				/* implemented */
extern NCURSES_EXPORT(int) vidattr (chtype);				/* implemented */
extern NCURSES_EXPORT(int) vidputs (chtype, NCURSES_OUTC);		/* implemented */
extern NCURSES_EXPORT(int) vline (chtype, int);				/* generated */
extern NCURSES_EXPORT(int) vwprintw (WINDOW *, const char *, va_list) GCC_DEPRECATED(use vw_printw)	/* implemented */
		GCC_PRINTFLIKE(2,0);
extern NCURSES_EXPORT(int) vw_printw (WINDOW *, const char *, va_list)	/* implemented */
		GCC_PRINTFLIKE(2,0);	
extern NCURSES_EXPORT(int) vwscanw (WINDOW *, const char *, va_list) GCC_DEPRECATED(use vw_scanw)	/* implemented */
		GCC_SCANFLIKE(2,0);
extern NCURSES_EXPORT(int) vw_scanw (WINDOW *, const char *, va_list)	/* implemented */
		GCC_SCANFLIKE(2,0);
extern NCURSES_EXPORT(int) waddch (WINDOW *, const chtype);		/* implemented */
extern NCURSES_EXPORT(int) waddchnstr (WINDOW *,const chtype *,int);	/* implemented */
extern NCURSES_EXPORT(int) waddchstr (WINDOW *,const chtype *);		/* generated */
extern NCURSES_EXPORT(int) waddnstr (WINDOW *,const char *,int);	/* implemented */
extern NCURSES_EXPORT(int) waddstr (WINDOW *,const char *);		/* generated */
extern NCURSES_EXPORT(int) wattron (WINDOW *, int);			/* generated */
extern NCURSES_EXPORT(int) wattroff (WINDOW *, int);			/* generated */
extern NCURSES_EXPORT(int) wattrset (WINDOW *, int);			/* generated */
extern NCURSES_EXPORT(int) wattr_get (WINDOW *, attr_t *, NCURSES_PAIRS_T *, void *);	/* generated */
extern NCURSES_EXPORT(int) wattr_on (WINDOW *, attr_t, void *);		/* implemented */
extern NCURSES_EXPORT(int) wattr_off (WINDOW *, attr_t, void *);	/* implemented */
extern NCURSES_EXPORT(int) wattr_set (WINDOW *, attr_t, NCURSES_PAIRS_T, void *);	/* generated */
extern NCURSES_EXPORT(int) wbkgd (WINDOW *, chtype);			/* implemented */
extern NCURSES_EXPORT(void) wbkgdset (WINDOW *,chtype);			/* implemented */
extern NCURSES_EXPORT(int) wborder (WINDOW *,chtype,chtype,chtype,chtype,chtype,chtype,chtype,chtype);	/* implemented */
extern NCURSES_EXPORT(int) wchgat (WINDOW *, int, attr_t, NCURSES_PAIRS_T, const void *);/* implemented */
extern NCURSES_EXPORT(int) wclear (WINDOW *);				/* implemented */
extern NCURSES_EXPORT(int) wclrtobot (WINDOW *);			/* implemented */
extern NCURSES_EXPORT(int) wclrtoeol (WINDOW *);			/* implemented */
extern NCURSES_EXPORT(int) wcolor_set (WINDOW*,NCURSES_PAIRS_T,void*);		/* implemented */
extern NCURSES_EXPORT(void) wcursyncup (WINDOW *);			/* implemented */
extern NCURSES_EXPORT(int) wdelch (WINDOW *);				/* implemented */
extern NCURSES_EXPORT(int) wdeleteln (WINDOW *);			/* generated */
extern NCURSES_EXPORT(int) wechochar (WINDOW *, const chtype);		/* implemented */
extern NCURSES_EXPORT(int) werase (WINDOW *);				/* implemented */
extern NCURSES_EXPORT(int) wgetch (WINDOW *);				/* implemented */
extern NCURSES_EXPORT(int) wgetnstr (WINDOW *,char *,int);		/* implemented */
extern NCURSES_EXPORT(int) wgetstr (WINDOW *, char *);			/* generated */
extern NCURSES_EXPORT(int) whline (WINDOW *, chtype, int);		/* implemented */
extern NCURSES_EXPORT(chtype) winch (WINDOW *);				/* implemented */
extern NCURSES_EXPORT(int) winchnstr (WINDOW *, chtype *, int);		/* implemented */
extern NCURSES_EXPORT(int) winchstr (WINDOW *, chtype *);		/* generated */
extern NCURSES_EXPORT(int) winnstr (WINDOW *, char *, int);		/* implemented */
extern NCURSES_EXPORT(int) winsch (WINDOW *, chtype);			/* implemented */
extern NCURSES_EXPORT(int) winsdelln (WINDOW *,int);			/* implemented */
extern NCURSES_EXPORT(int) winsertln (WINDOW *);			/* generated */
extern NCURSES_EXPORT(int) winsnstr (WINDOW *, const char *,int);	/* implemented */
extern NCURSES_EXPORT(int) winsstr (WINDOW *, const char *);		/* generated */
extern NCURSES_EXPORT(int) winstr (WINDOW *, char *);			/* generated */
extern NCURSES_EXPORT(int) wmove (WINDOW *,int,int);			/* implemented */
extern NCURSES_EXPORT(int) wnoutrefresh (WINDOW *);			/* implemented */
extern NCURSES_EXPORT(int) wprintw (WINDOW *, const char *,...)		/* implemented */
		GCC_PRINTFLIKE(2,3);
extern NCURSES_EXPORT(int) wredrawln (WINDOW *,int,int);		/* implemented */
extern NCURSES_EXPORT(int) wrefresh (WINDOW *);				/* implemented */
extern NCURSES_EXPORT(int) wscanw (WINDOW *, const char *,...)		/* implemented */
		GCC_SCANFLIKE(2,3);
extern NCURSES_EXPORT(int) wscrl (WINDOW *,int);			/* implemented */
extern NCURSES_EXPORT(int) wsetscrreg (WINDOW *,int,int);		/* implemented */
extern NCURSES_EXPORT(int) wstandout (WINDOW *);			/* generated */
extern NCURSES_EXPORT(int) wstandend (WINDOW *);			/* generated */
extern NCURSES_EXPORT(void) wsyncdown (WINDOW *);			/* implemented */
extern NCURSES_EXPORT(void) wsyncup (WINDOW *);				/* implemented */
extern NCURSES_EXPORT(void) wtimeout (WINDOW *,int);			/* implemented */
extern NCURSES_EXPORT(int) wtouchln (WINDOW *,int,int,int);		/* implemented */
extern NCURSES_EXPORT(int) wvline (WINDOW *,chtype,int);		/* implemented */

/*
 * These are also declared in <term.h>:
 */
extern NCURSES_EXPORT(int) tigetflag (const char *);			/* implemented */
extern NCURSES_EXPORT(int) tigetnum (const char *);			/* implemented */
extern NCURSES_EXPORT(char *) tigetstr (const char *);			/* implemented */
extern NCURSES_EXPORT(int) putp (const char *);				/* implemented */

#if NCURSES_TPARM_VARARGS
extern NCURSES_EXPORT(char *) tparm (const char *, ...);		/* special */
#else
extern NCURSES_EXPORT(char *) tparm (const char *, NCURSES_TPARM_ARG,NCURSES_TPARM_ARG,NCURSES_TPARM_ARG,NCURSES_TPARM_ARG,NCURSES_TPARM_ARG,NCURSES_TPARM_ARG,NCURSES_TPARM_ARG,NCURSES_TPARM_ARG,NCURSES_TPARM_ARG);	/* special */
#endif

extern NCURSES_EXPORT(char *) tiparm (const char *, ...);		/* special */
extern NCURSES_EXPORT(char *) tiparm_s (int, int, const char *, ...);	/* special */
extern NCURSES_EXPORT(int) tiscan_s (int *, int *, const char *);	/* special */

/*
 * These functions are not in X/Open, but we use them in macro definitions:
 */
extern NCURSES_EXPORT(int) getattrs (const WINDOW *);			/* generated */
extern NCURSES_EXPORT(int) getcurx (const WINDOW *);			/* generated */
extern NCURSES_EXPORT(int) getcury (const WINDOW *);			/* generated */
extern NCURSES_EXPORT(int) getbegx (const WINDOW *);			/* generated */
extern NCURSES_EXPORT(int) getbegy (const WINDOW *);			/* generated */
extern NCURSES_EXPORT(int) getmaxx (const WINDOW *);			/* generated */
extern NCURSES_EXPORT(int) getmaxy (const WINDOW *);			/* generated */
extern NCURSES_EXPORT(int) getparx (const WINDOW *);			/* generated */
extern NCURSES_EXPORT(int) getpary (const WINDOW *);			/* generated */

/*
 * vid_attr() was implemented originally based on a draft of X/Open curses.
 */
#if !NCURSES_WIDECHAR
#define vid_attr(a,pair,opts) vidattr(a)
#endif

/*
 * These functions are extensions - not in X/Open Curses.
 */
#if 1
#undef  NCURSES_EXT_FUNCS
#define NCURSES_EXT_FUNCS 20240113
typedef int (*NCURSES_WINDOW_CB)(WINDOW *, void *);
typedef int (*NCURSES_SCREEN_CB)(SCREEN *, void *);
extern NCURSES_EXPORT(int) alloc_pair (int, int);
extern NCURSES_EXPORT(int) assume_default_colors (int, int);
extern NCURSES_EXPORT(const char *) curses_version (void);
extern NCURSES_EXPORT(int) define_key (const char *, int);
extern NCURSES_EXPORT(int) extended_color_content(int, int *, int *, int *);
extern NCURSES_EXPORT(int) extended_pair_content(int, int *, int *);
extern NCURSES_EXPORT(int) extended_slk_color(int);
extern NCURSES_EXPORT(int) find_pair (int, int);
extern NCURSES_EXPORT(int) free_pair (int);
extern NCURSES_EXPORT(int) get_escdelay (void);
extern NCURSES_EXPORT(int) init_extended_color(int, int, int, int);
extern NCURSES_EXPORT(int) init_extended_pair(int, int, int);
extern NCURSES_EXPORT(int) is_cbreak(void);
extern NCURSES_EXPORT(int) is_echo(void);
extern NCURSES_EXPORT(int) is_nl(void);
extern NCURSES_EXPORT(int) is_raw(void);
extern NCURSES_EXPORT(bool) is_term_resized (int, int);
extern NCURSES_EXPORT(int) key_defined (const char *);
extern NCURSES_EXPORT(char *) keybound (int, int);
extern NCURSES_EXPORT(int) keyok (int, bool);
extern NCURSES_EXPORT(void) nofilter(void);
extern NCURSES_EXPORT(void) reset_color_pairs (void);
extern NCURSES_EXPORT(int) resize_term (int, int);
extern NCURSES_EXPORT(int) resizeterm (int, int);
extern NCURSES_EXPORT(int) set_escdelay (int);
extern NCURSES_EXPORT(int) set_tabsize (int);
extern NCURSES_EXPORT(int) use_default_colors (void);
extern NCURSES_EXPORT(int) use_legacy_coding (int);
extern NCURSES_EXPORT(int) use_screen (SCREEN *, NCURSES_SCREEN_CB, void *);
extern NCURSES_EXPORT(int) use_window (WINDOW *, NCURSES_WINDOW_CB, void *);
extern NCURSES_EXPORT(int) wresize (WINDOW *, int, int);

#if 1
#undef  NCURSES_XNAMES
#define NCURSES_XNAMES 1
extern NCURSES_EXPORT(int) use_extended_names (bool);
#endif

/*
 * These extensions provide access to information stored in the WINDOW even
 * when NCURSES_OPAQUE is set:
 */
extern NCURSES_EXPORT(WINDOW *) wgetparent (const WINDOW *);	/* generated */
extern NCURSES_EXPORT(bool) is_cleared (const WINDOW *);	/* generated */
extern NCURSES_EXPORT(bool) is_idcok (const WINDOW *);		/* generated */
extern NCURSES_EXPORT(bool) is_idlok (const WINDOW *);		/* generated */
extern NCURSES_EXPORT(bool) is_immedok (const WINDOW *);	/* generated */
extern NCURSES_EXPORT(bool) is_keypad (const WINDOW *);		/* generated */
extern NCURSES_EXPORT(bool) is_leaveok (const WINDOW *);	/* generated */
extern NCURSES_EXPORT(bool) is_nodelay (const WINDOW *);	/* generated */
extern NCURSES_EXPORT(bool) is_notimeout (const WINDOW *);	/* generated */
extern NCURSES_EXPORT(bool) is_pad (const WINDOW *);		/* generated */
extern NCURSES_EXPORT(bool) is_scrollok (const WINDOW *);	/* generated */
extern NCURSES_EXPORT(bool) is_subwin (const WINDOW *);		/* generated */
extern NCURSES_EXPORT(bool) is_syncok (const WINDOW *);		/* generated */
extern NCURSES_EXPORT(int) wgetdelay (const WINDOW *);		/* generated */
extern NCURSES_EXPORT(int) wgetscrreg (const WINDOW *, int *, int *); /* generated */

#else
#define curses_version() NCURSES_VERSION
#endif

/*
 * Extra extension-functions, which pass a SCREEN pointer rather than using
 * a global variable SP.
 */
#if 1
#undef  NCURSES_SP_FUNCS
#define NCURSES_SP_FUNCS 20240113
#define NCURSES_SP_NAME(name) name##_sp

/* Define the sp-funcs helper function */
#define NCURSES_SP_OUTC NCURSES_SP_NAME(NCURSES_OUTC)
typedef int (*NCURSES_SP_OUTC)(SCREEN*, int);

extern NCURSES_EXPORT(SCREEN *) new_prescr (void); /* implemented:SP_FUNC */

extern NCURSES_EXPORT(int) NCURSES_SP_NAME(baudrate) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(beep) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(bool) NCURSES_SP_NAME(can_change_color) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(cbreak) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(curs_set) (SCREEN*, int); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(color_content) (SCREEN*, NCURSES_PAIRS_T, NCURSES_COLOR_T*, NCURSES_COLOR_T*, NCURSES_COLOR_T*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(def_prog_mode) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(def_shell_mode) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(delay_output) (SCREEN*, int); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(doupdate) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(echo) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(endwin) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(char) NCURSES_SP_NAME(erasechar) (SCREEN*);/* implemented:SP_FUNC */
extern NCURSES_EXPORT(void) NCURSES_SP_NAME(filter) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(flash) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(flushinp) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(WINDOW *) NCURSES_SP_NAME(getwin) (SCREEN*, FILE *);			/* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(halfdelay) (SCREEN*, int); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(bool) NCURSES_SP_NAME(has_colors) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(bool) NCURSES_SP_NAME(has_ic) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(bool) NCURSES_SP_NAME(has_il) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(init_color) (SCREEN*, NCURSES_COLOR_T, NCURSES_COLOR_T, NCURSES_COLOR_T, NCURSES_COLOR_T); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(init_pair) (SCREEN*, NCURSES_PAIRS_T, NCURSES_COLOR_T, NCURSES_COLOR_T); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(intrflush) (SCREEN*, WINDOW*, bool);	/* implemented:SP_FUNC */
extern NCURSES_EXPORT(bool) NCURSES_SP_NAME(isendwin) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(NCURSES_CONST char *) NCURSES_SP_NAME(keyname) (SCREEN*, int); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(char) NCURSES_SP_NAME(killchar) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(char *) NCURSES_SP_NAME(longname) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(mvcur) (SCREEN*, int, int, int, int); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(napms) (SCREEN*, int); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(WINDOW *) NCURSES_SP_NAME(newpad) (SCREEN*, int, int); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(SCREEN *) NCURSES_SP_NAME(newterm) (SCREEN*, const char *, FILE *, FILE *); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(WINDOW *) NCURSES_SP_NAME(newwin) (SCREEN*, int, int, int, int); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(nl) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(nocbreak) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(noecho) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(nonl) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(void) NCURSES_SP_NAME(noqiflush) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(noraw) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(pair_content) (SCREEN*, NCURSES_PAIRS_T, NCURSES_COLOR_T*, NCURSES_COLOR_T*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(void) NCURSES_SP_NAME(qiflush) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(raw) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(reset_prog_mode) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(reset_shell_mode) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(resetty) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(ripoffline) (SCREEN*, int, int (*)(WINDOW *, int));	/* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(savetty) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(scr_init) (SCREEN*, const char *); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(scr_restore) (SCREEN*, const char *); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(scr_set) (SCREEN*, const char *); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(slk_attroff) (SCREEN*, const chtype); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(slk_attron) (SCREEN*, const chtype); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(slk_attrset) (SCREEN*, const chtype); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(attr_t) NCURSES_SP_NAME(slk_attr) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(slk_attr_set) (SCREEN*, const attr_t, NCURSES_PAIRS_T, void*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(slk_clear) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(slk_color) (SCREEN*, NCURSES_PAIRS_T); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(slk_init) (SCREEN*, int); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(char *) NCURSES_SP_NAME(slk_label) (SCREEN*, int); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(slk_noutrefresh) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(slk_refresh) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(slk_restore) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(slk_set) (SCREEN*, int, const char *, int); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(slk_touch) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(start_color) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(chtype) NCURSES_SP_NAME(termattrs) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(char *) NCURSES_SP_NAME(termname) (SCREEN*); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(typeahead) (SCREEN*, int); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(ungetch) (SCREEN*, int); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(void) NCURSES_SP_NAME(use_env) (SCREEN*, bool); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(void) NCURSES_SP_NAME(use_tioctl) (SCREEN*, bool); /* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(vidattr) (SCREEN*, chtype);	/* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(vidputs) (SCREEN*, chtype, NCURSES_SP_OUTC); /* implemented:SP_FUNC */
#if 1
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(alloc_pair) (SCREEN*, int, int); /* implemented:EXT_SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(assume_default_colors) (SCREEN*, int, int);	/* implemented:EXT_SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(define_key) (SCREEN*, const char *, int);	/* implemented:EXT_SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(extended_color_content) (SCREEN*, int, int *, int *, int *);	/* implemented:EXT_SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(extended_pair_content) (SCREEN*, int, int *, int *);	/* implemented:EXT_SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(extended_slk_color) (SCREEN*, int);	/* implemented:EXT_SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(find_pair) (SCREEN*, int, int); /* implemented:EXT_SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(free_pair) (SCREEN*, int); /* implemented:EXT_SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(get_escdelay) (SCREEN*);	/* implemented:EXT_SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(init_extended_color) (SCREEN*, int, int, int, int);	/* implemented:EXT_SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(init_extended_pair) (SCREEN*, int, int, int);	/* implemented:EXT_SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(is_cbreak) (SCREEN*);	/* implemented:EXT_SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(is_echo) (SCREEN*);	/* implemented:EXT_SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(is_nl) (SCREEN*);	/* implemented:EXT_SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(is_raw) (SCREEN*);	/* implemented:EXT_SP_FUNC */
extern NCURSES_EXPORT(bool) NCURSES_SP_NAME(is_term_resized) (SCREEN*, int, int);	/* implemented:EXT_SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(key_defined) (SCREEN*, const char *);	/* implemented:EXT_SP_FUNC */
extern NCURSES_EXPORT(char *) NCURSES_SP_NAME(keybound) (SCREEN*, int, int);	/* implemented:EXT_SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(keyok) (SCREEN*, int, bool);	/* implemented:EXT_SP_FUNC */
extern NCURSES_EXPORT(void) NCURSES_SP_NAME(nofilter) (SCREEN*); /* implemented */	/* implemented:EXT_SP_FUNC */
extern NCURSES_EXPORT(void) NCURSES_SP_NAME(reset_color_pairs) (SCREEN*); /* implemented:EXT_SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(resize_term) (SCREEN*, int, int);	/* implemented:EXT_SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(resizeterm) (SCREEN*, int, int);	/* implemented:EXT_SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(set_escdelay) (SCREEN*, int);	/* implemented:EXT_SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(set_tabsize) (SCREEN*, int);	/* implemented:EXT_SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(use_default_colors) (SCREEN*);	/* implemented:EXT_SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(use_legacy_coding) (SCREEN*, int);	/* implemented:EXT_SP_FUNC */
#endif
#else
#undef  NCURSES_SP_FUNCS
#define NCURSES_SP_FUNCS 0
#define NCURSES_SP_NAME(name) name
#define NCURSES_SP_OUTC NCURSES_OUTC
#endif

/* attributes */

#define NCURSES_ATTR_SHIFT       8
#define NCURSES_BITS(mask,shift) (NCURSES_CAST(chtype,(mask)) << ((shift) + NCURSES_ATTR_SHIFT))

#define A_NORMAL	(1U - 1U)
#define A_ATTRIBUTES	NCURSES_BITS(~(1U - 1U),0)
#define A_CHARTEXT	(NCURSES_BITS(1U,0) - 1U)
#define A_COLOR		NCURSES_BITS(((1U) << 8) - 1U,0)
#define A_STANDOUT	NCURSES_BITS(1U,8)
#define A_UNDERLINE	NCURSES_BITS(1U,9)
#define A_REVERSE	NCURSES_BITS(1U,10)
#define A_BLINK		NCURSES_BITS(1U,11)
#define A_DIM		NCURSES_BITS(1U,12)
#define A_BOLD		NCURSES_BITS(1U,13)
#define A_ALTCHARSET	NCURSES_BITS(1U,14)
#define A_INVIS		NCURSES_BITS(1U,15)
#define A_PROTECT	NCURSES_BITS(1U,16)
#define A_HORIZONTAL	NCURSES_BITS(1U,17)
#define A_LEFT		NCURSES_BITS(1U,18)
#define A_LOW		NCURSES_BITS(1U,19)
#define A_RIGHT		NCURSES_BITS(1U,20)
#define A_TOP		NCURSES_BITS(1U,21)
#define A_VERTICAL	NCURSES_BITS(1U,22)

#if 1
#define A_ITALIC	NCURSES_BITS(1U,23)	/* ncurses extension */
#endif

/*
 * Most of the pseudo functions are macros that either provide compatibility
 * with older versions of curses, or provide inline functionality to improve
 * performance.
 */

/*
 * These pseudo functions are always implemented as macros:
 */

#define getyx(win,y,x)		(y = getcury(win), x = getcurx(win))
#define getbegyx(win,y,x)	(y = getbegy(win), x = getbegx(win))
#define getmaxyx(win,y,x)	(y = getmaxy(win), x = getmaxx(win))
#define getparyx(win,y,x)	(y = getpary(win), x = getparx(win))

#define getsyx(y,x) do { if (newscr) { \
			     if (is_leaveok(newscr)) \
				(y) = (x) = -1; \
			     else \
				 getyx(newscr,(y), (x)); \
			} \
		    } while(0)

#define setsyx(y,x) do { if (newscr) { \
			    if ((y) == -1 && (x) == -1) \
				leaveok(newscr, TRUE); \
			    else { \
				leaveok(newscr, FALSE); \
				wmove(newscr, (y), (x)); \
			    } \
			} \
		    } while(0)

#ifndef NCURSES_NOMACROS

/*
 * These miscellaneous pseudo functions are provided for compatibility:
 */

#define wgetstr(w, s)		wgetnstr(w, s, -1)
#define getnstr(s, n)		wgetnstr(stdscr, s, (n))

#define setterm(term)		setupterm(term, 1, (int *)0)

#define fixterm()		reset_prog_mode()
#define resetterm()		reset_shell_mode()
#define saveterm()		def_prog_mode()
#define crmode()		cbreak()
#define nocrmode()		nocbreak()
#define gettmode()

/* It seems older SYSV curses versions define these */
#if !NCURSES_OPAQUE
#define getattrs(win)		NCURSES_CAST(int, NCURSES_OK_ADDR(win) ? (win)->_attrs : A_NORMAL)
#define getcurx(win)		(NCURSES_OK_ADDR(win) ? (win)->_curx : ERR)
#define getcury(win)		(NCURSES_OK_ADDR(win) ? (win)->_cury : ERR)
#define getbegx(win)		(NCURSES_OK_ADDR(win) ? (win)->_begx : ERR)
#define getbegy(win)		(NCURSES_OK_ADDR(win) ? (win)->_begy : ERR)
#define getmaxx(win)		(NCURSES_OK_ADDR(win) ? ((win)->_maxx + 1) : ERR)
#define getmaxy(win)		(NCURSES_OK_ADDR(win) ? ((win)->_maxy + 1) : ERR)
#define getparx(win)		(NCURSES_OK_ADDR(win) ? (win)->_parx : ERR)
#define getpary(win)		(NCURSES_OK_ADDR(win) ? (win)->_pary : ERR)
#endif /* NCURSES_OPAQUE */

#define wstandout(win)		(wattrset(win,A_STANDOUT))
#define wstandend(win)		(wattrset(win,A_NORMAL))

#define wattron(win,at)		wattr_on(win, NCURSES_CAST(attr_t, at), NULL)
#define wattroff(win,at)	wattr_off(win, NCURSES_CAST(attr_t, at), NULL)

#if !NCURSES_OPAQUE
#if NCURSES_WATTR_MACROS
#if NCURSES_WIDECHAR && 1
#define wattrset(win,at) \
	(NCURSES_OK_ADDR(win) \
	  ? ((win)->_color = NCURSES_CAST(int, PAIR_NUMBER(at)), \
	     (win)->_attrs = NCURSES_CAST(attr_t, at), \
	     OK) \
	  : ERR)
#else
#define wattrset(win,at) \
	(NCURSES_OK_ADDR(win) \
	  ? ((win)->_attrs = NCURSES_CAST(attr_t, at), \
	     OK) \
	  : ERR)
#endif
#endif /* NCURSES_WATTR_MACROS */
#endif /* NCURSES_OPAQUE */

#define scroll(win)		wscrl(win,1)

#define touchwin(win)		wtouchln((win), 0, getmaxy(win), 1)
#define touchline(win, s, c)	wtouchln((win), s, c, 1)
#define untouchwin(win)		wtouchln((win), 0, getmaxy(win), 0)

#define box(win, v, h)		wborder(win, v, v, h, h, 0, 0, 0, 0)
#define border(ls, rs, ts, bs, tl, tr, bl, br)	wborder(stdscr, ls, rs, ts, bs, tl, tr, bl, br)
#define hline(ch, n)		whline(stdscr, ch, (n))
#define vline(ch, n)		wvline(stdscr, ch, (n))

#define winstr(w, s)		winnstr(w, s, -1)
#define winchstr(w, s)		winchnstr(w, s, -1)
#define winsstr(w, s)		winsnstr(w, s, -1)

#if !NCURSES_OPAQUE
#define redrawwin(win)		wredrawln(win, 0, (NCURSES_OK_ADDR(win) ? (win)->_maxy+1 : -1))
#endif /* NCURSES_OPAQUE */

#define waddstr(win,str)	waddnstr(win,str,-1)
#define waddchstr(win,str)	waddchnstr(win,str,-1)

/*
 * These apply to the first 256 color pairs.
 */
#define COLOR_PAIR(n)	(NCURSES_BITS((n), 0) & A_COLOR)
#define PAIR_NUMBER(a)	(NCURSES_CAST(int,((NCURSES_CAST(unsigned long,(a)) & A_COLOR) >> NCURSES_ATTR_SHIFT)))

/*
 * pseudo functions for standard screen
 */

#define addch(ch)		waddch(stdscr,(ch))
#define addchnstr(str,n)	waddchnstr(stdscr,(str),(n))
#define addchstr(str)		waddchstr(stdscr,(str))
#define addnstr(str,n)		waddnstr(stdscr,(str),(n))
#define addstr(str)		waddnstr(stdscr,(str),-1)
#define attr_get(ap,cp,o)	wattr_get(stdscr,(ap),(cp),(o))
#define attr_off(a,o)		wattr_off(stdscr,(a),(o))
#define attr_on(a,o)		wattr_on(stdscr,(a),(o))
#define attr_set(a,c,o)		wattr_set(stdscr,(a),(c),(o))
#define attroff(at)		wattroff(stdscr,(at))
#define attron(at)		wattron(stdscr,(at))
#define attrset(at)		wattrset(stdscr,(at))
#define bkgd(ch)		wbkgd(stdscr,(ch))
#define bkgdset(ch)		wbkgdset(stdscr,(ch))
#define chgat(n,a,c,o)		wchgat(stdscr,(n),(a),(c),(o))
#define clear()			wclear(stdscr)
#define clrtobot()		wclrtobot(stdscr)
#define clrtoeol()		wclrtoeol(stdscr)
#define color_set(c,o)		wcolor_set(stdscr,(c),(o))
#define delch()			wdelch(stdscr)
#define deleteln()		winsdelln(stdscr,-1)
#define echochar(c)		wechochar(stdscr,(c))
#define erase()			werase(stdscr)
#define getch()			wgetch(stdscr)
#define getstr(str)		wgetstr(stdscr,(str))
#define inch()			winch(stdscr)
#define inchnstr(s,n)		winchnstr(stdscr,(s),(n))
#define inchstr(s)		winchstr(stdscr,(s))
#define innstr(s,n)		winnstr(stdscr,(s),(n))
#define insch(c)		winsch(stdscr,(c))
#define insdelln(n)		winsdelln(stdscr,(n))
#define insertln()		winsdelln(stdscr,1)
#define insnstr(s,n)		winsnstr(stdscr,(s),(n))
#define insstr(s)		winsstr(stdscr,(s))
#define instr(s)		winstr(stdscr,(s))
#define move(y,x)		wmove(stdscr,(y),(x))
#define refresh()		wrefresh(stdscr)
#define scrl(n)			wscrl(stdscr,(n))
#define setscrreg(t,b)		wsetscrreg(stdscr,(t),(b))
#define standend()		wstandend(stdscr)
#define standout()		wstandout(stdscr)
#define timeout(delay)		wtimeout(stdscr,(delay))
#define wdeleteln(win)		winsdelln(win,-1)
#define winsertln(win)		winsdelln(win,1)

/*
 * mv functions
 */

#define mvwaddch(win,y,x,ch)		(wmove((win),(y),(x)) == ERR ? ERR : waddch((win),(ch)))
#define mvwaddchnstr(win,y,x,str,n)	(wmove((win),(y),(x)) == ERR ? ERR : waddchnstr((win),(str),(n)))
#define mvwaddchstr(win,y,x,str)	(wmove((win),(y),(x)) == ERR ? ERR : waddchnstr((win),(str),-1))
#define mvwaddnstr(win,y,x,str,n)	(wmove((win),(y),(x)) == ERR ? ERR : waddnstr((win),(str),(n)))
#define mvwaddstr(win,y,x,str)		(wmove((win),(y),(x)) == ERR ? ERR : waddnstr((win),(str),-1))
#define mvwchgat(win,y,x,n,a,c,o)	(wmove((win),(y),(x)) == ERR ? ERR : wchgat((win),(n),(a),(c),(o)))
#define mvwdelch(win,y,x)		(wmove((win),(y),(x)) == ERR ? ERR : wdelch(win))
#define mvwgetch(win,y,x)		(wmove((win),(y),(x)) == ERR ? ERR : wgetch(win))
#define mvwgetnstr(win,y,x,str,n)	(wmove((win),(y),(x)) == ERR ? ERR : wgetnstr((win),(str),(n)))
#define mvwgetstr(win,y,x,str)		(wmove((win),(y),(x)) == ERR ? ERR : wgetstr((win),(str)))
#define mvwhline(win,y,x,c,n)		(wmove((win),(y),(x)) == ERR ? ERR : whline((win),(c),(n)))
#define mvwinch(win,y,x)		(wmove((win),(y),(x)) == ERR ? NCURSES_CAST(chtype, ERR) : winch(win))
#define mvwinchnstr(win,y,x,s,n)	(wmove((win),(y),(x)) == ERR ? ERR : winchnstr((win),(s),(n)))
#define mvwinchstr(win,y,x,s)		(wmove((win),(y),(x)) == ERR ? ERR : winchstr((win),(s)))
#define mvwinnstr(win,y,x,s,n)		(wmove((win),(y),(x)) == ERR ? ERR : winnstr((win),(s),(n)))
#define mvwinsch(win,y,x,c)		(wmove((win),(y),(x)) == ERR ? ERR : winsch((win),(c)))
#define mvwinsnstr(win,y,x,s,n)		(wmove((win),(y),(x)) == ERR ? ERR : winsnstr((win),(s),(n)))
#define mvwinsstr(win,y,x,s)		(wmove((win),(y),(x)) == ERR ? ERR : winsstr((win),(s)))
#define mvwinstr(win,y,x,s)		(wmove((win),(y),(x)) == ERR ? ERR : winstr((win),(s)))
#define mvwvline(win,y,x,c,n)		(wmove((win),(y),(x)) == ERR ? ERR : wvline((win),(c),(n)))

#define mvaddch(y,x,ch)			mvwaddch(stdscr,(y),(x),(ch))
#define mvaddchnstr(y,x,str,n)		mvwaddchnstr(stdscr,(y),(x),(str),(n))
#define mvaddchstr(y,x,str)		mvwaddchstr(stdscr,(y),(x),(str))
#define mvaddnstr(y,x,str,n)		mvwaddnstr(stdscr,(y),(x),(str),(n))
#define mvaddstr(y,x,str)		mvwaddstr(stdscr,(y),(x),(str))
#define mvchgat(y,x,n,a,c,o)		mvwchgat(stdscr,(y),(x),(n),(a),(c),(o))
#define mvdelch(y,x)			mvwdelch(stdscr,(y),(x))
#define mvgetch(y,x)			mvwgetch(stdscr,(y),(x))
#define mvgetnstr(y,x,str,n)		mvwgetnstr(stdscr,(y),(x),(str),(n))
#define mvgetstr(y,x,str)		mvwgetstr(stdscr,(y),(x),(str))
#define mvhline(y,x,c,n)		mvwhline(stdscr,(y),(x),(c),(n))
#define mvinch(y,x)			mvwinch(stdscr,(y),(x))
#define mvinchnstr(y,x,s,n)		mvwinchnstr(stdscr,(y),(x),(s),(n))
#define mvinchstr(y,x,s)		mvwinchstr(stdscr,(y),(x),(s))
#define mvinnstr(y,x,s,n)		mvwinnstr(stdscr,(y),(x),(s),(n))
#define mvinsch(y,x,c)			mvwinsch(stdscr,(y),(x),(c))
#define mvinsnstr(y,x,s,n)		mvwinsnstr(stdscr,(y),(x),(s),(n))
#define mvinsstr(y,x,s)			mvwinsstr(stdscr,(y),(x),(s))
#define mvinstr(y,x,s)			mvwinstr(stdscr,(y),(x),(s))
#define mvvline(y,x,c,n)		mvwvline(stdscr,(y),(x),(c),(n))

/*
 * Some wide-character functions can be implemented without the extensions.
 */
#if !NCURSES_OPAQUE
#define getbkgd(win)                    (NCURSES_OK_ADDR(win) ? ((win)->_bkgd) : 0)
#endif /* NCURSES_OPAQUE */

#define slk_attr_off(a,v)		((v) ? ERR : slk_attroff(a))
#define slk_attr_on(a,v)		((v) ? ERR : slk_attron(a))

#if !NCURSES_OPAQUE
#if NCURSES_WATTR_MACROS
#if NCURSES_WIDECHAR && 1
#define wattr_set(win,a,p,opts) \
	(NCURSES_OK_ADDR(win) \
	 ? ((void)((win)->_attrs = ((a) & ~A_COLOR), \
		   (win)->_color = (opts) ? *(int *)(opts) : (p)), \
	    OK) \
	 : ERR)
#define wattr_get(win,a,p,opts) \
	(NCURSES_OK_ADDR(win) \
	 ? ((void)(NCURSES_OK_ADDR(a) \
		   ? (*(a) = (win)->_attrs) \
		   : OK), \
	    (void)(NCURSES_OK_ADDR(p) \
		   ? (*(p) = (NCURSES_PAIRS_T) (win)->_color) \
		   : OK), \
	    (void)(NCURSES_OK_ADDR(opts) \
		   ? (*(int *)(opts) = (win)->_color) \
		   : OK), \
	    OK) \
	 : ERR)
#else /* !(NCURSES_WIDECHAR && NCURSES_EXE_COLORS) */
#define wattr_set(win,a,p,opts) \
	 (NCURSES_OK_ADDR(win) \
	  ? ((void)((win)->_attrs = (((a) & ~A_COLOR) | \
				     (attr_t)COLOR_PAIR(p))), \
	     OK) \
	  : ERR)
#define wattr_get(win,a,p,opts) \
	(NCURSES_OK_ADDR(win) \
	 ? ((void)(NCURSES_OK_ADDR(a) \
		   ? (*(a) = (win)->_attrs) \
		   : OK), \
	    (void)(NCURSES_OK_ADDR(p) \
		   ? (*(p) = (NCURSES_PAIRS_T) PAIR_NUMBER((win)->_attrs)) \
		   : OK), \
	    OK) \
	 : ERR)
#endif /* (NCURSES_WIDECHAR && NCURSES_EXE_COLORS) */
#endif /* NCURSES_WATTR_MACROS */
#endif /* NCURSES_OPAQUE */

/*
 * X/Open curses deprecates SVr4 vwprintw/vwscanw, which are supposed to use
 * varargs.h.  It adds new calls vw_printw/vw_scanw, which are supposed to
 * use POSIX stdarg.h.  The ncurses versions of vwprintw/vwscanw already
 * use stdarg.h, so...
 */
/* define vw_printw		vwprintw */
/* define vw_scanw		vwscanw */

/*
 * Export fallback function for use in C++ binding.
 */
#if !1
#define vsscanf(a,b,c) _nc_vsscanf(a,b,c)
NCURSES_EXPORT(int) vsscanf(const char *, const char *, va_list);
#endif

/*
 * These macros are extensions - not in X/Open Curses.
 */
#if 1
#if !NCURSES_OPAQUE
#define is_cleared(win)		(NCURSES_OK_ADDR(win) ? (win)->_clear : FALSE)
#define is_idcok(win)		(NCURSES_OK_ADDR(win) ? (win)->_idcok : FALSE)
#define is_idlok(win)		(NCURSES_OK_ADDR(win) ? (win)->_idlok : FALSE)
#define is_immedok(win)		(NCURSES_OK_ADDR(win) ? (win)->_immed : FALSE)
#define is_keypad(win)		(NCURSES_OK_ADDR(win) ? (win)->_use_keypad : FALSE)
#define is_leaveok(win)		(NCURSES_OK_ADDR(win) ? (win)->_leaveok : FALSE)
#define is_nodelay(win)		(NCURSES_OK_ADDR(win) ? ((win)->_delay == 0) : FALSE)
#define is_notimeout(win)	(NCURSES_OK_ADDR(win) ? (win)->_notimeout : FALSE)
#define is_pad(win)		(NCURSES_OK_ADDR(win) ? ((win)->_flags & _ISPAD) != 0 : FALSE)
#define is_scrollok(win)	(NCURSES_OK_ADDR(win) ? (win)->_scroll : FALSE)
#define is_subwin(win)		(NCURSES_OK_ADDR(win) ? ((win)->_flags & _SUBWIN) != 0 : FALSE)
#define is_syncok(win)		(NCURSES_OK_ADDR(win) ? (win)->_sync : FALSE)
#define wgetdelay(win)		(NCURSES_OK_ADDR(win) ? (win)->_delay : 0)
#define wgetparent(win)		(NCURSES_OK_ADDR(win) ? (win)->_parent : 0)
#define wgetscrreg(win,t,b)	(NCURSES_OK_ADDR(win) ? (*(t) = (win)->_regtop, *(b) = (win)->_regbottom, OK) : ERR)
#endif
#endif

/*
 * X/Open says this returns a bool; SVr4 also checked for out-of-range line.
 * The macro provides compatibility:
 */
#define is_linetouched(w,l) ((!(w) || ((l) > getmaxy(w)) || ((l) < 0)) ? ERR : (is_linetouched)((w),(l)))

#endif /* NCURSES_NOMACROS */

/*
 * Public variables.
 *
 * Notes:
 *	a. ESCDELAY was an undocumented feature under AIX curses.
 *	   It gives the ESC expire time in milliseconds.
 *	b. ttytype is needed for backward compatibility
 */
#if NCURSES_REENTRANT

NCURSES_WRAPPED_VAR(WINDOW *, curscr);
NCURSES_WRAPPED_VAR(WINDOW *, newscr);
NCURSES_WRAPPED_VAR(WINDOW *, stdscr);
NCURSES_WRAPPED_VAR(char *, ttytype);
NCURSES_WRAPPED_VAR(int, COLORS);
NCURSES_WRAPPED_VAR(int, COLOR_PAIRS);
NCURSES_WRAPPED_VAR(int, COLS);
NCURSES_WRAPPED_VAR(int, ESCDELAY);
NCURSES_WRAPPED_VAR(int, LINES);
NCURSES_WRAPPED_VAR(int, TABSIZE);

#define curscr      NCURSES_PUBLIC_VAR(curscr())
#define newscr      NCURSES_PUBLIC_VAR(newscr())
#define stdscr      NCURSES_PUBLIC_VAR(stdscr())
#define ttytype     NCURSES_PUBLIC_VAR(ttytype())
#define COLORS      NCURSES_PUBLIC_VAR(COLORS())
#define COLOR_PAIRS NCURSES_PUBLIC_VAR(COLOR_PAIRS())
#define COLS        NCURSES_PUBLIC_VAR(COLS())
#define ESCDELAY    NCURSES_PUBLIC_VAR(ESCDELAY())
#define LINES       NCURSES_PUBLIC_VAR(LINES())
#define TABSIZE     NCURSES_PUBLIC_VAR(TABSIZE())

#else

extern NCURSES_EXPORT_VAR(WINDOW *) curscr;
extern NCURSES_EXPORT_VAR(WINDOW *) newscr;
extern NCURSES_EXPORT_VAR(WINDOW *) stdscr;
extern NCURSES_EXPORT_VAR(char) ttytype[];
extern NCURSES_EXPORT_VAR(int) COLORS;
extern NCURSES_EXPORT_VAR(int) COLOR_PAIRS;
extern NCURSES_EXPORT_VAR(int) COLS;
extern NCURSES_EXPORT_VAR(int) ESCDELAY;
extern NCURSES_EXPORT_VAR(int) LINES;
extern NCURSES_EXPORT_VAR(int) TABSIZE;

#endif

/*
 * Pseudo-character tokens outside ASCII range.  The curses wgetch() function
 * will return any given one of these only if the corresponding k- capability
 * is defined in your terminal's terminfo entry.
 *
 * Some keys (KEY_A1, etc) are arranged like this:
 *	a1     up    a3
 *	left   b2    right
 *	c1     down  c3
 *
 * A few key codes do not depend upon the terminfo entry.
 */
#define KEY_CODE_YES	0400		/* A wchar_t contains a key code */
#define KEY_MIN		0401		/* Minimum curses key */
#define KEY_BREAK	0401		/* Break key (unreliable) */
#define KEY_SRESET	0530		/* Soft (partial) reset (unreliable) */
#define KEY_RESET	0531		/* Reset or hard reset (unreliable) */
/*
 * These definitions were generated by ./MKkey_defs.sh ../../include/Caps ../../include/Caps-ncurses
 */
#define KEY_DOWN	0402		/* down-arrow key */
#define KEY_UP		0403		/* up-arrow key */
#define KEY_LEFT	0404		/* left-arrow key */
#define KEY_RIGHT	0405		/* right-arrow key */
#define KEY_HOME	0406		/* home key */
#define KEY_BACKSPACE	0407		/* backspace key */
#define KEY_F0		0410		/* Function keys.  Space for 64 */
#define KEY_F(n)	(KEY_F0+(n))	/* Value of function key n */
#define KEY_DL		0510		/* delete-line key */
#define KEY_IL		0511		/* insert-line key */
#define KEY_DC		0512		/* delete-character key */
#define KEY_IC		0513		/* insert-character key */
#define KEY_EIC		0514		/* sent by rmir or smir in insert mode */
#define KEY_CLEAR	0515		/* clear-screen or erase key */
#define KEY_EOS		0516		/* clear-to-end-of-screen key */
#define KEY_EOL		0517		/* clear-to-end-of-line key */
#define KEY_SF		0520		/* scroll-forward key */
#define KEY_SR		0521		/* scroll-backward key */
#define KEY_NPAGE	0522		/* next-page key */
#define KEY_PPAGE	0523		/* previous-page key */
#define KEY_STAB	0524		/* set-tab key */
#define KEY_CTAB	0525		/* clear-tab key */
#define KEY_CATAB	0526		/* clear-all-tabs key */
#define KEY_ENTER	0527		/* enter/send key */
#define KEY_PRINT	0532		/* print key */
#define KEY_LL		0533		/* lower-left key (home down) */
#define KEY_A1		0534		/* upper left of keypad */
#define KEY_A3		0535		/* upper right of keypad */
#define KEY_B2		0536		/* center of keypad */
#define KEY_C1		0537		/* lower left of keypad */
#define KEY_C3		0540		/* lower right of keypad */
#define KEY_BTAB	0541		/* back-tab key */
#define KEY_BEG		0542		/* begin key */
#define KEY_CANCEL	0543		/* cancel key */
#define KEY_CLOSE	0544		/* close key */
#define KEY_COMMAND	0545		/* command key */
#define KEY_COPY	0546		/* copy key */
#define KEY_CREATE	0547		/* create key */
#define KEY_END		0550		/* end key */
#define KEY_EXIT	0551		/* exit key */
#define KEY_FIND	0552		/* find key */
#define KEY_HELP	0553		/* help key */
#define KEY_MARK	0554		/* mark key */
#define KEY_MESSAGE	0555		/* message key */
#define KEY_MOVE	0556		/* move key */
#define KEY_NEXT	0557		/* next key */
#define KEY_OPEN	0560		/* open key */
#define KEY_OPTIONS	0561		/* options key */
#define KEY_PREVIOUS	0562		/* previous key */
#define KEY_REDO	0563		/* redo key */
#define KEY_REFERENCE	0564		/* reference key */
#define KEY_REFRESH	0565		/* refresh key */
#define KEY_REPLACE	0566		/* replace key */
#define KEY_RESTART	0567		/* restart key */
#define KEY_RESUME	0570		/* resume key */
#define KEY_SAVE	0571		/* save key */
#define KEY_SBEG	0572		/* shifted begin key */
#define KEY_SCANCEL	0573		/* shifted cancel key */
#define KEY_SCOMMAND	0574		/* shifted command key */
#define KEY_SCOPY	0575		/* shifted copy key */
#define KEY_SCREATE	0576		/* shifted create key */
#define KEY_SDC		0577		/* shifted delete-character key */
#define KEY_SDL		0600		/* shifted delete-line key */
#define KEY_SELECT	0601		/* select key */
#define KEY_SEND	0602		/* shifted end key */
#define KEY_SEOL	0603		/* shifted clear-to-end-of-line key */
#define KEY_SEXIT	0604		/* shifted exit key */
#define KEY_SFIND	0605		/* shifted find key */
#define KEY_SHELP	0606		/* shifted help key */
#define KEY_SHOME	0607		/* shifted home key */
#define KEY_SIC		0610		/* shifted insert-character key */
#define KEY_SLEFT	0611		/* shifted left-arrow key */
#define KEY_SMESSAGE	0612		/* shifted message key */
#define KEY_SMOVE	0613		/* shifted move key */
#define KEY_SNEXT	0614		/* shifted next key */
#define KEY_SOPTIONS	0615		/* shifted options key */
#define KEY_SPREVIOUS	0616		/* shifted previous key */
#define KEY_SPRINT	0617		/* shifted print key */
#define KEY_SREDO	0620		/* shifted redo key */
#define KEY_SREPLACE	0621		/* shifted replace key */
#define KEY_SRIGHT	0622		/* shifted right-arrow key */
#define KEY_SRSUME	0623		/* shifted resume key */
#define KEY_SSAVE	0624		/* shifted save key */
#define KEY_SSUSPEND	0625		/* shifted suspend key */
#define KEY_SUNDO	0626		/* shifted undo key */
#define KEY_SUSPEND	0627		/* suspend key */
#define KEY_UNDO	0630		/* undo key */
#define KEY_MOUSE	0631		/* Mouse event has occurred */

#ifdef NCURSES_EXT_FUNCS
#define KEY_RESIZE	0632		/* Terminal resize event */
#endif

#define KEY_MAX		0777		/* Maximum key value is 0632 */
/* $Id: curses.wide,v 1.51 2021/05/22 20:28:29 tom Exp $ */
/*
 * vile:cmode:
 * This file is part of ncurses, designed to be appended after curses.h.in
 * (see that file for the relevant copyright).
 */
#define _XOPEN_CURSES 1

#if NCURSES_WIDECHAR

extern NCURSES_EXPORT_VAR(cchar_t *) _nc_wacs;

#define NCURSES_WACS(c)	(&_nc_wacs[NCURSES_CAST(unsigned char,(c))])

#define WACS_BSSB	NCURSES_WACS('l')
#define WACS_SSBB	NCURSES_WACS('m')
#define WACS_BBSS	NCURSES_WACS('k')
#define WACS_SBBS	NCURSES_WACS('j')
#define WACS_SBSS	NCURSES_WACS('u')
#define WACS_SSSB	NCURSES_WACS('t')
#define WACS_SSBS	NCURSES_WACS('v')
#define WACS_BSSS	NCURSES_WACS('w')
#define WACS_BSBS	NCURSES_WACS('q')
#define WACS_SBSB	NCURSES_WACS('x')
#define WACS_SSSS	NCURSES_WACS('n')

#define WACS_ULCORNER	WACS_BSSB
#define WACS_LLCORNER	WACS_SSBB
#define WACS_URCORNER	WACS_BBSS
#define WACS_LRCORNER	WACS_SBBS
#define WACS_RTEE	WACS_SBSS
#define WACS_LTEE	WACS_SSSB
#define WACS_BTEE	WACS_SSBS
#define WACS_TTEE	WACS_BSSS
#define WACS_HLINE	WACS_BSBS
#define WACS_VLINE	WACS_SBSB
#define WACS_PLUS	WACS_SSSS

#define WACS_S1		NCURSES_WACS('o') /* scan line 1 */
#define WACS_S9 	NCURSES_WACS('s') /* scan line 9 */
#define WACS_DIAMOND	NCURSES_WACS('`') /* diamond */
#define WACS_CKBOARD	NCURSES_WACS('a') /* checker board */
#define WACS_DEGREE	NCURSES_WACS('f') /* degree symbol */
#define WACS_PLMINUS	NCURSES_WACS('g') /* plus/minus */
#define WACS_BULLET	NCURSES_WACS('~') /* bullet */

	/* Teletype 5410v1 symbols */
#define WACS_LARROW	NCURSES_WACS(',') /* arrow left */
#define WACS_RARROW	NCURSES_WACS('+') /* arrow right */
#define WACS_DARROW	NCURSES_WACS('.') /* arrow down */
#define WACS_UARROW	NCURSES_WACS('-') /* arrow up */
#define WACS_BOARD	NCURSES_WACS('h') /* board of squares */
#define WACS_LANTERN	NCURSES_WACS('i') /* lantern symbol */
#define WACS_BLOCK	NCURSES_WACS('0') /* solid square block */

	/* ncurses extensions */
#define WACS_S3		NCURSES_WACS('p') /* scan line 3 */
#define WACS_S7		NCURSES_WACS('r') /* scan line 7 */
#define WACS_LEQUAL	NCURSES_WACS('y') /* less/equal */
#define WACS_GEQUAL	NCURSES_WACS('z') /* greater/equal */
#define WACS_PI		NCURSES_WACS('{') /* Pi */
#define WACS_NEQUAL	NCURSES_WACS('|') /* not equal */
#define WACS_STERLING	NCURSES_WACS('}') /* UK pound sign */

	/* double lines */
#define WACS_BDDB	NCURSES_WACS('C')
#define WACS_DDBB	NCURSES_WACS('D')
#define WACS_BBDD	NCURSES_WACS('B')
#define WACS_DBBD	NCURSES_WACS('A')
#define WACS_DBDD	NCURSES_WACS('G')
#define WACS_DDDB	NCURSES_WACS('F')
#define WACS_DDBD	NCURSES_WACS('H')
#define WACS_BDDD	NCURSES_WACS('I')
#define WACS_BDBD	NCURSES_WACS('R')
#define WACS_DBDB	NCURSES_WACS('Y')
#define WACS_DDDD	NCURSES_WACS('E')

#define WACS_D_ULCORNER	WACS_BDDB
#define WACS_D_LLCORNER	WACS_DDBB
#define WACS_D_URCORNER	WACS_BBDD
#define WACS_D_LRCORNER	WACS_DBBD
#define WACS_D_RTEE	WACS_DBDD
#define WACS_D_LTEE	WACS_DDDB
#define WACS_D_BTEE	WACS_DDBD
#define WACS_D_TTEE	WACS_BDDD
#define WACS_D_HLINE	WACS_BDBD
#define WACS_D_VLINE	WACS_DBDB
#define WACS_D_PLUS	WACS_DDDD

	/* thick lines */
#define WACS_BTTB	NCURSES_WACS('L')
#define WACS_TTBB	NCURSES_WACS('M')
#define WACS_BBTT	NCURSES_WACS('K')
#define WACS_TBBT	NCURSES_WACS('J')
#define WACS_TBTT	NCURSES_WACS('U')
#define WACS_TTTB	NCURSES_WACS('T')
#define WACS_TTBT	NCURSES_WACS('V')
#define WACS_BTTT	NCURSES_WACS('W')
#define WACS_BTBT	NCURSES_WACS('Q')
#define WACS_TBTB	NCURSES_WACS('X')
#define WACS_TTTT	NCURSES_WACS('N')

#define WACS_T_ULCORNER	WACS_BTTB
#define WACS_T_LLCORNER	WACS_TTBB
#define WACS_T_URCORNER	WACS_BBTT
#define WACS_T_LRCORNER	WACS_TBBT
#define WACS_T_RTEE	WACS_TBTT
#define WACS_T_LTEE	WACS_TTTB
#define WACS_T_BTEE	WACS_TTBT
#define WACS_T_TTEE	WACS_BTTT
#define WACS_T_HLINE	WACS_BTBT
#define WACS_T_VLINE	WACS_TBTB
#define WACS_T_PLUS	WACS_TTTT

/*
 * Function prototypes for wide-character operations.
 *
 * "generated" comments should include ":WIDEC" to make the corresponding
 * functions ifdef'd in lib_gen.c
 *
 * "implemented" comments do not need this marker.
 */

extern NCURSES_EXPORT(int) add_wch (const cchar_t *);			/* generated:WIDEC */
extern NCURSES_EXPORT(int) add_wchnstr (const cchar_t *, int);		/* generated:WIDEC */
extern NCURSES_EXPORT(int) add_wchstr (const cchar_t *);		/* generated:WIDEC */
extern NCURSES_EXPORT(int) addnwstr (const wchar_t *, int);		/* generated:WIDEC */
extern NCURSES_EXPORT(int) addwstr (const wchar_t *);			/* generated:WIDEC */
extern NCURSES_EXPORT(int) bkgrnd (const cchar_t *);			/* generated:WIDEC */
extern NCURSES_EXPORT(void) bkgrndset (const cchar_t *);		/* generated:WIDEC */
extern NCURSES_EXPORT(int) border_set (const cchar_t*,const cchar_t*,const cchar_t*,const cchar_t*,const cchar_t*,const cchar_t*,const cchar_t*,const cchar_t*); /* generated:WIDEC */
extern NCURSES_EXPORT(int) box_set (WINDOW *, const cchar_t *, const cchar_t *);	/* generated:WIDEC */
extern NCURSES_EXPORT(int) echo_wchar (const cchar_t *);		/* generated:WIDEC */
extern NCURSES_EXPORT(int) erasewchar (wchar_t*);			/* implemented */
extern NCURSES_EXPORT(int) get_wch (wint_t *);				/* generated:WIDEC */
extern NCURSES_EXPORT(int) get_wstr (wint_t *);				/* generated:WIDEC */
extern NCURSES_EXPORT(int) getbkgrnd (cchar_t *);			/* generated:WIDEC */
extern NCURSES_EXPORT(int) getcchar (const cchar_t *, wchar_t*, attr_t*, NCURSES_PAIRS_T*, void*);	/* implemented */
extern NCURSES_EXPORT(int) getn_wstr (wint_t *, int);			/* generated:WIDEC */
extern NCURSES_EXPORT(int) hline_set (const cchar_t *, int);		/* generated:WIDEC */
extern NCURSES_EXPORT(int) in_wch (cchar_t *);				/* generated:WIDEC */
extern NCURSES_EXPORT(int) in_wchnstr (cchar_t *, int);			/* generated:WIDEC */
extern NCURSES_EXPORT(int) in_wchstr (cchar_t *);			/* generated:WIDEC */
extern NCURSES_EXPORT(int) innwstr (wchar_t *, int);			/* generated:WIDEC */
extern NCURSES_EXPORT(int) ins_nwstr (const wchar_t *, int);		/* generated:WIDEC */
extern NCURSES_EXPORT(int) ins_wch (const cchar_t *);			/* generated:WIDEC */
extern NCURSES_EXPORT(int) ins_wstr (const wchar_t *);			/* generated:WIDEC */
extern NCURSES_EXPORT(int) inwstr (wchar_t *);				/* generated:WIDEC */
extern NCURSES_EXPORT(NCURSES_CONST char*) key_name (wchar_t);		/* implemented */
extern NCURSES_EXPORT(int) killwchar (wchar_t *);			/* implemented */
extern NCURSES_EXPORT(int) mvadd_wch (int, int, const cchar_t *);	/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvadd_wchnstr (int, int, const cchar_t *, int);/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvadd_wchstr (int, int, const cchar_t *);	/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvaddnwstr (int, int, const wchar_t *, int);	/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvaddwstr (int, int, const wchar_t *);	/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvget_wch (int, int, wint_t *);		/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvget_wstr (int, int, wint_t *);		/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvgetn_wstr (int, int, wint_t *, int);	/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvhline_set (int, int, const cchar_t *, int);	/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvin_wch (int, int, cchar_t *);		/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvin_wchnstr (int, int, cchar_t *, int);	/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvin_wchstr (int, int, cchar_t *);		/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvinnwstr (int, int, wchar_t *, int);	/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvins_nwstr (int, int, const wchar_t *, int);	/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvins_wch (int, int, const cchar_t *);	/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvins_wstr (int, int, const wchar_t *);	/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvinwstr (int, int, wchar_t *);		/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvvline_set (int, int, const cchar_t *, int);	/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvwadd_wch (WINDOW *, int, int, const cchar_t *);	/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvwadd_wchnstr (WINDOW *, int, int, const cchar_t *, int); /* generated:WIDEC */
extern NCURSES_EXPORT(int) mvwadd_wchstr (WINDOW *, int, int, const cchar_t *);	/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvwaddnwstr (WINDOW *, int, int, const wchar_t *, int);/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvwaddwstr (WINDOW *, int, int, const wchar_t *);	/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvwget_wch (WINDOW *, int, int, wint_t *);	/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvwget_wstr (WINDOW *, int, int, wint_t *);	/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvwgetn_wstr (WINDOW *, int, int, wint_t *, int);/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvwhline_set (WINDOW *, int, int, const cchar_t *, int);/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvwin_wch (WINDOW *, int, int, cchar_t *);	/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvwin_wchnstr (WINDOW *, int,int, cchar_t *,int);	/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvwin_wchstr (WINDOW *, int, int, cchar_t *);	/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvwinnwstr (WINDOW *, int, int, wchar_t *, int);	/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvwins_nwstr (WINDOW *, int,int, const wchar_t *,int); /* generated:WIDEC */
extern NCURSES_EXPORT(int) mvwins_wch (WINDOW *, int, int, const cchar_t *);	/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvwins_wstr (WINDOW *, int, int, const wchar_t *);	/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvwinwstr (WINDOW *, int, int, wchar_t *);		/* generated:WIDEC */
extern NCURSES_EXPORT(int) mvwvline_set (WINDOW *, int,int, const cchar_t *,int); /* generated:WIDEC */
extern NCURSES_EXPORT(int) pecho_wchar (WINDOW *, const cchar_t *);	/* implemented */
extern NCURSES_EXPORT(int) setcchar (cchar_t *, const wchar_t *, const attr_t, NCURSES_PAIRS_T, const void *);	/* implemented */
extern NCURSES_EXPORT(int) slk_wset (int, const wchar_t *, int);	/* implemented */
extern NCURSES_EXPORT(attr_t) term_attrs (void);			/* implemented */
extern NCURSES_EXPORT(int) unget_wch (const wchar_t);			/* implemented */
extern NCURSES_EXPORT(int) vid_attr (attr_t, NCURSES_PAIRS_T, void *);		/* implemented */
extern NCURSES_EXPORT(int) vid_puts (attr_t, NCURSES_PAIRS_T, void *, NCURSES_OUTC); /* implemented */
extern NCURSES_EXPORT(int) vline_set (const cchar_t *, int);		/* generated:WIDEC */
extern NCURSES_EXPORT(int) wadd_wch (WINDOW *,const cchar_t *);		/* implemented */
extern NCURSES_EXPORT(int) wadd_wchnstr (WINDOW *,const cchar_t *,int);	/* implemented */
extern NCURSES_EXPORT(int) wadd_wchstr (WINDOW *,const cchar_t *);	/* generated:WIDEC */
extern NCURSES_EXPORT(int) waddnwstr (WINDOW *,const wchar_t *,int);	/* implemented */
extern NCURSES_EXPORT(int) waddwstr (WINDOW *,const wchar_t *);		/* generated:WIDEC */
extern NCURSES_EXPORT(int) wbkgrnd (WINDOW *,const cchar_t *);		/* implemented */
extern NCURSES_EXPORT(void) wbkgrndset (WINDOW *,const cchar_t *);	/* implemented */
extern NCURSES_EXPORT(int) wborder_set (WINDOW *,const cchar_t*,const cchar_t*,const cchar_t*,const cchar_t*,const cchar_t*,const cchar_t*,const cchar_t*,const cchar_t*);	/* implemented */
extern NCURSES_EXPORT(int) wecho_wchar (WINDOW *, const cchar_t *);	/* implemented */
extern NCURSES_EXPORT(int) wget_wch (WINDOW *, wint_t *);		/* implemented */
extern NCURSES_EXPORT(int) wget_wstr (WINDOW *, wint_t *);		/* generated:WIDEC */
extern NCURSES_EXPORT(int) wgetbkgrnd (WINDOW *, cchar_t *);		/* generated:WIDEC */
extern NCURSES_EXPORT(int) wgetn_wstr (WINDOW *, wint_t *, int);	/* implemented */
extern NCURSES_EXPORT(int) whline_set (WINDOW *, const cchar_t *, int);	/* implemented */
extern NCURSES_EXPORT(int) win_wch (WINDOW *, cchar_t *);		/* implemented */
extern NCURSES_EXPORT(int) win_wchnstr (WINDOW *, cchar_t *, int);	/* implemented */
extern NCURSES_EXPORT(int) win_wchstr (WINDOW *, cchar_t *);		/* generated:WIDEC */
extern NCURSES_EXPORT(int) winnwstr (WINDOW *, wchar_t *, int);		/* implemented */
extern NCURSES_EXPORT(int) wins_nwstr (WINDOW *, const wchar_t *, int);	/* implemented */
extern NCURSES_EXPORT(int) wins_wch (WINDOW *, const cchar_t *);	/* implemented */
extern NCURSES_EXPORT(int) wins_wstr (WINDOW *, const wchar_t *);	/* generated:WIDEC */
extern NCURSES_EXPORT(int) winwstr (WINDOW *, wchar_t *);		/* implemented */
extern NCURSES_EXPORT(wchar_t*) wunctrl (cchar_t *);			/* implemented */
extern NCURSES_EXPORT(int) wvline_set (WINDOW *, const cchar_t *, int);	/* implemented */

#if NCURSES_SP_FUNCS
extern NCURSES_EXPORT(attr_t) NCURSES_SP_NAME(term_attrs) (SCREEN*);		/* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(erasewchar) (SCREEN*, wchar_t *);	/* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(killwchar) (SCREEN*, wchar_t *);	/* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(unget_wch) (SCREEN*, const wchar_t);	/* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(vid_attr) (SCREEN*, attr_t, NCURSES_PAIRS_T, void *);	/* implemented:SP_FUNC */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(vid_puts) (SCREEN*, attr_t, NCURSES_PAIRS_T, void *, NCURSES_SP_OUTC);	/* implemented:SP_FUNC */
extern NCURSES_EXPORT(wchar_t*) NCURSES_SP_NAME(wunctrl) (SCREEN*, cchar_t *);	/* implemented:SP_FUNC */
#endif

#ifndef NCURSES_NOMACROS

/*
 * XSI curses macros for XPG4 conformance.
 */
#define add_wch(c)			wadd_wch(stdscr,(c))
#define add_wchnstr(str,n)		wadd_wchnstr(stdscr,(str),(n))
#define add_wchstr(str)			wadd_wchstr(stdscr,(str))
#define addnwstr(wstr,n)		waddnwstr(stdscr,(wstr),(n))
#define addwstr(wstr)			waddwstr(stdscr,(wstr))
#define bkgrnd(c)			wbkgrnd(stdscr,(c))
#define bkgrndset(c)			wbkgrndset(stdscr,(c))
#define border_set(l,r,t,b,tl,tr,bl,br) wborder_set(stdscr,(l),(r),(t),(b),tl,tr,bl,br)
#define box_set(w,v,h)			wborder_set((w),(v),(v),(h),(h),0,0,0,0)
#define echo_wchar(c)			wecho_wchar(stdscr,(c))
#define get_wch(c)			wget_wch(stdscr,(c))
#define get_wstr(t)			wget_wstr(stdscr,(t))
#define getbkgrnd(wch)			wgetbkgrnd(stdscr,(wch))
#define getn_wstr(t,n)			wgetn_wstr(stdscr,(t),(n))
#define hline_set(c,n)			whline_set(stdscr,(c),(n))
#define in_wch(c)			win_wch(stdscr,(c))
#define in_wchnstr(c,n)			win_wchnstr(stdscr,(c),(n))
#define in_wchstr(c)			win_wchstr(stdscr,(c))
#define innwstr(c,n)			winnwstr(stdscr,(c),(n))
#define ins_nwstr(t,n)			wins_nwstr(stdscr,(t),(n))
#define ins_wch(c)			wins_wch(stdscr,(c))
#define ins_wstr(t)			wins_wstr(stdscr,(t))
#define inwstr(c)			winwstr(stdscr,(c))
#define vline_set(c,n)			wvline_set(stdscr,(c),(n))
#define wadd_wchstr(win,str)		wadd_wchnstr((win),(str),-1)
#define waddwstr(win,wstr)		waddnwstr((win),(wstr),-1)
#define wget_wstr(w,t)			wgetn_wstr((w),(t),-1)
#define win_wchstr(w,c)			win_wchnstr((w),(c),-1)
#define wins_wstr(w,t)			wins_nwstr((w),(t),-1)

#if !NCURSES_OPAQUE
#define wgetbkgrnd(win,wch)		(NCURSES_OK_ADDR(wch) ? ((win) ? (*(wch) = (win)->_bkgrnd) : *(wch), OK) : ERR)
#endif

#define mvadd_wch(y,x,c)		mvwadd_wch(stdscr,(y),(x),(c))
#define mvadd_wchnstr(y,x,s,n)		mvwadd_wchnstr(stdscr,(y),(x),(s),(n))
#define mvadd_wchstr(y,x,s)		mvwadd_wchstr(stdscr,(y),(x),(s))
#define mvaddnwstr(y,x,wstr,n)		mvwaddnwstr(stdscr,(y),(x),(wstr),(n))
#define mvaddwstr(y,x,wstr)		mvwaddwstr(stdscr,(y),(x),(wstr))
#define mvget_wch(y,x,c)		mvwget_wch(stdscr,(y),(x),(c))
#define mvget_wstr(y,x,t)		mvwget_wstr(stdscr,(y),(x),(t))
#define mvgetn_wstr(y,x,t,n)		mvwgetn_wstr(stdscr,(y),(x),(t),(n))
#define mvhline_set(y,x,c,n)		mvwhline_set(stdscr,(y),(x),(c),(n))
#define mvin_wch(y,x,c)			mvwin_wch(stdscr,(y),(x),(c))
#define mvin_wchnstr(y,x,c,n)		mvwin_wchnstr(stdscr,(y),(x),(c),(n))
#define mvin_wchstr(y,x,c)		mvwin_wchstr(stdscr,(y),(x),(c))
#define mvinnwstr(y,x,c,n)		mvwinnwstr(stdscr,(y),(x),(c),(n))
#define mvins_nwstr(y,x,t,n)		mvwins_nwstr(stdscr,(y),(x),(t),(n))
#define mvins_wch(y,x,c)		mvwins_wch(stdscr,(y),(x),(c))
#define mvins_wstr(y,x,t)		mvwins_wstr(stdscr,(y),(x),(t))
#define mvinwstr(y,x,c)			mvwinwstr(stdscr,(y),(x),(c))
#define mvvline_set(y,x,c,n)		mvwvline_set(stdscr,(y),(x),(c),(n))

#define mvwadd_wch(win,y,x,c)		(wmove(win,(y),(x)) == ERR ? ERR : wadd_wch((win),(c)))
#define mvwadd_wchnstr(win,y,x,s,n)	(wmove(win,(y),(x)) == ERR ? ERR : wadd_wchnstr((win),(s),(n)))
#define mvwadd_wchstr(win,y,x,s)	(wmove(win,(y),(x)) == ERR ? ERR : wadd_wchstr((win),(s)))
#define mvwaddnwstr(win,y,x,wstr,n)	(wmove(win,(y),(x)) == ERR ? ERR : waddnwstr((win),(wstr),(n)))
#define mvwaddwstr(win,y,x,wstr)	(wmove(win,(y),(x)) == ERR ? ERR : waddwstr((win),(wstr)))
#define mvwget_wch(win,y,x,c)		(wmove(win,(y),(x)) == ERR ? ERR : wget_wch((win),(c)))
#define mvwget_wstr(win,y,x,t)		(wmove(win,(y),(x)) == ERR ? ERR : wget_wstr((win),(t)))
#define mvwgetn_wstr(win,y,x,t,n)	(wmove(win,(y),(x)) == ERR ? ERR : wgetn_wstr((win),(t),(n)))
#define mvwhline_set(win,y,x,c,n)	(wmove(win,(y),(x)) == ERR ? ERR : whline_set((win),(c),(n)))
#define mvwin_wch(win,y,x,c)		(wmove(win,(y),(x)) == ERR ? ERR : win_wch((win),(c)))
#define mvwin_wchnstr(win,y,x,c,n)	(wmove(win,(y),(x)) == ERR ? ERR : win_wchnstr((win),(c),(n)))
#define mvwin_wchstr(win,y,x,c)		(wmove(win,(y),(x)) == ERR ? ERR : win_wchstr((win),(c)))
#define mvwinnwstr(win,y,x,c,n)		(wmove(win,(y),(x)) == ERR ? ERR : winnwstr((win),(c),(n)))
#define mvwins_nwstr(win,y,x,t,n)	(wmove(win,(y),(x)) == ERR ? ERR : wins_nwstr((win),(t),(n)))
#define mvwins_wch(win,y,x,c)		(wmove(win,(y),(x)) == ERR ? ERR : wins_wch((win),(c)))
#define mvwins_wstr(win,y,x,t)		(wmove(win,(y),(x)) == ERR ? ERR : wins_wstr((win),(t)))
#define mvwinwstr(win,y,x,c)		(wmove(win,(y),(x)) == ERR ? ERR : winwstr((win),(c)))
#define mvwvline_set(win,y,x,c,n)	(wmove(win,(y),(x)) == ERR ? ERR : wvline_set((win),(c),(n)))

#endif /* NCURSES_NOMACROS */

#if defined(TRACE) || defined(NCURSES_TEST)
extern NCURSES_EXPORT(const char *) _nc_viswbuf(const wchar_t *);
extern NCURSES_EXPORT(const char *) _nc_viswibuf(const wint_t *);
#endif

#endif /* NCURSES_WIDECHAR */
/* $Id: curses.tail,v 1.27 2023/08/05 19:43:46 tom Exp $ */
/*
 * vile:cmode:
 * This file is part of ncurses, designed to be appended after curses.h.in
 * (see that file for the relevant copyright).
 */

/* mouse interface */

#if NCURSES_MOUSE_VERSION > 1
#define NCURSES_MOUSE_MASK(b,m) ((m) << (((b) - 1) * 5))
#else
#define NCURSES_MOUSE_MASK(b,m) ((m) << (((b) - 1) * 6))
#endif

#define	NCURSES_BUTTON_RELEASED	001UL
#define	NCURSES_BUTTON_PRESSED	002UL
#define	NCURSES_BUTTON_CLICKED	004UL
#define	NCURSES_DOUBLE_CLICKED	010UL
#define	NCURSES_TRIPLE_CLICKED	020UL
#define	NCURSES_RESERVED_EVENT	040UL

/* event masks */
#define	BUTTON1_RELEASED	NCURSES_MOUSE_MASK(1, NCURSES_BUTTON_RELEASED)
#define	BUTTON1_PRESSED		NCURSES_MOUSE_MASK(1, NCURSES_BUTTON_PRESSED)
#define	BUTTON1_CLICKED		NCURSES_MOUSE_MASK(1, NCURSES_BUTTON_CLICKED)
#define	BUTTON1_DOUBLE_CLICKED	NCURSES_MOUSE_MASK(1, NCURSES_DOUBLE_CLICKED)
#define	BUTTON1_TRIPLE_CLICKED	NCURSES_MOUSE_MASK(1, NCURSES_TRIPLE_CLICKED)

#define	BUTTON2_RELEASED	NCURSES_MOUSE_MASK(2, NCURSES_BUTTON_RELEASED)
#define	BUTTON2_PRESSED		NCURSES_MOUSE_MASK(2, NCURSES_BUTTON_PRESSED)
#define	BUTTON2_CLICKED		NCURSES_MOUSE_MASK(2, NCURSES_BUTTON_CLICKED)
#define	BUTTON2_DOUBLE_CLICKED	NCURSES_MOUSE_MASK(2, NCURSES_DOUBLE_CLICKED)
#define	BUTTON2_TRIPLE_CLICKED	NCURSES_MOUSE_MASK(2, NCURSES_TRIPLE_CLICKED)

#define	BUTTON3_RELEASED	NCURSES_MOUSE_MASK(3, NCURSES_BUTTON_RELEASED)
#define	BUTTON3_PRESSED		NCURSES_MOUSE_MASK(3, NCURSES_BUTTON_PRESSED)
#define	BUTTON3_CLICKED		NCURSES_MOUSE_MASK(3, NCURSES_BUTTON_CLICKED)
#define	BUTTON3_DOUBLE_CLICKED	NCURSES_MOUSE_MASK(3, NCURSES_DOUBLE_CLICKED)
#define	BUTTON3_TRIPLE_CLICKED	NCURSES_MOUSE_MASK(3, NCURSES_TRIPLE_CLICKED)

#define	BUTTON4_RELEASED	NCURSES_MOUSE_MASK(4, NCURSES_BUTTON_RELEASED)
#define	BUTTON4_PRESSED		NCURSES_MOUSE_MASK(4, NCURSES_BUTTON_PRESSED)
#define	BUTTON4_CLICKED		NCURSES_MOUSE_MASK(4, NCURSES_BUTTON_CLICKED)
#define	BUTTON4_DOUBLE_CLICKED	NCURSES_MOUSE_MASK(4, NCURSES_DOUBLE_CLICKED)
#define	BUTTON4_TRIPLE_CLICKED	NCURSES_MOUSE_MASK(4, NCURSES_TRIPLE_CLICKED)

/*
 * In 32 bits the version-1 scheme does not provide enough space for a 5th
 * button, unless we choose to change the ABI by omitting the reserved-events.
 */
#if NCURSES_MOUSE_VERSION > 1

#define	BUTTON5_RELEASED	NCURSES_MOUSE_MASK(5, NCURSES_BUTTON_RELEASED)
#define	BUTTON5_PRESSED		NCURSES_MOUSE_MASK(5, NCURSES_BUTTON_PRESSED)
#define	BUTTON5_CLICKED		NCURSES_MOUSE_MASK(5, NCURSES_BUTTON_CLICKED)
#define	BUTTON5_DOUBLE_CLICKED	NCURSES_MOUSE_MASK(5, NCURSES_DOUBLE_CLICKED)
#define	BUTTON5_TRIPLE_CLICKED	NCURSES_MOUSE_MASK(5, NCURSES_TRIPLE_CLICKED)

#define	BUTTON_CTRL		NCURSES_MOUSE_MASK(6, 0001L)
#define	BUTTON_SHIFT		NCURSES_MOUSE_MASK(6, 0002L)
#define	BUTTON_ALT		NCURSES_MOUSE_MASK(6, 0004L)
#define	REPORT_MOUSE_POSITION	NCURSES_MOUSE_MASK(6, 0010L)

#else

#define	BUTTON1_RESERVED_EVENT	NCURSES_MOUSE_MASK(1, NCURSES_RESERVED_EVENT)
#define	BUTTON2_RESERVED_EVENT	NCURSES_MOUSE_MASK(2, NCURSES_RESERVED_EVENT)
#define	BUTTON3_RESERVED_EVENT	NCURSES_MOUSE_MASK(3, NCURSES_RESERVED_EVENT)
#define	BUTTON4_RESERVED_EVENT	NCURSES_MOUSE_MASK(4, NCURSES_RESERVED_EVENT)

#define	BUTTON_CTRL		NCURSES_MOUSE_MASK(5, 0001L)
#define	BUTTON_SHIFT		NCURSES_MOUSE_MASK(5, 0002L)
#define	BUTTON_ALT		NCURSES_MOUSE_MASK(5, 0004L)
#define	REPORT_MOUSE_POSITION	NCURSES_MOUSE_MASK(5, 0010L)

#endif

#define	ALL_MOUSE_EVENTS	(REPORT_MOUSE_POSITION - 1)

/* macros to extract single event-bits from masks */
#define	BUTTON_RELEASE(e, x)		((e) & NCURSES_MOUSE_MASK(x, 001))
#define	BUTTON_PRESS(e, x)		((e) & NCURSES_MOUSE_MASK(x, 002))
#define	BUTTON_CLICK(e, x)		((e) & NCURSES_MOUSE_MASK(x, 004))
#define	BUTTON_DOUBLE_CLICK(e, x)	((e) & NCURSES_MOUSE_MASK(x, 010))
#define	BUTTON_TRIPLE_CLICK(e, x)	((e) & NCURSES_MOUSE_MASK(x, 020))
#define	BUTTON_RESERVED_EVENT(e, x)	((e) & NCURSES_MOUSE_MASK(x, 040))

typedef struct
{
    short id;		/* ID to distinguish multiple devices */
    int x, y, z;	/* event coordinates (character-cell) */
    mmask_t bstate;	/* button state bits */
}
MEVENT;

extern NCURSES_EXPORT(bool)    has_mouse(void);
extern NCURSES_EXPORT(int)     getmouse (MEVENT *);
extern NCURSES_EXPORT(int)     ungetmouse (MEVENT *);
extern NCURSES_EXPORT(mmask_t) mousemask (mmask_t, mmask_t *);
extern NCURSES_EXPORT(bool)    wenclose (const WINDOW *, int, int);
extern NCURSES_EXPORT(int)     mouseinterval (int);
extern NCURSES_EXPORT(bool)    wmouse_trafo (const WINDOW*, int*, int*, bool);
extern NCURSES_EXPORT(bool)    mouse_trafo (int*, int*, bool);              /* generated */

#if NCURSES_SP_FUNCS
extern NCURSES_EXPORT(bool)    NCURSES_SP_NAME(has_mouse) (SCREEN*);
extern NCURSES_EXPORT(int)     NCURSES_SP_NAME(getmouse) (SCREEN*, MEVENT *);
extern NCURSES_EXPORT(int)     NCURSES_SP_NAME(ungetmouse) (SCREEN*,MEVENT *);
extern NCURSES_EXPORT(mmask_t) NCURSES_SP_NAME(mousemask) (SCREEN*, mmask_t, mmask_t *);
extern NCURSES_EXPORT(int)     NCURSES_SP_NAME(mouseinterval) (SCREEN*, int);
#endif

#ifndef NCURSES_NOMACROS
#define mouse_trafo(y,x,to_screen) wmouse_trafo(stdscr,y,x,to_screen)
#endif

/* other non-XSI functions */

extern NCURSES_EXPORT(int) mcprint (char *, int);	/* direct data to printer */
extern NCURSES_EXPORT(int) has_key (int);		/* do we have given key? */

#if NCURSES_SP_FUNCS
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(has_key) (SCREEN*, int);    /* do we have given key? */
extern NCURSES_EXPORT(int) NCURSES_SP_NAME(mcprint) (SCREEN*, char *, int);	/* direct data to printer */
#endif

/* Debugging : use with libncurses_g.a */

extern NCURSES_EXPORT(void) _tracef (const char *, ...) GCC_PRINTFLIKE(1,2);
extern NCURSES_EXPORT(char *) _traceattr (attr_t);
extern NCURSES_EXPORT(char *) _traceattr2 (int, chtype);
extern NCURSES_EXPORT(char *) _tracechar (int);
extern NCURSES_EXPORT(char *) _tracechtype (chtype);
extern NCURSES_EXPORT(char *) _tracechtype2 (int, chtype);
#if NCURSES_WIDECHAR
#define _tracech_t		_tracecchar_t
extern NCURSES_EXPORT(char *) _tracecchar_t (const cchar_t *);
#define _tracech_t2		_tracecchar_t2
extern NCURSES_EXPORT(char *) _tracecchar_t2 (int, const cchar_t *);
#else
#define _tracech_t		_tracechtype
#define _tracech_t2		_tracechtype2
#endif
extern NCURSES_EXPORT(void) trace (const unsigned) GCC_DEPRECATED("use curses_trace");
extern NCURSES_EXPORT(unsigned) curses_trace (const unsigned);

/* trace masks */
#define TRACE_DISABLE	0x0000	/* turn off tracing */
#define TRACE_TIMES	0x0001	/* trace user and system times of updates */
#define TRACE_TPUTS	0x0002	/* trace tputs calls */
#define TRACE_UPDATE	0x0004	/* trace update actions, old & new screens */
#define TRACE_MOVE	0x0008	/* trace cursor moves and scrolls */
#define TRACE_CHARPUT	0x0010	/* trace all character outputs */
#define TRACE_ORDINARY	0x001F	/* trace all update actions */
#define TRACE_CALLS	0x0020	/* trace all curses calls */
#define TRACE_VIRTPUT	0x0040	/* trace virtual character puts */
#define TRACE_IEVENT	0x0080	/* trace low-level input processing */
#define TRACE_BITS	0x0100	/* trace state of TTY control bits */
#define TRACE_ICALLS	0x0200	/* trace internal/nested calls */
#define TRACE_CCALLS	0x0400	/* trace per-character calls */
#define TRACE_DATABASE	0x0800	/* trace read/write of terminfo/termcap data */
#define TRACE_ATTRS	0x1000	/* trace attribute updates */

#define TRACE_SHIFT	13	/* number of bits in the trace masks */
#define TRACE_MAXIMUM	((1 << TRACE_SHIFT) - 1) /* maximum trace level */

#if defined(TRACE) || defined(NCURSES_TEST)
extern NCURSES_EXPORT_VAR(int) _nc_optimize_enable;		/* enable optimizations */
extern NCURSES_EXPORT(const char *) _nc_visbuf (const char *);
#define OPTIMIZE_MVCUR		0x01	/* cursor movement optimization */
#define OPTIMIZE_HASHMAP	0x02	/* diff hashing to detect scrolls */
#define OPTIMIZE_SCROLL		0x04	/* scroll optimization */
#define OPTIMIZE_ALL		0xff	/* enable all optimizations (dflt) */
#endif

extern GCC_NORETURN NCURSES_EXPORT(void) exit_curses (int);

#include <unctrl.h>

#ifdef __cplusplus

#ifndef NCURSES_NOMACROS

/* these names conflict with STL */
#undef box
#undef clear
#undef erase
#undef move
#undef refresh

#endif /* NCURSES_NOMACROS */

}
#endif


#define PTHREAD_CREATE_OK (0)
#define PTHREAD_JOIN_OK (0)
#define SENDTO_MIN_OK (0)
#define REALLOC_ERR (NULL)
#define FXN_SOCKET_ERR (-1)
#define NEWBUF_ERR (NULL)
#define NEW_ERR (NULL)

// <locale.h>
typedef struct lconv {
    char *decimal_point;
    char *thousands_sep;
    char *grouping;
    char *mon_decimal_point;
    char *mon_thousands_sep;
    char *mon_grouping;
    char *positive_sign;
    char *negative_sign;
    char *currency_symbol;
    char frac_digits;
    char p_cs_precedes;
    char n_cs_precedes;
    char p_sep_by_space;
    char n_sep_by_space;
    char p_sign_posn;
    char n_sign_posn;
    char *int_curr_symbol;
    char int_frac_digits;
    char int_p_cs_precedes;
    char int_n_cs_precedes;
    char int_p_sep_by_space;
    char int_n_sep_by_space;
    char int_p_sign_posn;
    char int_n_sign_posn;
} lconv;
char *setlocale(int category, const char *locale);
struct lconv *localeconv(void);
#define LC_ALL 0
#define LC_COLLATE 1
#define LC_CTYPE 2
#define LC_MONETARY 3
#define LC_NUMERIC 4
#define LC_TIME 5
#define LC_MESSAGES 6

const char * format_pps( char * buf , size_t buflen , ubigint pps , int number_of_float /*=2*/ , const char * unit_name /*= "pps"*/);

struct PacketQueue
{
	char data[ QUEUE_CAPACITY ][ PACKET_SIZE ];
	size_t lengths[ QUEUE_CAPACITY ];
	int head;
	int tail;
	int count;

	pthread_mutex_t lock;
	pthread_cond_t not_empty;
	pthread_cond_t not_full;
};

void queue_destroy( PacketQueue * q );
void queue_push( PacketQueue * q , const char * buf , size_t len );
int queue_pop( PacketQueue * q , char * out_buf , size_t * out_len );
int queue_peek_available( PacketQueue * q );
int queue_try_pop( PacketQueue * q , char * out_buf , size_t * out_len );

#define _THREAD_FXN /*identified as thread callback fxn*/

#endif // ( defined(INTELISENSE_BUILD) && (INTELISENSE_BUILD + 0) ) || defined(__FORCE_INTELLISENSE_ASSIST)
