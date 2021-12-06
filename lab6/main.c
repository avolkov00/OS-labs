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
#include <pthread.h>

static int counter = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;

void* writeT() {
	int local = 0;
	sleep(1);
	while (1) {
		pthread_mutex_lock(&mutex);
		counter+= 1;
		pthread_cond_broadcast(&cond);
		pthread_mutex_unlock(&mutex);
		sleep(2);
	}
	return NULL;
}

void* readT() {

	while (1) {
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond, &mutex);
		printf("TID: %ld; ", pthread_self());
		printf("COUNTER: %d\n", counter);
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

#define NUM_OF_READING_THREADS 10
#define NUM_OF_WRITING_THREADS 1
int main() {
	pthread_t reading_threads[NUM_OF_READING_THREADS];
	pthread_t writing_threads[NUM_OF_WRITING_THREADS];
	size_t i;

	//printf("counter = %d\n", counter);
	//Инициализация мьютекса
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);

	for (i = 0; i < NUM_OF_WRITING_THREADS; i++) {
		pthread_create(&writing_threads[i], NULL, writeT, NULL);
	}
	for (i = 0; i < NUM_OF_READING_THREADS; i++) {
		pthread_create(&reading_threads[i], NULL, readT, NULL);
	}
	for (i = 0; i < NUM_OF_READING_THREADS; i++) {
		pthread_join(reading_threads[i], NULL);
	}
	for (i = 0; i < NUM_OF_WRITING_THREADS; i++) {
		pthread_join(writing_threads[i], NULL);
	}
	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);
	return 0;
}