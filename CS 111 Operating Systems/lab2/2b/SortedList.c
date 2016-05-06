#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "SortedList.h"

void SortedList_insert(SortedList_t *list, SortedListElement_t *element) {
	SortedListElement_t *previous = list;
	SortedListElement_t *next = list->next;

	while(next != list) {
		// increasing order
		if (strcmp(element->key, next->key) <= 0) 
			break;
		previous = next;
		next = next->next;
		
	}

	if (opt_yield & INSERT_YIELD)
		pthread_yield();

	element->prev = previous;
	element->next = next;
	previous->next = element;
	next->prev = element;
	
}

int SortedList_length(SortedList_t *list) {
	int len = 0; 
	SortedListElement_t *next = list->next; 
	
	while (next != list) {
		if (next->next->prev != next || next->prev->next != next) {
      return -1;
    }
    next = next->next;
    len++;
	}

	if (opt_yield & SEARCH_YIELD) { pthread_yield(); }

	return len; 
}

SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key) {
  SortedListElement_t *next = list->next;

  while (next != list) {
    if (strcmp(next->key, key) == 0) {
      if (opt_yield & SEARCH_YIELD) {
        pthread_yield();
      }
      return next;
    }
    next = next->next;
  }

  if (opt_yield & SEARCH_YIELD) { pthread_yield(); }

  return NULL;
}

int SortedList_delete(SortedListElement_t *element) {
  if (element->next->prev != element || element->next->prev != element) {
    return 1;
  }
  
  if (element->next != NULL) {
  	element->next->prev = element->prev;
  }

  if (opt_yield & DELETE_YIELD) { pthread_yield(); }

  if (element->prev != NULL) {
  	element->prev->next = element->next;
  }
  return 0;
}
