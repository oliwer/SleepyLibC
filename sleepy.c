/*
 * SleepyLibC for Linux x86_64
 */

#include "sleepy.h"


/* crt1 */

int main(int argc, char **argv);

char **environ;

__asm__("\
.text \n\
.global _start \n\
_start: \n\
	xor %rbp,%rbp \n\
	mov %rsp,%rdi \n\
	andq $-16,%rsp \n\
	call __cstart \n\
");

void __cstart(long *p)
{
	int argc = p[0];
	char **argv = (void *)(p+1);

	environ = argv + argc + 1;
	exit(main(argc, argv));
}



/* System Calls */

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
#define SYS_unlink             87
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


/* unistd.h */

void
exit(int status)
{
	/* We use exit_goup instead of exit to close any
	   thread attached to this process. */
	__syscall1(SYS_exit_group, (long)status);
	for(;;);
}

int
open(const char *path, int flags, mode_t mode)
{
	return __syscall3(SYS_open, (long)path, (long)flags, (long)mode);
}

int
close(int fd)
{
	return __syscall1(SYS_close, (long)fd);
}

ssize_t
read(int fd, void *buf, size_t len)
{
	return __syscall3(SYS_read, (long)fd, (long)buf, (long)len);
}

ssize_t
write(int fd, const void *buf, size_t len)
{
	return __syscall3(SYS_write, (long)fd, (long)buf, (long)len);
}

int
unlink(const char *path)
{
	return __syscall1(SYS_unlink, (long)path);
}

int
fsync(int fd)
{
	return __syscall1(SYS_fsync, (long)fd);
}


/* string.h */

size_t
strlen(const char *string)
{
	const char *s = string;

	if (!s)
		return 0;

	while (*s)
		s++;

	return (s - string);
}

char *
strchr(const char *s, int c)
{
	while (*s) {
		if (*s == (char)c)
			return (char *)s;
		s++;
	}

	return NULL;
}

int
strcmp(const char *s1, const char *s2)
{
	int diff;

	for (diff = 0; *s1 && !diff; s1++, s2++)
		diff = (int)*s1 - (int)*s2;

	return diff;
}

int
strncmp(const char *s1, const char *s2, size_t len)
{
	if (!len)
		return 0;

	for (; len; --len, s1++, s2++) {
		if (*s1 != *s2)
			return *(unsigned char *)s1 - *(unsigned char *)s2;
		if (*s1 == '\0')
			break;
	}

	return 0;
}

int
memcmp(const void *__s1, const void *__s2, size_t len)
{
	char *s1 = (char *)__s1;
	char *s2 = (char *)__s2;
	int diff;

	for (diff = 0; len && !diff; len--)
		diff = (int)*s1++ - (int)*s2++;

	return diff;
}

void *
memset(void *buf, int c, size_t len)
{
	char *s = buf;

	for (s = buf; len; len--, s++)
		*s = (char)c;

	return buf;
}

/* Straight from OpenBSD */
size_t
strlcpy(char *dst, const char *src, size_t dsize)
{
	const char *osrc = src;
	size_t nleft = dsize;

	/* Copy as many bytes as will fit. */
	if (nleft != 0) {
		while (--nleft != 0) {
			if ((*dst++ = *src++) == '\0')
				break;
		}
	}

	/* Not enough room in dst, add NUL and traverse rest of src. */
	if (nleft == 0) {
		if (dsize != 0)
			*dst = '\0';		/* NUL-terminate dst */
		while (*src++)
			;
	}

	return(src - osrc - 1);	/* count does not include NUL */
}


/* stdio.h */

int
fputs(const char *str, FILE *stream)
{
	size_t len, total = 0;
	ssize_t written;

	len = strlen(str);

	if (len == 0)
		return EOF;

	while (len) {
		written = write(fileno(stream), str, len);

		if (written <= 0) {
			if (written == -EINTR)
				continue;
			else
				return EOF;
		}

		str += written;
		total += written;
		len -= written;
	}

	return (int)total;
}

int
fputc(int c, FILE *stream)
{
	unsigned char buf = (unsigned char)c;
	ssize_t written;

	written = write(fileno(stream), &buf, sizeof buf);

	if (written == sizeof buf)
		return (int)buf;

	return EOF;
}

int
puts(const char *str)
{
	int ret;

	ret = fputs(str, stdout);

	if (ret != EOF)
		ret += putchar('\n');

	return ret;
}

/* stdlib.h */

int
abs(int num)
{
	return num < 0 ? -num : num;
}

long
labs(long num)
{
	return num < 0 ? -num : num;
}

