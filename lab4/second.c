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
	sleep(1);

	key_t key = ftok(MEM_NAME, 9);
	if (key == -1) {
		printf("Second process key creation error: %s\n", strerror(errno));
		exit(-1);
	}

	int shmem_id = shmget(key, sizeof(data_struct), 0666);

	if (shmem_id < 0)
	{
		printf("Second process SHMEM creation failed with code: %s\n", strerror(errno));
		exit(-1);
	}

	int sem_id = semget(key, 1, IPC_CREAT | 0666);
	if (sem_id < 0) {
		printf("SEM creation error code: %s\n", strerror(errno));
		exit(-1);
	}

	semop(sem_id, &sem_lock, 1);
	void* struct_in_mem = shmat(shmem_id, NULL, 0);
	data_struct buf_data = *((data_struct*)struct_in_mem);

	if (struct_in_mem < 0) {
		printf("Second process SHMEM attach failed with code: %s\n", strerror(errno));
		exit(-1);
	}
	time_t cur_time = time(NULL);
	printf("Current time %sPID of this process %d\n", ctime(&cur_time), getpid());
	printf("Time of transmitting process %sPID of transmitting process %d\n", ctime(&(buf_data.time)), buf_data.pid);
	shmdt(struct_in_mem);
	semop(sem_id, &sem_open, 1);
	return 0;
}
