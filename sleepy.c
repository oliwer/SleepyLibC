/*
 * SleepyLibC for Linux x86_64
 */

#include "sleepy.h"


/* crt1 */

int main(int argc, char **argv);

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
	argc = main(argc, argv);
	/* exit, argc now hold the return code */
	__syscall1(SYS_exit_group, (long)argc);
}



/* System Calls */

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
	fputs(argv[optind], stderr);
	fputs(": unknown option\n", stderr);
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

int
fputn(long num, FILE *stream)
{
	char buf[32];  /* More than enough */

	bzero(buf, sizeof buf);
	return fputs(ltoa(num, buf, sizeof buf), stream);
}

/*
 * Supported formats: %c %d %s %li %lu
 */
int
vfprintf(FILE *stream, const char *fmt, va_list ap)
{
	/* TODO: not finished */
	char *pos, *lastpos = (char *)fmt;
	ssize_t written = 0;
	size_t len;
	char cval;
	long lval;

	/* read the format */
	while ((pos = strchr(lastpos, '%'))) {
		/* print the text before the '%', if any */
		len = pos - lastpos;
		if (len) {
			written += write(fileno(stream), lastpos, len);
		}

		pos++;  /* move to the first char of the type */

		switch(*pos) {
		case 'c':
			cval = va_arg(ap, int);
			written += write(fileno(stream), &cval, sizeof cval);
			break;
		case 'd':
			lval = (long)va_arg(ap, int);
			written += fputn(lval, stream);
			break;
		case 's':
			lval = (long)va_arg(ap, char *);
			written += fputs((const char *)lval, stream);
			break;
		case 'l':
			pos++;
			switch(*pos) {
			case 'i':
			case 'u':
				lval = va_arg(ap, long);
				written += fputn(lval, stream);
				break;
			}
		default:
			fputs("error :(\n", stderr);
			return -1;
		}
		lastpos = pos+1;
	}

	return written;
}

int
fprintf(FILE *stream, const char *fmt, ...)
{
	int ret;
	va_list ap;

	va_start(ap, fmt);
	ret = vfprintf(stream, fmt, ap);
	va_end(ap);

	return ret;
}
