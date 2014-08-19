/* cat from plan9 */
/* Adapted for SleepyLibC by odc on 2014/04/25 */

#include "sleepy.h"

void
sysfatal(const char *s1, char *s2, int errno)
{
	fputs(s1, stderr);
	fputs(s2, stderr);
	fputs(" error ", stderr);
	fputn(errno, stderr);
	fputc('\n', stderr);
	exit(1);
}

void
cat(int fd, char *s)
{
	char buf[8192];
	ssize_t n;

	while((n = read(fd, buf, sizeof buf)) > 0)
		if (write(STDOUT, buf, n) != n)
			sysfatal("write error copying: ", s, n);
	if(n < 0)
		sysfatal("error reading: ", s, n);
}

int
main(int argc, char *argv[])
{
	int f, i;

	if(argc == 1)
		cat(0, "<stdin>");
	else for(i=1; i<argc; i++){
		f = open(argv[i], O_RDONLY, 0);
		if(f < 0)
			sysfatal("can't open: ", argv[i], f);
		else{
			cat(f, argv[i]);
			close(f);
		}
	}

	return 0;
}
