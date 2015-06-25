#include "sleepy.h"

int main(int argc, char **argv)
{
	int fd, ret;
	ssize_t len;
	(void)argc;
	(void)argv;

	puts("Starting up...");

	fd = open("file.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd <= 0) {
		fprintf(stderr, "open failed: %d\n", fd);
		return 1;
	}

	len = write(fd, "Test Message.", strlen("Test Message."));
	if (len <= 0) {
		fprintf(stderr, "write failed: %ld\n", len);
		close(fd);
		return 1;
	}

	ret = close(fd);
	if (ret != 0) {
		fprintf(stderr, "close failed: %d\n", ret);
		return 1;
	}

	ret = unlink("file.txt");
	if (ret != 0) {
		fprintf(stderr, "close failed: %d\n", ret);
		return 1;
	}

	puts("Done!");

	return 0;
}
