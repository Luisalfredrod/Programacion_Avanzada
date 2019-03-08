/*----------------------------------------------------------------

*

* Programación avanzada: Problema del barbero.

* Fecha: 3-Mar-2019

* Autor: A01701260 Luis Alfredo Rodriguez

*

*--------------------------------------------------------------*/

#include <fcntl.h>
#include <sys/stat.h>
#include <limits.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
  
  char *ext="log";
  char *name=argv[2];
  char filename[PATH_MAX + NAME_MAX + 1];
  char newfilename[PATH_MAX + NAME_MAX + 1];
  
  int times = atoi(argv[3]);
  int time = atoi(argv[4]);
  int i, j, k, limit;
  DIR* dir;
  
// Argumentos necesarios de ejecucion
  if(argc !=5){
    printf("Usage: %s [directory] [log_file] [number_of_files] [timeout]\n", argv[0]);
    return -1;
  }
  if((dir=opendir(argv[1]))==NULL){
    perror(argv[0]);
    return -1;
  }
  
  sprintf(filename, "%s/%s.%s", argv[1], name, ext);
  FILE *fd;
  fd = fopen(filename, "r");
  
  if(fd==NULL){
    printf("%s\n", "File does not exist");
    return -1;
  }
  fclose(fd);
  
  
// Rename and rewrite files
  for(i = 0; i<times; i++){
    j=i;
    k=j+1;
    char str1[10];
    char str2[10];
    
    for(limit = i; limit > 0; limit--){
      int num = limit;
      char str3[10];
      char str4[10];

      sprintf(str3, "%d", num);
      sprintf(str4, "%d", num+1);
      sprintf(filename, "%s/%s.%s", argv[1], name, str3);
      sprintf(newfilename, "%s/%s.%s", argv[1], name, str4);
      
      rename(filename, newfilename);
    }
    
    sprintf(str1, "%d", j);
    sprintf(str2, "%d", k);
    sprintf(filename, "%s/%s.%s", argv[1], name, ext);
    sprintf(newfilename, "%s/%s.%s", argv[1], name, "1");
    
    rename(filename, newfilename);
    
    fd = fopen(filename, "w");
    fclose(fd);
    
    // Delay
    sleep(time);
  }
}