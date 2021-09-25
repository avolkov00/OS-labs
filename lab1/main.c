#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
void exit_handler(){
	printf("Процесс завершен,сработал обработчик atexit(). \n");
}
	
int main()
{
	pid_t pid;
	int status;

	atexit(exit_handler);
	switch(pid=fork()) {
		case -1:
			exit(1); 
		case 0:
			printf("Это дочерний процесс\n");
			printf("PID %d\n", getpid());
			printf("PPID %d\n",getppid());
			printf("Выход!\n");
			exit(status);
		default:
			printf("Это родительский  процесс\n");
			printf("PID %d\n", getpid());
			printf("PPID %d\n",getppid);
			printf("Ожидание, пока дочерний процесс не вызовет exit()...\n");
			while ((wait(&status)) > 0)          
			printf("Выход!\n");
	}
}
