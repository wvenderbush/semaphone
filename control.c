#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/sem.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
	int semid;
	int key = ftok("makefile", 20);
	int sc;

	if ( argv[1] == NULL){
		printf("Please provide an argument!\n");
		return 1;
	}

	if (strncmp( argv[1], "-c", strlen(argv[1])) == 0) {
		semid = semget( key, 1, IPC_CREAT | IPC_EXCL | 0644 );
		if (semid >= 0) {

			int f = open("phone.log", O_CREAT, 0644);
			if (f == -1){
				printf("Unable to open file!\n");
				return 1;
			}
			close(f);

			printf("Story Created [@%d]\n", semid);
			int value = 1;
			if (argv[2] != NULL){
				value = atoi(argv[2]);
			}

			int sd = shmget( key, 4, O_CREAT | 0644 );
			int *a = (int *) shmat( sd, 0, 0 );
			*a = 0;
			shmdt(a);

			union semun su;
			su.val = value;
			sc = semctl( semid, 0, SETVAL, su );
		}
		else {
			printf("Semaphore already exists!\n");
			return 1;
		}
	}

	else if (strncmp( argv[1], "-v", strlen(argv[1])) == 0) {
		FILE *f;
	    char c;
	    f = fopen("phone.log", "rt");
	    if (f == NULL){
	    	printf("Unable to open file!\n");
	    	return 1;
	    }
	    printf("Story So Far:\n\n");

	    while((c = fgetc(f))!= EOF){
	        printf("%c", c);
	    }
	    fclose(f);
	    return 0;
	}

	else if (strncmp( argv[1], "-r", strlen(argv[1])) == 0){
		int sd = shmget( key, 4, 0644 );
		semid = semget(key, 1, 0);
		int shc = shmctl( sd, IPC_RMID, 0);
		sc = semctl( semid, 0, IPC_RMID);
		printf("Story Removed [@%d]\n\n", semid);
		FILE *f;
	    char c;
	    f = fopen("phone.log", "rt");
	    if (f == NULL){
	    	printf("Unable to open file!\n");
	    	return 1;
	    }
		printf("Story:\n");
		while((c = fgetc(f))!= EOF){
	        printf("%c", c);
	    }
	    fclose(f);
	    return 0;
	}
	return 0;
}
