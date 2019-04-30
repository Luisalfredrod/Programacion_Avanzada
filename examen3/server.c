#include "header.h"
#include <string.h>

void serves_client(int nsfd) {

    uchar cmd;
    ulong id; 
    ulong val;

	read(nsfd, &cmd, sizeof(uchar));
	read(nsfd, &id, sizeof(ulong));
	read(nsfd, &val, sizeof(ulong));
    if(cmd == 101){
        //val = ask_db(id);
        sleep(1);
    }else if(cmd == 102)
    {
        //update_db(id,val);
        sleep(5);
    }
    


    write(nsfd, &cmd, sizeof(uchar));
	write(nsfd, &val, sizeof(ulong));

	close(nsfd);
}

void update_db(ulong id, ulong val){
	FILE* file;
	file = fopen("inventory.dat", "a");
	
	if (!file) {
		perror("inventory.dat");
		exit(-1);
	} 

    // CarPart aux;
    // aux.id = id;
    // strcpy(aux.name, ("part " + id)); 
    // aux.quantity = val;
    // fwrite(aux, sizeof(CarPart), 1, file);

    fclose(file);
}
int ask_db(ulong id){
    ulong val;
	FILE* file;
	file = fopen("inventory.dat", "r");

	
	if (!file) {
		perror("inventory.dat");
		exit(-1);
	} 

    fclose(file);
    return val;
}

void server(char* ip, int port, char* program) {
	int sfd, nsfd, pid;
	struct sockaddr_in server_info, client_info;

	if ( (sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		perror(program);
		exit(-1);
	}
	
	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = inet_addr(ip);
	server_info.sin_port = htons(port);
	if ( bind(sfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0 ) {
		perror(program);
		exit(-1);
	}
	
	listen(sfd, 1);
	while (1) {
		int len = sizeof(client_info);
		if ( (nsfd = accept(sfd, (struct sockaddr *) &client_info, &len)) < 0 ) {
			perror(program);
			exit(-1);
		}
		
	
		if ( (pid = fork()) < 0 ) {
			perror(program);
		} else if (pid == 0) {
			close(sfd);
			serves_client(nsfd);
			exit(0);
		} else {
			close(nsfd);
		}
	}
}

int main(int argc, char* argv[]) {
	char ip[15];	
	int port;
	
	strcpy(ip, SERVER_IP);
	port = SERVER_PORT;
	
	server(ip, port, argv[0]);
	
	return 0;
}