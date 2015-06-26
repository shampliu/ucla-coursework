#include <unistd.h>

int main() {
 char buf[1]; 
 while (read(0, &buf, 1) != 0) {
  write (1, &buf, 1);
 }
}
