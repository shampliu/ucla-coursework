/* 
 * Chang Liu
 * CS 33 Homework 5
 */

// creates and reaps n joinable peer threads, where n is a command line argument

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread(void *vargp);

int main (int argc, char *argv[])
{
 int numThreads = atoi(argv[1]);
 // printf("%d\n", numThreads); 
 pthread_t tid[numThreads]; 
 for (int i = 0; i < numThreads; i++) {
  pthread_create(&tid[i], NULL, thread, NULL);
  pthread_join(tid[i], NULL);
 }
 // pthread_join(tid[0], NULL);
 exit(0);
}

void *thread(void *vargp)
{
 printf("Hello, world!\n");
 return NULL;
}


