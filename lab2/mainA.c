#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>

typedef struct data_struct
{
	time_t time;
	pid_t pid;
} data_struct;

int main(int argc, char** argv) {
	int pipe_des[2];
	pid_t pid;
	pipe(pipe_des);

	switch (pid = fork())
	{
	case -1:
		printf("Error fork\n");
		return -1;
	case 0:
		printf("[CHILD]\n");
		close(pipe_des[1]);

		data_struct rec_data;
		read(pipe_des[0], &rec_data, sizeof(data_struct));

		close(pipe_des[0]);

		sleep(5);

		time_t rawtime;
		time(&rawtime);
		printf("\n[CHILD] Current time: %d h %d min %d sec\n", localtime(&rawtime)->tm_hour, localtime(&rawtime)->tm_min, localtime(&rawtime)->tm_sec);

		printf("[CHILD] Sender time: %d h %d min %d sec\n[CHILD] Sender pid %d\n", localtime(&(rec_data.time))->tm_hour, localtime(&(rec_data.time))->tm_min, localtime(&(rec_data.time))->tm_sec, rec_data.pid);

		return 0;
	default:
		printf("[PARENT]\n");

		time_t sender_rawtime;
		time(&sender_rawtime);

		pid_t send_pid = getpid();

		close(pipe_des[0]);

		data_struct send_data = { sender_rawtime ,send_pid };

		write(pipe_des[1], (void*)(&send_data), sizeof(data_struct));

		close(pipe_des[1]);

		printf("[PARENT] Sended data to child.\n");
		//sleep(5);
		return 0;
	}
	return 0;
}
