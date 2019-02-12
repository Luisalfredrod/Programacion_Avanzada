/*----------------------------------------------------------------

*

* Programación avanzada: Manejo de Directorios

* Fecha: 12-Feb-2019

* Autor: A01701260 Luis Alfredo Rodriguez

*

*--------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

void get_info(char *name, char *direc, char* program) {


  struct stat info;
  struct tm* timeinfo;
  time_t rawtimeA, rawtimeM;
  char filename[PATH_MAX + NAME_MAX + 1];
  sprintf(filename, "%s/%s", direc, name);
  
  if (lstat(filename, &info) < 0) {
    perror(program);
    exit(-1);
  }
  
  
  printf("Name: %s \n", name);
  printf("Path: %s \n", direc);
  
  
  rawtimeA = info.st_atime; 
  timeinfo = localtime(&rawtimeA);
  printf("Last access: %s", asctime(timeinfo));
  

  rawtimeM = info.st_mtime;
  timeinfo = localtime(&rawtimeM);
  printf("Last modification: %s\n", asctime(timeinfo));
}

void find(char *direc, char *str, int recu, char *program){
  DIR* dir;
  struct dirent* direntry;
  struct stat info;
  char filename[PATH_MAX + NAME_MAX + 1];
		
		
    	if ((dir = opendir(direc)) == NULL){
    		perror(program);
    		exit(-1);
		}
	  
	  while ( (direntry = readdir(dir)) != NULL ) {
	  	if (strstr(direntry->d_name, str) != NULL) {
	      sprintf(filename, "%s/%s", direc, direntry->d_name);      
	      get_info(direntry->d_name, direc, program);
	    }
	  }
	
	  if (recu) {
	    rewinddir(dir);
	    while ( (direntry = readdir(dir)) != NULL ) {
	      if (strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, "..") != 0) {
	        sprintf(filename, "%s/%s", direc, direntry->d_name);
	        lstat(filename, &info);
	        
	        if (S_ISDIR(info.st_mode)) {
	          find(filename, str, recu, program);
	        }
	        
	        
	      }
	    }
	  }
}


int main(int argc, char* argv[]) {
  
	char *directory;
	char *str;
	int recursive= 0;
	
	if (argc == 3){
	directory = argv[2];
	str = argv[1];
	} else if (argc == 4){
	    if (strcmp(argv[3], "-r") == 0){
	    	recursive= 1;
	    	directory = argv[2];
	    	str = argv[1];
	    }else{
	    	printf("Usage: %s str directory [-r]\n", argv[0]);
	    	return -1;
	    }
	    
	}else{
		printf("Usage: %s str directory [-r]\n", argv[0]);
		return -1;
	}
	
	find(directory, str, recursive, argv[0]);
	
	return 0;
}