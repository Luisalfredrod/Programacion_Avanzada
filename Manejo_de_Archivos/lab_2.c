/*----------------------------------------------------------------

*

* Programación avanzada: Cifrado de un archivo

* Fecha: 1-Feb-2019

* Autor: A01701260 Luis Rodriguez

*

*--------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char* argv[]){
    int i, limitbit = 128;
    int amt = atoi(argv[1]);
    char s[limitbit];
    char buffer[limitbit];
    ssize_t x;
    int input,output;

    if ( (input = open(argv[2], O_RDONLY)) < 0 ) {
		perror(argv[0]);
		return -1;
	}
	
	if ( (output = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0666)) < 0 ) {
		perror(argv[0]);
		return -1;
	}

    if(argc!=4){
        printf("usage: number origin destination\n");
        return 0;
        
    } else if (amt<1){
        printf("encryption: the first parameter must be a positive integer number.\n");
        return 0;
    }else {
        // Doesn't find the input file
        if ((input)<0){
            printf("File or directory does not exist.\n");
            return 0;
        } else {
            
            while ((x =read(input, buffer, sizeof(buffer))) != 0){
                
                if(x == limitbit){
                    // Loop ends when 128 bits have been moved
                    for( i = 0; i < limitbit; i++){
                
                        if((i - amt)>-1){
                            s[i-amt]=buffer[i];
                        } else {
                            s[i-amt+limitbit]=buffer[i];
                        }
                    }
                    
                    //copies n characters from memory one area to another memory area
                    memcpy(buffer, s, limitbit);
                    
                    //writes the new output file
                    write(output, buffer, x);
                } else {
                    write(output, buffer, x);
                }
            }
            // Closes files
            close(input);
            close(output);
        }
    }
}