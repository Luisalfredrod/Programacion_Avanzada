#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// 2028 bits para ser copiados
#define SIZE 2028

int main(int argc, char* argv[]) {
	int fd_in, fd_out;
	
	if (argc != 3) {
		printf("usage: %s origin destination\n", argv[0]);
		return -1;
	}
	
	if ( (fd_in = open(argv[1], O_RDONLY)) < 0 ) {
		perror(argv[0]);
		return -1;
	}
	
	if ( (fd_out = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0666)) < 0 ) {
		perror(argv[0]);
		return -1;
	}
	
	char buffer[SIZE];
	ssize_t nbytes;
    // Copiar 2028 bits especificados en size
    // En caso de no ser int se debe multiplicar por el size of
	while ( (nbytes = read(fd_in, buffer, SIZE)) != 0 ) {
		write(fd_out, buffer, nbytes);
	}	
	
	close(fd_in);
	close(fd_out);
	return 0;
}