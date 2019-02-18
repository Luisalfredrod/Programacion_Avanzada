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
#include <dirent.h>
#include <errno.h>

void get_info(char *name, char *directory,char *dir_less, char *dir_gt, char* program, int size) {
	struct stat info;
	char filename[PATH_MAX + NAME_MAX + 1];
	
	sprintf(filename, "%s/%s", directory, name);
	if (lstat(filename, &info) < 0) {
		perror(program);
		exit(-1);
	}
	
	// FILE* infile; 
    // FILE* outfile;
    

    // infile = fopen(name, "r");
    // if ( size >= info.st_size){
    //     printf(name, "%s/%s", dir_less, name);

    //     outfile = fopen(name, "w");
    // }else{
    //     printf(name, "%s/%s", dir_gt, name);

    //     outfile = fopen(name, "w");
    // }

	printf(" %7li", info.st_size);
	
	printf(" %s\n", name);
    printf("done");
}

void list(char *directory,char *dir_less, char *dir_gt, int recursive, char* program, int size) {
	DIR* dir;
	struct dirent* direntry;
	struct stat info;
	char filename[PATH_MAX + NAME_MAX + 1];
	
	if ( (dir = opendir(directory)) == NULL ) {
		perror(program);
		exit(-1);
	}
	
	printf("DIRECTORY %s:\n", directory);
	while ( (direntry = readdir(dir)) != NULL ) {
		if (strcmp(direntry->d_name, ".") != 0 &&
			strcmp(direntry->d_name, "..") != 0) {
			sprintf(filename, "%s/%s", directory, direntry->d_name);
			get_info(direntry->d_name, directory, dir_less, dir_gt, program, size);
		}
	}
	
	if (recursive) {
		rewinddir(dir);
		while ( (direntry = readdir(dir)) != NULL ) {
			if (strcmp(direntry->d_name, ".") != 0 &&
				strcmp(direntry->d_name, "..") != 0) {
				
				sprintf(filename, "%s/%s", directory, direntry->d_name);
				lstat(filename, &info);
				if (S_ISDIR(info.st_mode)) {
					list(filename, dir_less, dir_gt, recursive, program, size);
				}
			}
		}
	}
	printf("\n");
}

int main(int argc, char* argv[]) {
	char dir_name[NAME_MAX + 1];
	char *directory;
	int recursive;
    char *dir_base, *dir_less, *dir_gt, *sc;
    int size;
    DIR* dirBase;
    DIR* dirLess;
    DIR* dirGt;
	
	if (argc > 5) {
		printf("usage: ./split base lesser greater N\n");
		return -1;
	}
	
	getcwd(dir_name, NAME_MAX);
	directory = dir_name;

    if (argc == 5){
        
        if(atoi(argv[4]) < 0){
            printf("./split: N must be a positive number greater than zero. \n");
            return -1;
        }else if ((dirBase = opendir(dir_base)) == NULL){
            perror(argv[0]);
            exit(-1);
        }else if ((dirLess = opendir(dir_less)) == NULL){
            perror(argv[0]);
            exit(-1);
        }else if ((dirGt = opendir(dir_gt)) == NULL){
            perror(argv[0]);
            exit(-1);
        }else
        {
            dir_base = argv[1];
            dir_less = argv[2];
            dir_gt = argv[3];
            sc = argv [4];
            size = atoi ( sc );
            printf("%i",size);
        }
        
    }else{
        printf("usage: ./split base lesser greater N\n");
		return -1;
    }
	
    recursive = 1;
	list(dir_base, dir_less, dir_gt, recursive, argv[0], size);
	return 0;
}
//     if(argc != 5)
//   {
//     printf("usage: ./split dir_base dir_less dir_gt N \n");
//     return -1;
