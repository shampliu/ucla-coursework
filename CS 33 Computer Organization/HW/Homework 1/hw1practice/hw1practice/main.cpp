#include <stdio.h>
#include <limits.h>

int main ()
{
    int x = INT_MIN;
    int y = 1;
    
//    if (~x + ~y + 1 == ~(x+y)) {
//        printf("yes");
//    }
//    else {
//        printf("no");
//    }
    
    
    unsigned ux = (unsigned) x;
    unsigned uy = (unsigned) y;
    
    
//    printf("%u",uy-ux);
//    if ((ux - uy) == -(unsigned)(y-x)) {
//        printf("yes");
//    }
//    else {
//        printf("no");
//    }
    printf("x = %d\n", x);
    int ix = -x;
    printf("-x = %d\n", ix);
    if ((x < y) == (-x > -y)) {
        
        printf("yes");
    }
    else {
        printf("no");
    }
    
   
}