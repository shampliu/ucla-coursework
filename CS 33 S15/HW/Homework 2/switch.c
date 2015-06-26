// Homework #2 Chang Liu 704291153

#include <stdio.h>

int switch_prob(int x, int n) {
	int result = x;
	switch(n) {
	case 0x32:
	case 0x34:
		result <<= 2; 
		break;
	case 0x35:
		result >>= 2;
		break;
	case 0x36:
		result *= 3;
	case 0x37:
		result *= result; 
	case 0x33: 
	default:
		result += 10; 
	}

	return result;
}
