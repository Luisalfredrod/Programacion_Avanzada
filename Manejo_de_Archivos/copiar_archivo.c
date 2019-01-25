#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


// argc --------------> numero de argumentos incluyendo el nombre del programa
// argv identificador de argumentos ingresados en una cadena de char
// argv[0] es el nombre del archivo

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
	// buffer va a guardar la inforomacion del archivo para copiarlo
	char buffer;
	ssize_t nbytes;
	// Se copia el archivo
	while ( (nbytes = read(fd_in, &buffer, sizeof(buffer))) != 0 ) {
		write(fd_out, &buffer, nbytes);
	}	
	
	close(fd_in);
	close(fd_out);
	return 0;
}