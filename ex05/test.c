#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <strings.h>

int		main(void) {
	char	buff[50];
	int	fd, n;

	if ((fd = open("/dev/fortytwo", O_RDWR)) <= 0) {
		dprintf(STDERR_FILENO, "cannot open /dev/fortytwo\n");
		return (-1);
	}

	bzero(&buff[0], 50);
	while ((n = read(fd, &buff[0], 50)) > 0) {
		buff[n] = 0;
		printf("read %d char: [%s] \n", n, buff);
	}
	printf("write toto to /dev/fortytwo: %d\n", write(fd, "toto", 4));
	printf("write tototot to /dev/fortytwo: %d\n", write(fd, "tototot", 7));
	printf("write heynard to /dev/fortytwo: %d\n", write(fd, "heynard", 7));
	close(fd);
	
	if (n < 0) {
		dprintf(STDERR_FILENO, "error while reading\n");
		return (-1);
	}

	return (0);
}
