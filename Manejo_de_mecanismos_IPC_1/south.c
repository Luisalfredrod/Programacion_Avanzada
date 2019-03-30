#include "header.h"

void a_south(char* program){
	int semid;
	key_t key;

	if((key = ftok("/dev/null", 65)) ==(key_t) -1){
		perror(program);
		exit(-1);
	}

	if((semid = semget(key, 4, 0666))  < 0){
		perror(program);
		exit(-1);
	}

	printf("South farmer %i trying to cross the bridge.\n", getpid());
	mutex_wait(semid, MUTEX);
	printf("South farmer %i trying to cross the bridge with %i space\n", getpid(), semctl(semid, BRIDGE, GETVAL, 0));
  sem_wait(semid, BRIDGE, 1);

  if(semctl(semid, NORTH, GETVAL, 0) == 1){
    printf("The bridge is full\n");
    sem_signal(semid, BRIDGE, 1);
  }
	else if((semctl(semid, NORTH, GETVAL, 0) == 1) &&(semctl(semid, SOUTH, GETVAL, 0) == 1)){
		printf("Get off south farmer!!!!\n");
		sem_signal(semid, BRIDGE, 1);
	}
  else{
    sem_signal(semid, SOUTH, 1);
  	printf("South farmer %i accepted in the bridge.\n", getpid());
    printf("%i North farmer in the bridge, %i South farmer in the bridge\n", semctl(semid, NORTH, GETVAL, 0), semctl(semid, SOUTH, GETVAL, 0));
  }
  if(semctl(semid, BRIDGE, GETVAL, 0) == 0){
    sem_signal(semid, BRIDGE, 1);
    printf("Crossing %i North farmers in the bridge, %i South Farmers in the bridge\n\n", semctl(semid, NORTH, GETVAL, 0), semctl(semid, SOUTH, GETVAL, 0));
		sleep(3);
    semctl(semid, NORTH, SETVAL, 0);
    semctl(semid, SOUTH , SETVAL, 0);
  }

	mutex_signal(semid, MUTEX);


	exit(0);
}

int main(int argc, char* argv[]){
	int amount = 0, pid;

	if(argc != 2){
		printf("usage: %s amount\n", argv[0]);
		return -1;
	}

	amount = atoi(argv[1]);
	if(amount < 1){
		printf("%s: The amount must be a positive number greater than zero.\n", argv[0]);
		return -1;
	}

	for(int i = 0; i < amount; i++){
		if((pid = fork()) < 0){
			perror("fork");
			return -1;
		} else if(pid == 0){
			a_south(argv[0]);
		} else{
		}
	}
	return 0;
}
