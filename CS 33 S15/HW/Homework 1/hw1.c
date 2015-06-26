#include <stdio.h>
#include <limits.h>

int main() {
	int x = INT_MAX; 
	int y = -1; 

	unsigned ux = (unsigned) x;
	unsigned uy = (unsigned) y;

	if ((ux - uy) == -(unsigned)(y-x)) {
		printf("yes");
	}
	else {
		printf("no");
	}
}
