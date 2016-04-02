#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

void SIGSEGV_handler(int signum) {
  fprintf(stderr, "ERROR: Segmentation fault\n");
  exit(3);
}

int main(int argc, char **argv) {
  int segfault = 0;
  int catch = 0;
  const char *options = "abc:d"; 
  static struct option long_options[] = {
    { "input", required_argument, NULL, 'i' },
    { "output", required_argument, NULL, 'o' },
    { "segfault", no_argument, 0, 's' },
    { "catch", no_argument, 0, 'c' }
  };
  int options_index = 0;
  int c;

  while((c = getopt_long(argc, argv, options, long_options, &options_index)) != -1) {
  
    switch(c) {
      case 'i':
        if (optarg) { 
          int ifd = open(optarg, O_RDONLY);
          if (ifd >= 0) {
            close(0);
            dup(ifd);
            close(ifd);
          }
          else { 
            fprintf(stderr, "ERROR: Input file could not be opened\n");
            perror("");
            exit(1);
          } 
        }
        break; 
      case 'o':
        if (optarg) {
          int ofd = creat(optarg, 0666);
          if (ofd >= 0) {
      
            close(1);
            dup(ofd);
            close(ofd);
          }
          else {
            fprintf(stderr, "ERROR: Output file could not be created\n");
            perror("");
            exit(2);
          }
        }
        break;
      case 's': {
        segfault = 1;
        break;
      }
      case 'c':
        catch = 1;
	break;
    }
    
  }  
  
  if (catch) {
    signal(SIGSEGV, SIGSEGV_handler);
  }
  if (segfault) {
     int *a = NULL;
     int b = *a; 
  }

  char buf[1];
  int r; 
  while (1) {
    r = read(0, buf, sizeof(buf));
    if (r <= 0) {
      break;
    }
    write(1, buf, sizeof(buf)); 
  }
  if (r < 0) {

  }
  exit(0); 
}