char*
getenv(const char *name)
{
	extern char **environ;
	char **ep, *vp;

	if (!name || !*name)
		return NULL;

	for (ep = environ; *ep; ++ep) {
		vp = *ep;

		while (*vp != '=')
			vp++;

		if (!strncmp(name, *ep, vp - *ep))
			return ++vp;
	}

	return NULL;
}


/* unistd.h */

char *optarg;
int optind=1, opterr=1, optopt;

int
getopt(int argc, char * const argv[], const char *optstr)
{
	char *optstrpos;

	if (optind >= argc || !argv[optind] || argv[optind][0] != '-')
		return -1;

	optopt = argv[optind][1];

	if (!optopt || optopt == '-')
		return -1;

	if ((optstrpos = strchr(optstr, optopt))) {
		/* option found! */
		if (*(optstrpos + 1) == ':') {
			optarg = argv[optind+1];
			if (!*optarg) {
				fputs(argv[optind], stderr);
				fputs(": option requires an argument\n", stderr);
				return ':';
			}
		}
		return optopt;
	}

	/* option not found */
	fprintf(stderr, "%s: unknown option\n", argv[optind]);
	return '?';

}


/* Display */

char*
ltoa (long num, char *buf, size_t buf_len)
{
	char const digits[] = "0123456789";
	long shifter = num;
	char *p = buf;

	if (num < 0) {
		*p++ = '-';
		num *= -1;
	}

	/* Move to where representation ends */
	do {
		++p;
		shifter = shifter / 10;
	} while (shifter);

	/* Check if the buffer is big enough */
	if ((size_t)(p - buf) > buf_len) {
		return NULL;
	}

	/* Move backward, adding digits */
	do {
		*--p = digits[num%10];
		num = num / 10;
	} while (num);

	return buf;
}


/*
 * Adapted from libulz
 * Supports all int types but only in base 10, and %s, and %% for an escaped %
 * Returns the number of bytes written to the target. Should behave exactly
 * as posix sprintf except %x also returns hex decimals in uppercase.
 * destsize is treated like in the original. It will leave space for a trailing 0,
 * if NULL is passed as dest it will return the number of bytes it would have
 * written if a sufficiently large buffer was provided.
 */
int
vsnprintf(char* dest, size_t destsize, const char *format, va_list ap)
{
	char *ins, *a, *o = dest;
	char *omax = dest ? dest + destsize - 1 : NULL;
	char cbuf[32];
	long inn;
	int res = 0;
	int lflag;

	for (a = (char*)format; *a; a++) {
		if (*a == '%') {
			++a;
			switch (*a) {
				case '%':
					goto inschar;
				case 's':
					ins = va_arg(ap, char*);
					strmove:
					if (!ins)
						return -1;
					while (*ins) {
						if (o < omax) {
							*o = *ins;
							o++;
							res++;
						}
						else if (!o) {
							res++;
						}
						ins++;
					}
					break;
				case 'c':
					cbuf[0] = (char)va_arg(ap, int);
					cbuf[1] = 0;
					ins = cbuf;
					goto strmove;
				case 'd':
				case 'i':
					inn = va_arg(ap, int);
					nconv:
					ins = ltoa(inn, cbuf, sizeof(cbuf));
					goto strmove;
					break;
				case 'l':
				case 'z':
					lflag = a[0] == 'l';
					if (lflag) {
						if (a[1] == 'l') {
							/* treat l and ll the same */
							a++;
						}
					}
					a++;
					if (lflag) {
						inn = va_arg(ap, unsigned long);
					}
					else {
						inn = va_arg(ap, size_t);
					}
					goto nconv;
				case 'u':
					inn = va_arg(ap, unsigned);
					goto nconv;
				default:
					return -1;
			}
		}
		else {
			inschar:
			if (o < omax) {
				*o = *a;
				o++;
				res++;
			}
			else if (!o) {
				res++;
			}
		}
	}

	if (o)
		*o = '\0';

	return res;
}

int
snprintf(char* dest, size_t destsize, const char* fmt, ...)
{
	va_list ap;
	int result;

	va_start(ap, fmt);
	result = vsnprintf(dest, destsize, fmt, ap);
	va_end(ap);

	return result;
}

int
fprintf(FILE *stream, const char* fmt, ...)
{
	char dest[4096];
	va_list ap;
	int result;

	va_start(ap, fmt);
	result = vsnprintf(dest, sizeof(dest), fmt, ap);
	va_end(ap);

	if (result)
		result = fputs(dest, stream);

	return result;
}

int
vfprintf(FILE *stream, const char *fmt, va_list ap)
{
	char dest[4096];
	int result;

	result = vsnprintf(dest, sizeof(dest), fmt, ap);

	if (result)
		result = fputs(dest, stream);

	return result;
}
