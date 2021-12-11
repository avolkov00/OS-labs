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
static pthread_rwlock_t rw_lock;

void* writeT() {
	int local = 0;
	while (1) {
		pthread_rwlock_trywrlock(&rw_lock);
		counter+= 1;
		pthread_rwlock_unlock(&rw_lock);
		sleep(2);
	}
	return NULL;
}

void* readT() {
	sleep(0.5);
	while (1) {
		pthread_rwlock_tryrdlock(&rw_lock);
		printf("TID: %x; COUNTER: %d\n", pthread_self(), counter);
		pthread_rwlock_unlock(&rw_lock);
		sleep(2);
	}
	return NULL;
}

#define NUM_OF_READING_THREADS 10
#define NUM_OF_WRITING_THREADS 1
int main() {
	pthread_t reading_threads[NUM_OF_READING_THREADS];
	pthread_t writing_threads[NUM_OF_WRITING_THREADS];
	size_t i;

	pthread_rwlock_init(&rw_lock, NULL);

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
	pthread_rwlock_destroy(&rw_lock);
	return 0;
}