/*----------------------------------------------------------------

*

* Programación avanzada: Normalización de archivos

* Fecha: 22-Feb-2019

* Autor: A01701260 Luis Alfredo Rodriguez

*

*--------------------------------------------------------------*/




#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>



int main(int argc, char * argv[]){
	struct stat dir_input_info;
	struct stat dir_output_info;
	struct dirent *direntry;
	DIR *dir_input;
	FILE *arch_input, *arch_output;
	char filename_input[PATH_MAX + NAME_MAX + 1], filename_output[PATH_MAX + NAME_MAX + 1];
	double num[627], min = INT_MAX, max = INT_MIN;


	
	if (argc != 3){
		printf("usage: %s input directory, output directory\n", argv[0]);
		return -1;
	}

	if (lstat(argv[1], &dir_input_info) < 0){
		printf("%s: no such file or directory\n", argv[0]);
		return -1;
	}
	if (!(S_ISDIR(dir_input_info.st_mode))){
		printf("%s: %s is not a directory\n", argv[0], argv[1]);
		return -1;
	}
	
	if (lstat(argv[2], &dir_output_info) < 0){
		printf("%s: no such file or directory\n", argv[0]);
		return -1;
	}
	if (!(S_ISDIR(dir_output_info.st_mode))){
		printf("%s: %s is not a directory\n", argv[0], argv[2]);
		return -1;
	}
	
	
	
	if ((dir_input = opendir(argv[1])) != NULL){
		while ((direntry = readdir(dir_input)) != NULL){
			if (strcmp(direntry -> d_name, ".") != 0 && strcmp(direntry -> d_name, "..") != 0 && strcmp(direntry -> d_name, ".DS_Store") != 0) {
				sprintf(filename_input, "%s/%s", argv[1], direntry->d_name);
				sprintf(filename_output, "%s/%s", argv[2], direntry->d_name);

				if ((arch_input = fopen(filename_input, "r")) == NULL){
					printf("Could not open file %s\n", filename_input);
					return -1;
				} 
				if ((arch_output = fopen(filename_output, "w")) == NULL){
					printf("Could not open file %s\n", filename_output);
					return -1;
				}

				while (fscanf(arch_input, "%lf,%lf", &num[0], &num[1]) != EOF){
					// Guadar numero temporalmente para cambiar orden
					for (int i=2; i<627; i++){
						fscanf(arch_input, ",%lf", &num[i]);
						
						if (num[i] > max){
							max = num[i];
						}
						if (num[i] < min){
							min = num[i];
						}
					}
					fscanf(arch_input, "\n");
					
                    //http://sebastianraschka.com/Articles/2014_about_feature_scaling.html#about-min-max-scaling
					for (int i=2; i<627; i++){
						num[i] = (num[i] - min)/(max-min);
					}
					
					//imprimir en el archivo ls valores en el nuevo orden
					fprintf(arch_output, "%lf,%lf", num[0], num[1]);

					for (int i= 2; i< 627; i++){
						fprintf(arch_output, ",%lf", num[i]);
					}
					
					
					fprintf(arch_output, "\n");
					min = INT_MAX;
					max = INT_MIN;
				
				}
				fclose(arch_input);
				fclose(arch_output);
				
			}
		}

        printf("\nDone\n");
		closedir(dir_input);
	}
}
