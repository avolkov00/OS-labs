#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>

#define MEM_NAME "buff_mem_4"

typedef struct data_struct
{
	time_t time;
	pid_t pid;
} data_struct;

int main(int argc, char** argv) {

	key_t key = ftok(MEM_NAME, 7);

	if (key == -1) {
		printf("First process key creation error: %s\n", strerror(errno));
		exit(-1);
	}

	time_t send_time;
	pid_t send_pid;
	send_time = time(NULL);
	send_pid = getpid();
	data_struct struct_to_send = { send_time ,send_pid };

	int shmem_id = shmget(key, sizeof(data_struct), IPC_CREAT | IPC_EXCL | 0666);

	if (shmem_id < 0)
	{
		printf("First process SHMEM creation failed with code: %s\n", strerror(errno));
		exit(-1);
	}

	//shmctl(shmem_id, IPC_RMID, NULL);

	while (1)
	{

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

		shmdt(struct_in_mem);
		sleep(5);

	};
	return 0;
}
