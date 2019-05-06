/*----------------------------------------------------------------

*

* Programación avanzada: Proyecto 2

* Fecha: 4-Mayo-2019

* Autor: A01701260 Luis Alfredo Rodriguez

*

*--------------------------------------------------------------*/


#include <stdio.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <sys/ipc.h>
#include <sys/sem.h>


// Codigo de hola
#define HI       	1
// Un mensaje de envío de archivo (código '101' y la ruta del archivo).
#define SEND		101
// Un mensaje de desplegado de un directorio (código '102' y la ruta del directorio).
#define LS		    102
// Un mensaje de fin de conexión (código '103' con un "adios").
#define END		    103
// Permiso denegado (código '201' y el mensaje de error).
#define DENIED      201
// Archivo no encontrado (código '202' y el mensaje de error).
#define NOTFOUND	202
// Error interno (código '203' y el error generado).
#define ERROR       203
// Comando no conocido (código '204' y el mensaje de error).
#define UNKNOWN     204
// La ruta es un directorio (código '205' y el mensaje de error).
#define DIRECTORY   205
// Enviando archivo (código '301' y el contenido del archivo).
#define SENDFILE    301
// Directorio no encontrado (código '206' y el mensaje de error).
#define DIRNOTFOUND 206
// La ruta no es un directorio (código '207' y el mensaje de error).
#define NOTDIR      207
// Enviando el contenido de un directorio (código '302' y el listado del directorio).
#define SENDDIR     302
// Mutex
#define MUTEX       0


// Esta funcion es para el concatenar ya sea la ip con el mensaje para el log
char* create_message(char* string_1, char* string_2){
	char* result = malloc(strlen(string_1)+strlen(string_2)+1);
	strcpy(result, string_1);
	strcat(result, string_2);
	
	return result;
}


int wait_mutex(int semid, int sem_num) {
	struct sembuf op;

	op.sem_num = sem_num;
	op.sem_op = -1;
	op.sem_flg = 0;
	return semop(semid, &op, 1);
}

int signal_mutex(int semid, int sem_num) {
	struct sembuf op;

	op.sem_num = sem_num;
	op.sem_op = 1;
	op.sem_flg = 0;
	return semop(semid, &op, 1);
}

// Escribir en consola
void write_console(char* filename, char* text){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	FILE *fp;
	if(access(filename, F_OK) != -1){
		fp = fopen(filename, "a");
	}else{
		fp = fopen(filename, "w+");
	}
	
	fseek(fp, 0, SEEK_END);
	fprintf(fp, "%d-%d-%d %d:%d:%d IP: %s\n",tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, text);
	fflush(fp);
	fclose(fp);
}
