// Homework #2 Chang Liu 704291153

#include <stdio.h>

/*
 * A. %esi holds x, %ebx holds n, %edi holds result and %edx holds mask
 * B. result and mask are initially -1 and 1 respectively  
 * C. mask != 0 
 * D. mask <<= n
 * E. result ^= (mask & x) 
 * F. (below)
 */

int loop(int x, int n) {
	int result = -1;
	int mask;
	for (mask = 1; mask != 0; mask <<= n) {
		result ^= x & mask;
	}
	return result;
}
