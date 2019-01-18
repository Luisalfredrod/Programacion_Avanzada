#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	long *p;
	long x = 10; //, y = 1000;
	
	/*
	p = (long*) malloc(sizeof(long));
	if (!p) {
		perror(argv[0]);
		exit(0);
	}
	*/
	p = &x;
	printf("value of x = %li\n", x);
	printf("memory address (&x) = %p\n", (&x));
	printf("pointer p = %p\n\n\n", p);
	//Change the value of x
	(*p) = 10;
	printf("modifying through p\n");
	printf("value of x = %li\n", x);
	printf("memory address (&x) = %p\n", (&x));
	printf("pointer p = %p\n\n\n", p);
	
	(*p)++;
	printf("modifying through p\n");
	printf("value of x = %li\n", x);
	printf("memory address (&x) = %p\n", (&x));
	printf("pointer p = %p\n\n\n", p);
	
    //Move location of pointer
	p++;
	printf("moving pointer p\n");
	printf("pointer p = %p\n", p);
	printf("content (*p) = %li\n\n\n", (*p));
    
	//Can't be modified because of the pointer location
	(*p) = 14;
	printf("modifying through p\n");
	printf("pointer p = %p\n", p);
	printf("content (*p) = %li\n\n\n", (*p));
	
	//free(p);
	
	return 0;
}