#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
void exit_handler(){
	printf("Процесс %d, с родительским процессом %d, завершен,сработал обработчик atexit().\n", getpid(), getppid());
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
		//	printf("Выход!\n");
			exit(0);
		default:
			printf("Это родительский  процесс\n");
			printf("PID %d\n", getpid());
			printf("PPID %d\n",getppid());
			printf("Ожидание, пока дочерний процесс не вызовет exit()...\n");
			wait(&status);
			printf("Статус возврата дочернего процесса:%d\n",WEXITSTATUS(status));      
		//	printf("Выход!\n");
	}
	return 0;
}
