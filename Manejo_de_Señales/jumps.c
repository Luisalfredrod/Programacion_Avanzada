/*----------------------------------------------------------------

*

* Programación avanzada: Manejando señales

* Fecha: 12-Mar-2019

* Autor: A01701260 Luis Alfredo Rodriguez

*

*--------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <math.h>

void cuadrados() {
	int i,res,temp;
	
    printf("\nPunto de retorno 1:\nTabla de cuadrados\nNumero Cuadrado");

    for(i = 0; i < 5; i++){
        temp = i+1;
        res = pow(temp,2);
        printf("\n%i\t%i",(i+1),res);
    }
	

}


void raices() {
	int i;
    float res;
	
    printf("\nPunto de retorno 2:\nTabla de raices cuadradas\nNúmero Raíz cuadrada\n");

    for(i = 0; i < 5; i++){
        res = sqrt((i+1));
        printf("%i\t%.6f\n",(i+1),res);
    }
}


void logaritmos(){
	int i;
    float res;
	
    printf("\nPunto de retorno 3:\nTabla de logaritmos\nNúmero Logaritmo");

    for(i = 0; i < 5; i++){
        res = log((i+1));
        printf("\n%i\t%.6f",(i+1),res);
    }
}

void handler(int sig){

    switch (sig)
    {
        case SIGHUP:
            cuadrados();
            raices();
            logaritmos();
            break;
    
        case SIGINT:
            raices();
            logaritmos();
            break;

        case SIGQUIT:
            logaritmos();
            break;
    
    	default:
		printf("\nSenal invalida!\n");
		break;
    }

}


int main(int argc, char* argv[]) {
	
	signal(SIGHUP, handler);
	signal(SIGINT, handler);
	signal(SIGQUIT, handler);

	if (argc != 1) {
		printf("\nusage: jumps\n");
		return -1;
	}
	else {
		cuadrados();
		raices();
		logaritmos();
		printf("\nPID = %i\n", getpid());
	}

	while(1){
		sleep(10);
	}
	
	return 0;
}