#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>	

#include "SortedList.c"
#define KEY_SIZE 5

int NTHREADS = 1;
int NITERATIONS = 1;
int NLISTS = 1; 

int opt_yield = 0;
char opt_sync; 

pthread_t *threads;
pthread_mutex_t *LIST_MUTEXES;
int *LIST_SPIN_LOCKS; 

SortedListElement_t *LIST_HEADS; 
SortedListElement_t *LIST_ELEMS; 

int hash(const char *key, int len) {
  int sum = 0;
  for (int i = 0; i < len; i++) {
    sum += key[i];
  }
  return sum % NLISTS;
}

void *thread_func(void *arg) {

	int *ind = (int *) arg; 
	int i = 0;
 
	int start_ind = (*ind) * NITERATIONS; 
	int end_ind = start_ind + NITERATIONS; 

	int bucket; 


	// insert
	for (i = start_ind; i < end_ind; i++) {
		bucket = hash(LIST_ELEMS[i].key, KEY_SIZE);

		switch(opt_sync) {
			// PTHREAD MUTEX
			case 'm': {
				pthread_mutex_lock(&LIST_MUTEXES[bucket]);
				SortedList_insert(&LIST_HEADS[bucket], &LIST_ELEMS[i]);
				pthread_mutex_unlock(&LIST_MUTEXES[bucket]);
				break;
			}
			// SPIN LOCK
			case 's': {
				while (__sync_lock_test_and_set(&LIST_SPIN_LOCKS[bucket], 1)) { }
				SortedList_insert(&LIST_HEADS[bucket], &LIST_ELEMS[i]);
				__sync_lock_release(&LIST_SPIN_LOCKS[bucket]);
				break;
			}
			// NO LOCK	
			default: 
				SortedList_insert(&LIST_HEADS[bucket], &LIST_ELEMS[i]);
				break;
		}
	}

	// length
	int len = 0;
	for (i = 0; i < NLISTS; i++) {
		switch(opt_sync) {
			// PTHREAD MUTEX
			case 'm': {
				pthread_mutex_lock(&LIST_MUTEXES[i]);
				len += SortedList_length(&LIST_HEADS[i]);
				pthread_mutex_unlock(&LIST_MUTEXES[i]);
				break;
			}
			// SPIN LOCK
			case 's': {
				while (__sync_lock_test_and_set(&LIST_SPIN_LOCKS[i], 1)) { }
				len += SortedList_length(&LIST_HEADS[i]);
				__sync_lock_release(&LIST_SPIN_LOCKS[i]);
				break;
			}
			// NO LOCK	
			default: 
				len += SortedList_length(&LIST_HEADS[i]);
				break;
		}
	}

	SortedListElement_t *elem;
	// lookup
	for (i = start_ind; i < end_ind; i++) {
		bucket = hash(LIST_ELEMS[i].key, KEY_SIZE);
		switch(opt_sync) {
			// PTHREAD MUTEX
			case 'm': {
				pthread_mutex_lock(&LIST_MUTEXES[bucket]);
				elem = SortedList_lookup(&LIST_HEADS[bucket], LIST_ELEMS[i].key);
				SortedList_delete(elem);
				pthread_mutex_unlock(&LIST_MUTEXES[bucket]);
				break;
			}
			// SPIN LOCK
			case 's': {
				while (__sync_lock_test_and_set(&LIST_SPIN_LOCKS[bucket], 1)) { }
				elem = SortedList_lookup(&LIST_HEADS[bucket], LIST_ELEMS[i].key);
				SortedList_delete(elem);
				__sync_lock_release(&LIST_SPIN_LOCKS[bucket]);
				break;
			}
			// NO LOCK	
			default: 
				elem = SortedList_lookup(&LIST_HEADS[bucket], LIST_ELEMS[i].key);
				SortedList_delete(elem);
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
    { "yield", required_argument, NULL, 'y' },
    { "sync", required_argument, NULL, 's' },
    { "lists", required_argument, NULL, 'l' }
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
	    case 'y':
	    	if (optarg) {
	    		for (int i=0; optarg[i] != '\0'; ++i) {
	    			if (optarg[i] == 'i') { opt_yield |= INSERT_YIELD; }
	    			else if (optarg[i] == 'd') { opt_yield |= DELETE_YIELD; }
	    			else if (optarg[i] == 's') { opt_yield |= SEARCH_YIELD; }
	    			else { }
	    		}
	    	}
	    	break;
	    case 's': 
	    	opt_sync = *optarg;
	    	break;
	    case 'l':
	    	if (optarg) {
	      	NLISTS = atoi(optarg);
	      }
	      break;


    }
  }
  threads = (pthread_t *)malloc(sizeof(pthread_t) * NTHREADS);

  LIST_HEADS = (SortedListElement_t *) malloc(NLISTS * sizeof(SortedListElement_t));
  for (int i = 0; i < NLISTS; i++) {
    LIST_HEADS[i].prev = &LIST_HEADS[i];
    LIST_HEADS[i].next = &LIST_HEADS[i];
    LIST_HEADS[i].key = NULL;
  }

  int NELEMS = NITERATIONS * NTHREADS;
  LIST_ELEMS = (SortedListElement_t *)malloc(sizeof(SortedListElement_t) * NELEMS);

  const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

 	for (int i = 0; i < NELEMS; i++) {
 		char *str = (char *)malloc(sizeof(char) * (KEY_SIZE + 1));
    str[KEY_SIZE] = '\0';  
    for (int j = 0; j < KEY_SIZE; j++) {
      int ind = rand() % (int) (sizeof(charset) - 1);
      str[j] = charset[ind]; 
    }
    LIST_ELEMS[i].key = str; 
  }

  clock_gettime(CLOCK_MONOTONIC, &start);

  if (opt_sync == 'm') {
    LIST_MUTEXES = (pthread_mutex_t *) malloc(NLISTS * sizeof(pthread_mutex_t));
    for (int i = 0; i < NLISTS; i++) {
      pthread_mutex_init(&LIST_MUTEXES[i], NULL);; 
    }
  }

  if (opt_sync == 's') {
    LIST_SPIN_LOCKS = (int *) malloc(NLISTS * sizeof(int));
    for (int i = 0; i < NLISTS; i++) {
      LIST_SPIN_LOCKS[i] = 0;
    }
  }

  int r; 

  for (int j = 0; j < NTHREADS; j++) {

  	int *arg = malloc(sizeof(*arg));
  	*arg = j; 
  	r = pthread_create(&(threads[j]), NULL, &thread_func, arg); 
  	if (r < 0) exit(2);
  }

  for (int k = 0; k < NTHREADS; k++) {
  	r = pthread_join(threads[k], NULL); 
  	if (r < 0) exit(2);
  }

  clock_gettime(CLOCK_MONOTONIC, &end);

  long long OPERATIONS = NTHREADS * NITERATIONS * 2;
  printf("%d threads x %d iterations x (insert + lookup/delete) = %d operations\n", NTHREADS, NITERATIONS, OPERATIONS);

  long long ELAPSED_TIME = ((end.tv_sec - start.tv_sec) * 1000000000) + (end.tv_nsec - start.tv_nsec);

  int fail = 0; 

  int check = 0; 
  for (int i = 0; i < NLISTS; i++) {
  	check = SortedList_length(&LIST_HEADS[i]);
  	if (check != 0) {
  		fail = 1; 
  		fprintf(stderr, "ERROR: final length of sublist %i = %d\n", i, check); 
  	}
  }

  printf("elapsed time: %ldns\n", ELAPSED_TIME); 
  printf("per operation: %fns\n", (float) (ELAPSED_TIME / OPERATIONS) / ((float) NELEMS / NLISTS));


  fail ? exit(1) : exit(0);
}