/*----------------------------------------------------------------

*

* Programación avanzada: Examen 2

* Fecha: 19-Mar-2019

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
#include <string.h>
#include <ctype.h>
 
#define MAX 256 
 
void limpia(char *cadena) 
{  
    char *p = strchr(cadena, '\n');  
    if (p) *p = '\0';
} 
 
int main(int argc, char *argv[]) 
{  
    int pipeA[2];  
    int pipeB[2];  
    int pid,parent;  
    char mensaje[MAX];
    char *c;

    if(argc > 1)
    {
        printf("usage: pipe");
        return -1;
    }

    if (pipe(pipeA) < 0 || pipe(pipeB) < 0) 
    {   
        perror("pipe");
        return -1; 
    }    
    
    if ((pid = fork()) < 0) 
    {   
        perror("fork");   
        return -1;  
    } 
    else if (pid == 0) 
    {   
        while(read(pipeA[0], mensaje, MAX) > 0 && strcmp(mensaje, "EOF") != 0) 
        {   
            int i = 0;
                                        
            while(mensaje[i]) 
            {
                mensaje[i] = toupper(mensaje[i]);
                i++;
            }                           
            write(pipeB[1], mensaje, strlen(mensaje) + 1);   
        }
        close (pipeA[0]);   
        close (pipeA[1]);   
        close (pipeB[0]);   
        close (pipeB[1]);   
        return 0;  
    } 
    else 
    {   
        parent = getpid();
        printf("pid = %i parent ...\n",parent);
        printf("pid = %i, ppid = %i process starting ...\n",parent+1, parent);
        do 
        {    
            fprintf(stdout, "INPUT: ");    
            while(fgets(mensaje, MAX, stdin))
            {            
                limpia(mensaje);                
                write(pipeA[1], mensaje, strlen(mensaje) + 1);    
                if (strcmp(mensaje, "EOF") == 0) break;            
                do 
                {     
                    strcpy(mensaje, "");     
                    read(pipeB[0], mensaje, MAX);
                    fprintf(stdout,  "%s\n", mensaje);
                    strcpy(mensaje, "LISTO");
                } while (strcmp(mensaje, "LISTO") != 0);            
            }
            exit(EXIT_SUCCESS);               
        } while (1);   
        close (pipeA[0]);  
        close (pipeA[1]);   
        close (pipeB[0]);
        close (pipeB[1]);   
        return 0; 
    }
}




