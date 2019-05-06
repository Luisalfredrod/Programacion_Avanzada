/*----------------------------------------------------------------

*

* Programación avanzada: Proyecto 2

* Fecha: 4-Mayo-2019

* Autor: A01701260 Luis Alfredo Rodriguez

*

*--------------------------------------------------------------*/

#include "headerServer.h"



void message_101 (char* data_sent, char* data_log, char* log_filename, char* ip){
	data_log = create_message(ip, " Comando recibido: 101, Parametro: ");
	data_log = create_message(data_log, data_sent);
	write_console(log_filename, data_log);	
}

void message_102(char* data_sent, char* data_log, char* log_filename, char* ip){
	data_log = create_message(ip, " Comando recibido: 102, Parametro: ");
	data_log = create_message(data_log, data_sent);
	write_console(log_filename, data_log);
}


// Maneja señales mandadas al servidor
void server_handler(int nsfd, char* directory, char* ip, char* program){
	int semid;
	key_t key;

	if((key = ftok("/dev/null", 65)) ==(key_t) -1){
		(program);
		exit(-1);
	}
	// por si no tiene permisos
	if((semid = semget(key, 1, 0666)) <0){
		perror(program);
		exit(-1);
	}
	
	// Variable para el registro o log
	char* data_log= "";
	// Variable que guarda el path y nombre del archivo log
	char log_filename[PATH_MAX+NAME_MAX+1];
	sprintf(log_filename, "%s/log.txt", directory);
	// Guarda la primera conexion en el log
	data_log = create_message(ip, "\nSe ha iniciado una conexcion con ip:");
	// espera
	wait_mutex(semid, MUTEX);
	// Escribe en el log
	write_console(log_filename, data_log);
	signal_mutex(semid, MUTEX);
	// primer mensaje que es mandado
	char* data = "Hello there";
	char* data_sent;
	char filename[PATH_MAX+NAME_MAX+1];
	// variable de retorno de llamada del servicor, comienza con el HI que es el 1
	int answer = HI, input_sent;
	long length = strlen(data), length_sent, data_read;
	write(nsfd, &answer, sizeof(answer));
	write(nsfd, &length, sizeof(length));
	write(nsfd, data, length * sizeof(char));
	srand(getpid());
	
	while( input_sent != END ){
			data = "";
			data_log = "";
			read(nsfd, &input_sent, sizeof(input_sent));
			read(nsfd, &length_sent, sizeof(length_sent));
			data_sent =(char*) malloc(length_sent*sizeof(char));
			data_read = read(nsfd, data_sent, length_sent * sizeof(char));
			data_sent[length_sent] = '\0';
			struct stat info;
			
				//Si el codigo es 101
			if( input_sent == SEND ){
				
				if( data_sent[0] != '/' ) {
					wait_mutex(semid, MUTEX);
					message_101(data_sent,data_log,log_filename,ip);
					data_log = "";
					data_log = create_message(ip, " Respuesta mandada: 203, Parametro: Error interno");
					write_console(log_filename, data_log);
					signal_mutex(semid, MUTEX);
					//retorno de mensaje de error
					answer = ERROR;
					data = " Error";
				} else {
					sprintf(filename, "%s%s", directory, data_sent);
					printf("%s\n", filename);
					int s = lstat(filename, &info);
					
					if( s < 0 ){
						wait_mutex(semid, MUTEX);
						message_101(data_sent,data_log,log_filename,ip);
						data_log = "";
						data_log = create_message(ip, " Respuesta mandada: 202, Parametro: Archivo no encontrado");
						write_console(log_filename, data_log);
						signal_mutex(semid, MUTEX);
						//mensaje de error en caso que no se encuenntre el archivo
						answer = NOTFOUND;
						data = "No se encontro el archivo";
					} else {
						
						if( S_ISDIR(info.st_mode) ) {
							wait_mutex(semid, MUTEX);
							message_101(data_sent,data_log,log_filename,ip);
							data_log = "";
							data_log = create_message(ip, " Respuesta mandada: 205, Parametro: La ruta era de un directorio");
							write_console(log_filename, data_log);
							signal_mutex(semid, MUTEX);
							//Regreso de mensaje con el codigo de 205
							answer = DIRECTORY;
							data = " La ruta es directorio";
						} else {
							printf("%s\n", filename);
							
							if(access(filename, R_OK) == 0){
								char buffer;
								char* target_filename = data_sent+1;
								int fd_in, fd_out;
								fd_in = open(filename, O_RDONLY);
								fd_out = open(target_filename, O_WRONLY | O_TRUNC | O_CREAT, 0666);
								ssize_t nbytes;
								
								//se escriben el fd_in en fs_out bit por bit
								while((nbytes = read(fd_in, &buffer, sizeof(buffer))) != 0){
									//escritura bit por bit de la copia del archivo
									write(fd_out, &buffer, nbytes);
								}

								wait_mutex(semid, MUTEX);
								message_101(data_sent,data_log,log_filename,ip);
								data_log = "";
								data_log = create_message(ip, " Respuesta enviada: 301, Parametro: Enviando archivo");
								write_console(log_filename, data_log);
								signal_mutex(semid, MUTEX);
								//mensaje de enviando archivo cuando el codigo es 301
								answer = SENDFILE;
								data = " Enviando archivo";
								close(fd_in);
								close(fd_out);
							} else {
								
								wait_mutex(semid, MUTEX);
								message_101(data_sent,data_log,log_filename,ip);
								data_log = "";
								data_log = create_message(ip, " Respuesta enviada: 201, Parametro: Permiso denegado");
								write_console(log_filename, data_log);
								signal_mutex(semid, MUTEX);
								//Mandas como respuesta un mensaje de denegado al no tener los permisos
								answer = DENIED;
								data = "Permiso denegado";
							}
						}
					}
				}
			//Codigo LS que es 102
			} else if(input_sent == LS) {
				if( data_sent[0] != '/' ) {
					wait_mutex(semid, MUTEX);
					message_102(data_sent,data_log,log_filename,ip);
					data_log = "";
					data_log = create_message(ip, " Respuesta enviada: 203, Parametro: Error interno");
					write_console(log_filename, data_log);
					signal_mutex(semid, MUTEX);
					//regreso de error
					answer = ERROR;
					data = " Error";
				} else {
					sprintf(filename, "%s%s", directory, data_sent);
					if( lstat(filename, &info) != 0 ) {
						wait_mutex(semid, MUTEX);
						message_102(data_sent,data_log,log_filename,ip);
						data_log = "";
						data_log = create_message(ip, " Respuesta: 206 Parametro: Directorio no encontrado");
						write_console(log_filename, data_log);
						signal_mutex(semid, MUTEX);
						//No se encontro el Directiorio
						answer = DIRNOTFOUND;
						data = " No se encontro el directorio";
					} else {
						DIR* dir;
							//en caso de no encontrar el directorio que fue enviado
						if( (dir = opendir(filename)) == NULL ) {
							wait_mutex(semid, MUTEX);
							message_102(data_sent,data_log,log_filename,ip);
							data_log = "";
							data_log = create_message(ip, " Respuesta enviada: 207, Parametro: La ruta era un directorio");
							write_console(log_filename, data_log);
							signal_mutex(semid, MUTEX);
							//se manda el mensaje de error 
							answer = NOTDIR;
							data = " La ruta era un directorio";
								//en caso que si se encontro el directorio que fue enviado
						} else {
							answer = SENDDIR;
							struct dirent* direntry;
							//lee los contenidos del directorio
							while( (direntry = readdir(dir)) != NULL ) {
								//busca el archivo comparando los nombres
								if(strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, "..") != 0){
									data = create_message(data, direntry->d_name);
									data = create_message(data, "\n");
								}
							}
							wait_mutex(semid, MUTEX);
							message_102(data_sent,data_log,log_filename,ip);
							data_log = "";
							data_log = create_message(ip, " Respuesta enviada: 302, Parametro: Enviando el contenido del directorio: \n");
							data_log = create_message(data_log, data);
							write_console(log_filename, data_log);
							//Señal al mutex
							signal_mutex(semid, MUTEX);
						}
						closedir(dir);
					}
				}
			} else {
					//cuado el mensaje es el de terminar
				if( input_sent == END ) {
					wait_mutex(semid, MUTEX);
					data_log = create_message(ip, " Se ha terminado la conexion");
					write_console(log_filename, data_log);
					signal_mutex(semid, MUTEX);
				} else {
					wait_mutex(semid, MUTEX);
					//se guarda el mensaje en caso de ser un codigo 102
					message_102(data_sent,data_log,log_filename,ip);
					//se limpia la variable del mensaje
					data_log = "";
					//se guarda el mensaje de respuesta
					data_log = create_message(ip, " Respuesta enviada: 204, Parametro: Comando no existente");
					//se escribe en el log
					write_console(log_filename, data_log);
					signal_mutex(semid, MUTEX);
					//regreso de mensaje de desconocido o inexistente comando
					answer = UNKNOWN;
					data = "Comando no existente";
				}
			}
			length = strlen(data);
			write(nsfd, &answer, sizeof(answer));
			write(nsfd, &length, sizeof(length));
			write(nsfd, data, length * sizeof(char));
			//liberar variable
			free(data_sent);
			memset(filename, 0, PATH_MAX+NAME_MAX+1);
	}
	close(nsfd);
}


