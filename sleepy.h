/*
 * SleepyLibc for Linux x86_64
 */

#include <stdarg.h>

#ifndef SLEEPYC
#define SLEEPYC

#define SYS_read                0
#define SYS_write               1
#define SYS_open                2
#define SYS_close               3
#define SYS_stat                4
#define SYS_fstat               5
#define SYS_lstat               6
#define SYS_poll                7
#define SYS_lseek               8
#define SYS_exit               60
#define SYS_fsync              74
#define SYS_exit_group        231

static __inline long __syscall0(long n)
{
	long ret;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n) : "rcx", "r11", "memory");
	return ret;
}

static __inline long __syscall1(long n, long a1)
{
	long ret;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1) : "rcx", "r11", "memory");
	return ret;
}

static __inline long __syscall2(long n, long a1, long a2)
{
	long ret;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2)
						  : "rcx", "r11", "memory");
	return ret;
}

static __inline long __syscall3(long n, long a1, long a2, long a3)
{
	long ret;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3) : "rcx", "r11", "memory");
	return ret;
}

static __inline long __syscall4(long n, long a1, long a2, long a3, long a4)
{
	long ret;
	/* long r10 __asm__("r10") = a4; */
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3), "r"(a4): "rcx", "r11", "memory");
	return ret;
}


#define LONG_MAX    9223372036854775807L
#define LONG_MIN    (-LONG_MAX - 1L)

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


void    exit(int);
int     open(const char *, int, mode_t);
int     close(int);
ssize_t read(int, void *, size_t);
ssize_t write(int, const void *, size_t);
int     fsync(int);

size_t  strlen(const char *);
char   *strchr(const char *, int);
int     strcmp(const char *, const char *);
int     memcmp(const void *, const void *, size_t);
void   *memset(void *, int, size_t);
int     fputs(const char *, FILE *);
int     fputc(int, FILE *);
int     puts(const char *);
int     getopt(int, char * const *, const char *);

char   *ltoa(long, char *, size_t);
int     fputn(long, FILE *);

/* Helpers */

#define MIN(a,b)    ((a) > (b) ? (b) : (a))
#define MAX(a,b)    ((a) > (b) ? (a) : (b))
#define bzero(s,n)  (memset(s, '\0', n))
#define putchar(c)  (fputc(c, stdout))
#define putc(c,s)   (fputc(c, s))
#define NL          (putchar('\n'))

#endif
