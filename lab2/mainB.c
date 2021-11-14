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
	pid_t pid_0;
	switch (pid_0 = fork())
	{
	case -1:
		printf("Error fork\n");
		return -1;
	case 0:
		printf("[CHILD]\n");
		sleep(1);
		int fifo_read = open("temporary_fifo", O_RDONLY);

		if (fifo_read < 0) {
			printf("[CHILD] Error reading fifo\n");
			return -1;
		}

		data_struct rec_data;
		read(fifo_read, &rec_data, sizeof(data_struct));
		close(fifo_read);

		sleep(5);


		time_t rawtime;
		time(&rawtime);
		printf("\n[CHILD] Current time: %d h %d min %d sec\n", localtime(&rawtime)->tm_hour, localtime(&rawtime)->tm_min, localtime(&rawtime)->tm_sec);

		printf("[CHILD] Sender time: %d h %d min %d sec\n[CHILD] Sender pid %d\n", localtime(&(rec_data.time))->tm_hour, localtime(&(rec_data.time))->tm_min, localtime(&(rec_data.time))->tm_sec, rec_data.pid);

		return 0;
	default:
		printf("[PARENT]\n");
		int fifo;
		if (access("temporary_fifo", F_OK) == -1) {
			fifo = mkfifo("temporary_fifo", 0777);
		}

		int fifo_write = open("temporary_fifo", O_WRONLY);

		if (fifo_write < 0) {
			printf("[PARENT] Error writing fifo\n");
			return -1;
		}
		time_t sender_rawtime;
		time(&sender_rawtime);

		pid_t send_pid = getpid();

		data_struct send_data = { sender_rawtime ,send_pid };

		write(fifo_write, &send_data, sizeof(data_struct));
		close(fifo_write);

		printf("[PARENT] Sended data to child.\n");
		//sleep(5);
		return 0;
	}
	return 0;
}