void start_server(char* ip, int port, char* directory, char* program){
	int sfd, nsfd, pid;
	struct sockaddr_in server_info, client_info;
	if((sfd = socket(AF_INET, SOCK_STREAM, 0))<0){
		perror(program);
		exit(-1);
	}

	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = inet_addr(ip);
	server_info.sin_port = htons(port);
	if( bind(sfd,(struct sockaddr *) &server_info, sizeof(server_info))<0){
		perror(program);
		exit(-1);
	}

	listen(sfd, 1);
	while(1){
		socklen_t len = sizeof(client_info);
		if((nsfd = accept(sfd,(struct sockaddr *) &client_info, &len))<0){
			perror(program);
			exit(-1);
		}

		if((pid = fork())<0){
			perror(program);
		}else if(pid == 0){
			close(sfd);
			server_handler(nsfd, directory, ip, program);
			exit(0);
		}else{
			close(nsfd);
		}
	}
}


int main(int argc, char* argv[]){
	int semid;
	unsigned short final_values[1];
	key_t key;
	
	if(argc != 4){
		printf("Usage: %s dir_ip port root_dir \n", argv[0]);
		return -1;
	}

	int port = atoi(argv[2]);
	if(port<5000){
		printf("%s The port must be grater than 5000\n", argv[0]);
		return -1;
	}
	
	char* directory = argv[3];
	if(directory[0] != '/'){
		printf("%s The path must be an absolute path\n", argv[0]);
		return -1;
	}

	DIR* dir = opendir(directory);
	if(!dir){
		perror(argv[0]);
		return -1;
	}
	if((key = ftok("/dev/null", 65)) ==(key_t) -1){
		perror(argv[0]);
		return -1;
	}
	if((semid = semget(key, 1, 0666 | IPC_CREAT))<0){
		perror(argv[0]);
		return -1;
	}
	semctl(semid, MUTEX, SETVAL, 1);
	semctl(semid, 0, GETALL, final_values);
	start_server(argv[1], port, directory, argv[0]);

	return 0;
}
