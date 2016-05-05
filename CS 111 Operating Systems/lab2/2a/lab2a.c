#define _GNU_SOURCE
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

long long COUNTER = 0;

typedef enum {
	NO_LOCK, 
	PTHREAD_MUTEX,
	SPIN_LOCK,
	CMP_AND_SWAP
} lock_type; 

lock_type WHICH_LOCK = NO_LOCK; 

// locks 
pthread_mutex_t MY_MUTEX; 
volatile int MY_SPIN_LOCK = 0; 

int opt_yield;

void add(long long *pointer, long long value) {
	long long sum = *pointer + value;
	if (opt_yield)
		pthread_yield();
	*pointer = sum;
}

void *thread_func(void *arg) {
	long long prev = 0;
	long long sum; 

	for (int i = 0; i < NITERATIONS; i++) {
		switch(WHICH_LOCK) {
			case NO_LOCK: {
				add(&COUNTER, 1);
				break;
			}	
			case PTHREAD_MUTEX: {
				pthread_mutex_lock(&MY_MUTEX);
				add(&COUNTER, 1);
				pthread_mutex_unlock(&MY_MUTEX);
				break;
			}
			case SPIN_LOCK: {
				while (__sync_lock_test_and_set(&MY_SPIN_LOCK, 1)) { }
				add(&COUNTER, 1);
				__sync_lock_release(&MY_SPIN_LOCK);
				break;
			}
			case CMP_AND_SWAP: {

				do {
					prev = COUNTER;
					sum = prev + 1;						
					if (opt_yield) {
						pthread_yield();
					}
				} while(__sync_val_compare_and_swap(&COUNTER, prev, sum) != prev);
				break;
			}		
			default: 
				break;
		}

	}

	for (int j = 0; j < NITERATIONS; j++) {
		switch(WHICH_LOCK) {
			case NO_LOCK: {
				add(&COUNTER, -1);
				break;
			}	
			case PTHREAD_MUTEX: {
				pthread_mutex_lock(&MY_MUTEX);
				add(&COUNTER, -1);
				pthread_mutex_unlock(&MY_MUTEX);
				break;
			}
			case SPIN_LOCK: {
				while (__sync_lock_test_and_set(&MY_SPIN_LOCK, 1)) { }
				add(&COUNTER, -1);
				__sync_lock_release(&MY_SPIN_LOCK);
				break;
			}
			case CMP_AND_SWAP: {

				do {
					prev = COUNTER;
					sum = prev - 1;						
					if (opt_yield) {
						pthread_yield();
					}
				} while(__sync_val_compare_and_swap(&COUNTER, prev, sum) != prev);
				break;
			}		
			default: 
				break;
		}

	}

	return NULL;
}

int main(int argc, char *argv[]) {
	struct timespec start, end;

	static struct option long_options[] = {
    { "threads", required_argument, NULL, 't' },
    { "iterations" , required_argument, NULL, 'i' },
    { "yield", no_argument, &opt_yield, 1 },
    { "sync", required_argument, NULL, 's' }
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
	      break;
	    case 's': 
	    	if (*optarg == 'm') {
	    		WHICH_LOCK = PTHREAD_MUTEX;
	    	}
	    	else if (*optarg == 's') {
	    		WHICH_LOCK = SPIN_LOCK;
	    	}
	    	else if (*optarg == 'c') {
	    		WHICH_LOCK = CMP_AND_SWAP;
	    	}
	    	else {
	    		// invalid arg
	    	}

    }
  }

  pthread_t tid[NTHREADS];

  clock_gettime(CLOCK_MONOTONIC, &start);

  for (int i = 0; i < NTHREADS; i++) {

  	if (pthread_create(&(tid[i]), NULL, &thread_func, NULL) != 0) {
  		printf("ERROR: Thread could not be created!\n");
  	}
  }

  for (int i = 0; i < NTHREADS; i++) {
  	pthread_join(tid[i], NULL);
  }

  clock_gettime(CLOCK_MONOTONIC, &end);

  long long OPERATIONS = NTHREADS * NITERATIONS;
  printf("%d threads x %d iterations x (add + subtract) = %d operations\n", NTHREADS, NITERATIONS, OPERATIONS, COUNTER);

  long long ELAPSED_TIME = ((end.tv_sec - start.tv_sec) * 1000000000) + (end.tv_nsec - start.tv_nsec);

  int fail = 0; 
  if (COUNTER != 0) {
  	fail = 1; 
  	printf("ERROR: final count = %d\n", COUNTER); 
  }

  printf("elapsed time: %ldns\n", ELAPSED_TIME); 
  printf("per operation: %dns\n", ELAPSED_TIME / OPERATIONS);


  if (fail) exit(1);
  else exit(0);





}