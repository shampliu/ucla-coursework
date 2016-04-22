#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

int NTHREADS = 1;
int NITERATIONS = 1;

void add(long long *pointer, long long value) {
	long long sum = *pointer + value;
	*pointer = sum;
}

void *thread_func(void *arg) {
	for (int i = 0; i < NITERATIONS; i++) {
		add((long long *) arg, 1);
	}
	for (int i = 0; i < NITERATIONS; i++) {
		add((long long *) arg, -1);
	}
	pthread_exit(NULL);

}

int main(int argc, char *argv[]) {
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);

	static struct option long_options[] = {
    { "threads", required_argument, NULL, 't' },
    { "iterations" , required_argument, NULL, 'i' }
  };

  int options_index = 0;
  int c;

  while((c = getopt_long(argc, argv, "", long_options, &options_index)) != -1) {
    switch(c) {
      case 't':
      if (optarg) { 
      	NTHREADS = atoi(optarg); 
      }
      break;
      case 'i':
      if (optarg) {
      	NITERATIONS = atoi(optarg);
      }
    }
  }

  long long *arg = malloc(sizeof(*arg));
  *arg = 0;

  pthread_t tid[NTHREADS];

  for (int i = 0; i < NTHREADS; i++) {

  	if (pthread_create(&(tid[i]), NULL, &thread_func, arg) != 0) {
  		printf("ERROR: Thread could not be created!\n");
  	}
  }

  for (int i = 0; i < NTHREADS; i++) {
  	pthread_join(tid[i], NULL);
  }

  printf("%d threads x %d iterations x (add + subtract) = %d operations\n", NTHREADS, NITERATIONS, NTHREADS * NITERATIONS, *arg);

  clock_gettime(CLOCK_MONOTONIC, &end);

  long long elapsed_time = ((end.tv_sec - start.tv_sec) * 1000000000) + (end.tv_nsec - start.tv_nsec);
  // double t = end.tv_nsec - start.tv_nsec;

  int fail = 0; 
  if (*arg != 0) {
  	fail = 1; 
  	printf("ERROR: final count = %d\n", *arg); 
  }

  printf("elapsed time: %ldns\n", elapsed_time); 


  if (fail) exit(1);
  else exit(0);





}