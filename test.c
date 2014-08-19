#include "sleepy.h"

int main(int argc, char **argv)
{
	int fd;
	ssize_t len;
	int ret;
	(void)argc;
	(void)argv;

	puts("Starting up...");

	fd = open("file.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd <= 0) {
		fputs("open failed: ", stderr);
		fputn(fd, stderr); NL;
		return 1;
	}

	len = write(fd, "Test Message.", strlen("Test Message."));
	if (len <= 0) {
		fputs("write failed: ", stderr);
		fputn(len, stderr); NL;
		close(fd);
		return 1;
	}

	ret = close(fd);
	if (ret != 0) {
		fputs("close failed: ", stderr);
		fputn(len, stderr); NL;
		return 1;
	}

	puts("Done!");

	return 0;
}


