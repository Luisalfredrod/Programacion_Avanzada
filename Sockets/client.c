    
/*-----------------------------------------------------------

* Programación avanzada: Echo server

* Fecha: 26-Abr-2019

* Autor: A01701260 Luis Alfredo Rodriguez Santos

*------------------------------------------------------------*/

#include "header.h"
#include "string.h"
int main(int argc, char* argv[]) {
    int sfd, answer, continuE;
    char str[50];
    char ip[16];
	int port, guess;
	struct sockaddr_in server_info;
	
	if (argc != 2) {
	    printf("usage: %s ip port\n", argv[0]);
	    return -1;
	}
	
	port = DEFAULT_PORT;
	if (port < 5000) {
		printf("%s: The port must be greater than 5000.\n", argv[0]);
		return -1;
	}
	
	if ( (sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		perror(argv[0]);
		return -1;
	}
	strcpy(ip,argv[1]);
	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = inet_addr(argv[1]);
	server_info.sin_port = htons(port);
	if ( connect(sfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0 ) {
		perror(argv[0]);
		return -1;
	}
	
	// do {
	// 	guess = 0;
	// 	while (!guess) {
			printf("Input the message: ");
            fgets(str,50,stdin);
			// scanf("%49s", str);
			
			write(sfd, &str, sizeof(str));
			read(sfd, &str, sizeof(str));
			fprintf(stdout,"\nServer %s : %s\n",ip,str);
			// switch (answer) {
			// 	case SMALLER: printf("The number is smaller.\n"); break;
			// 	case BIGGER : printf("The number is bigger.\n"); break;
			// 	default     : guess = 1; break;
			// }
		// }
	// 	printf("You guessed it! Congratulations!\n");
	// 	printf("Continue? (0/1)? ");
	// 	scanf("%i", &continuE);
		
	// 	answer = (continuE == 1)? CONTINUE : END;
	// 	write(sfd, &answer, sizeof(answer));
	// } while (continuE == 1);
	close(sfd);
	return 0;
}