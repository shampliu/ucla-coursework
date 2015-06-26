#include <stdio.h>

int main() {
	int max = 2;
	int val = 100; 
	printf("size of int is %i\n", sizeof(val)); 
	if (max - (int) sizeof(val) >= 0) {
		printf("ok\n");
	}
	return 0;


}
