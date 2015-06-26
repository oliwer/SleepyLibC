/*
 * SleepyLibc for Linux x86_64
 */

#include <stdarg.h>

#ifndef SLEEPYC
#define SLEEPYC


#define CHAR_MAX    127
#define CHAR_MIN    (-128)
#define UCHAR_MAX   255

#define SHRT_MAX    32767
#define SHRT_MIN    (-32768)
#define USHRT_MAX   65535

#define INT_MAX     2147483647
#define INT_MIN     (-INT_MAX - 1)
#define UINT_MAX    4294967295U

#define LONG_MAX    9223372036854775807L
#define LONG_MIN    (-LONG_MAX - 1L)
#define ULONG_MAX   18446744073709551615UL

#define SIZE_MAX    ULONG_MAX
#define SSIZE_MAX   LONG_MAX

#define WORD_SIZE   (sizeof(void*))

#define NULL        ((void *)0L)
#define EOF         (-1)

typedef unsigned long int    size_t;
typedef signed long int      intptr_t;
typedef signed long int      ssize_t;
typedef unsigned int         mode_t;

/* Standard file descriptors */
#define STDIN     0
#define STDOUT    1
#define STDERR    2

/* klibc style FILE */
typedef int          FILE;
#define fileno(F)    ((int)(intptr_t)(F) - 1)
#define MKFILE(fd)   ((FILE *)((fd) + 1))

#define stdin    MKFILE(STDIN)
#define stdout   MKFILE(STDOUT)
#define stderr   MKFILE(STDERR)


/* Cross-platform errno */
#define EPERM            1      /* Operation not permitted */
#define ENOENT           2      /* No such file or directory */
#define ESRCH            3      /* No such process */
#define EINTR            4      /* Interrupted system call */
#define EIO              5      /* I/O error */
#define ENXIO            6      /* No such device or address */
#define E2BIG            7      /* Argument list too long */
#define ENOEXEC          8      /* Exec format error */
#define EBADF            9      /* Bad file number */
#define ECHILD          10      /* No child processes */
#define EAGAIN          11      /* Try again */
#define ENOMEM          12      /* Out of memory */
#define EACCES          13      /* Permission denied */
#define EFAULT          14      /* Bad address */
#define ENOTBLK         15      /* Block device required */
#define EBUSY           16      /* Device or resource busy */
#define EEXIST          17      /* File exists */
#define EXDEV           18      /* Cross-device link */
#define ENODEV          19      /* No such device */
#define ENOTDIR         20      /* Not a directory */
#define EISDIR          21      /* Is a directory */
#define EINVAL          22      /* Invalid argument */
#define ENFILE          23      /* File table overflow */
#define EMFILE          24      /* Too many open files */
#define ENOTTY          25      /* Not a typewriter */
#define ETXTBSY         26      /* Text file busy */
#define EFBIG           27      /* File too large */
#define ENOSPC          28      /* No space left on device */
#define ESPIPE          29      /* Illegal seek */
#define EROFS           30      /* Read-only file system */
#define EMLINK          31      /* Too many links */
#define EPIPE           32      /* Broken pipe */
#define EDOM            33      /* Math argument out of domain of func */
#define ERANGE          34      /* Math result not representable */

/* Sleepy Error codes */
#define ESTRLONG  999

/* File access flags */
#define O_RDONLY  00
#define O_WRONLY  01
#define O_RDWR    02

#define O_CREAT        0100
#define O_EXCL         0200
#define O_NOCTTY       0400
#define O_TRUNC       01000
#define O_APPEND      02000
#define O_NONBLOCK    04000
#define O_DSYNC      010000
#define O_SYNC     04010000
#define O_RSYNC    04010000
#define O_DIRECTORY 0200000
#define O_NOFOLLOW  0400000
#define O_CLOEXEC  02000000

#define O_ASYNC      020000
#define O_DIRECT     040000
#define O_LARGEFILE       0
#define O_NOATIME  01000000
#define O_TMPFILE 020200000
#define O_NDELAY O_NONBLOCK

/* File creation modes */
#define S_ISUID 04000
#define S_ISGID 02000
#define S_ISVTX 01000
#define S_IRUSR 0400
#define S_IWUSR 0200
#define S_IXUSR 0100
#define S_IRWXU 0700
#define S_IRGRP 0040
#define S_IWGRP 0020
#define S_IXGRP 0010
#define S_IRWXG 0070
#define S_IROTH 0004
#define S_IWOTH 0002
#define S_IXOTH 0001
#define S_IRWXO 0007

/* Offset seeking */
#define SEEK_SET   0
#define SEEK_CUR   1
#define SEEK_END   2

/* unistd.h */
int     close(int);
int     fsync(int);
int     getopt(int, char * const *, const char *);
int     open(const char *, int, mode_t);
ssize_t read(int, void *, size_t);
int     unlink(const char *path);
ssize_t write(int, const void *, size_t);

/* string.h */
int     memcmp(const void *, const void *, size_t);
void   *memset(void *, int, size_t);
#define putc(c,s)   (fputc(c, s))
#define putchar(c)  (fputc(c, stdout))
char   *strchr(const char *, int);
int     strcmp(const char *, const char *);
size_t  strlcpy(char *, const char *, size_t);
size_t  strlen(const char *);
int     strncmp(const char *, const char *, size_t);

/* stdio.h */
int     fprintf(FILE *, const char *, ...);
int     fputc(int, FILE *);
int     fputs(const char *, FILE *);
#define printf(...)  (fprintf(stdout, __VA_ARGS__);
int     puts(const char *);
int     snprintf(char *, size_t, const char *, ...);
#define sprintf(dest, ...)  (snprintf(dest, SIZE_MAX, __VA_ARGS__))
int     vfprintf(FILE *, const char *, va_list);
#define vprintf(fmt,ap)  (vfprintf(stdout, fmt, ap))
#define vsprintf(str,fmt,ap)  (vsnprintf(str, SIZE_MAX, fmt, ap))
int     vsnprintf(char *, size_t, const char *, va_list);

char   *ltoa(long, char *, size_t);

/* stdlib.h */
int     abs(int);
long    labs(long);
void    exit(int);
char   *getenv(const char *);

/* ctype.h */
#define isalpha(a) ((((unsigned)(a)|32)-'a') < 26)
#define isdigit(a) (((unsigned)(a)-'0') < 10)
#define islower(a) (((unsigned)(a)-'a') < 26)
#define isupper(a) (((unsigned)(a)-'A') < 26)
#define isprint(a) (((unsigned)(a)-0x20) < 0x5f)
#define isgraph(a) (((unsigned)(a)-0x21) < 0x5e)
#define tolower(a) ((a)|0x20)
#define toupper(a) ((a)&0x5f)

/* Helpers */

#define MIN(a,b)      ((a) > (b) ? (b) : (a))
#define MAX(a,b)      ((a) > (b) ? (a) : (b))
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#define bzero(s,n)    (memset(s, '\0', n))

#endif
