/*----------------------------------------------------------------

*

* Programación avanzada: Creación de procesos.

* Fecha: 1-Mar-2019

* Autor: A01701260 Luis Alfredo Rodriguez Santos

*

*--------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>


void create_child(int i, int num_lvl, char* program);

void create_parent(int i, int num_lvl, char* program) {
	int pid = fork();
	int j;


	if (pid < 0) {
		perror(program);
		exit(-1);
		
	}else if (pid == 0) {
		create_child(++i, num_lvl, program);
	}else {
		for (j = 0; j < i; j++) {
			printf("\t");
		}
		printf("PPID = %i PID = %i LEVEL = %i\n", getppid(), getpid(), i);
		wait(NULL);
		exit(0);
	}
}

// Funcion: crea un proceso hijo de un proceso padre
void create_child(int i, int num_lvl, char* program) {
    int pid, j;
	
	if (i == 0) {
		create_parent(i, num_lvl, program);
	} 
	else if (i != (num_lvl + 1)) {
		for (j = 0; j < i; j++) {
			create_parent(i, num_lvl, program);
		}
	}
}

int main(int argc, char *argv[]){
	int num_lvl;
	
	
	if (argc != 2){
		fprintf(stderr, "Usage: %s [level]\n", argv[0]);
		return -1;
	}
    
	num_lvl = atoi(argv[1]);

	// Verifica que el número de procesos sea válido
	if (num_lvl <= 0){
		fprintf(stderr, "Error: level must be a positive integer greater than 0\n");
		return -1;
	}
	
    // Llama la función que crea el número de procesos establecidos
	create_parent(0, num_lvl, argv[0]);

	return 0;
}
