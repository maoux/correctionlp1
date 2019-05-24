#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <fcntl.h>

int		main(void)
{
	int	fd, n;
	char	buff[256];

	fd = open("/sys/kernel/debug/fortytwo/id", O_RDWR);
	if (fd < 0) {
		dprintf(STDERR_FILENO, "error on open /sys/kernel/debug/fortytwo/id\n");
		return (fd);
	}
	n = 0;
	bzero(buff, 256);
	while ((n = read(fd, &buff[0], 5)) > 0) {
		buff[n] = 0;
		printf("read 5 /sys/kernel/debug/fortytwo/id: [%s]\n", buff);
	}
	printf("write into /sys/kernel/debug/fortytwo/id \"heynard\": %d\n",
			write(fd, "heynard", 7));
	printf("write into /sys/kernel/debug/fortytwo/id \"tototot\": %d\n",
			write(fd, "tototot", 7));
	printf("write into /sys/kernel/debug/fortytwo/id \"toto\": %d\n",
			write(fd, "heynard", 4));
	close(fd);
	if (n < 0) {
		dprintf(STDERR_FILENO, "error on read /sys/kernel/debug/fortytwo/id\n");
		return (-1);
	}

	fd = open("/sys/kernel/debug/fortytwo/jiffies", O_RDONLY);
	if (fd < 0) {
		dprintf(STDERR_FILENO, "error on open /sys/kernel/debug/fortytwo/jiffies\n");
		return (fd);
	}
	printf("current jiffies timer value =: %d\n", read(fd, NULL, 0));
	close(fd);

	fd = open("/sys/kernel/debug/fortytwo/foo", O_RDWR);
	if (fd < 0) {
		dprintf(STDERR_FILENO, "error on open /sys/kernel/debug/fortytwo/foo\n");
		return (fd);
	}
	
	if (write(fd, "coucou", 6) < 0)
		dprintf(STDERR_FILENO, "error on write /sys/kernel/debug/fortytwo/foo\n");
	bzero(buff, 256);

	printf("read foo count = 6 : [");
	while ((n = read(fd, buff, 6)) > 0) {
		buff[n] = 0;
		printf("%s", buff);
	}
	printf("]\n");
	if (n < 0)
		dprintf(STDERR_FILENO, "error on read /sys/kernel/debug/fortytwo/foo\n");
	close(fd);

	return (0);
}
