#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>

#define MEM_NAME "buff_mem_4"

typedef struct data_struct
{
	time_t time;
	pid_t pid;
} data_struct;

struct sembuf sem_lock = { 0, -1, 0 }, sem_open = { 0,1,0 };

int main(int argc, char** argv) {

	key_t key = ftok(MEM_NAME, 9);

	if (key == -1) {
		printf("First process key creation error: %s\n", strerror(errno));
		exit(-1);
	}

	time_t send_time;
	pid_t send_pid;
	send_time = time(NULL);
	send_pid = getpid();
	data_struct struct_to_send = { send_time ,send_pid };

	int shmem_id = shmget(key, sizeof(data_struct), IPC_CREAT | 0666);

	struct shmid_ds buf;
	shmctl(shmem_id, IPC_STAT, &buf);
	
	if (shmem_id < 0)
	{
		printf("First process SHMEM creation failed with code: %s\n", strerror(errno));
		exit(-1);
	}
	if (buf.shm_nattch >= 1)
	{
		printf("Attachment already exists\n");
		exit(-1);
	}
	
	int sem_id = semget(key, 1, IPC_CREAT | 0666);
	if (sem_id < 0) {
		printf("SEM creation error code: %s\n", strerror(errno));
		exit(-1);
	}
	semop(sem_id, &sem_open, 1);
	while (1)
	{
		
		semop(sem_id, &sem_lock, 1);
		void* struct_in_mem = shmat(shmem_id, NULL, 0);

		if (struct_in_mem < 0) {
			printf("First process SHMEM attach failed with code: %s\n", strerror(errno));
			exit(-1);
		}

		//data_struct struct_to_send;
		
		send_time = time(NULL);
		send_pid = getpid();
		struct_to_send.time = send_time;
		struct_to_send.pid = send_pid;
		*((data_struct*)struct_in_mem) = struct_to_send;
		semop(sem_id, &sem_open, 1);
		
		sleep(5); 
		shmdt(struct_in_mem);

		
	};
	return 0;
}
