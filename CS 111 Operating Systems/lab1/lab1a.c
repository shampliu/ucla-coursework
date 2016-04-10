#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/wait.h>

struct termios saved_attributes;

void reset_input_mode(void)
{
  tcsetattr (0, TCSANOW, &saved_attributes);
}

void set_input_mode(void)
{
  struct termios tattr;
  char *name;

  /* Make sure stdin is a terminal. */
  if (!isatty (0))
    {
      fprintf (stderr, "Not a terminal.\n");
      exit (EXIT_FAILURE);
    }

  /* Save the terminal attributes so we can restore them later. */
  tcgetattr (0, &saved_attributes);
  atexit (reset_input_mode);

  /* Set the terminal modes. */
  tcgetattr (0, &tattr);
  tattr.c_lflag &= ~(ICANON|ECHO); 	// clear ICANON and ECHO 
  tattr.c_cc[VMIN] = 1; 		// min bytes that must be available for input 
  tattr.c_cc[VTIME] = 0;		// how long to wait for input
  tcsetattr (0, TCSAFLUSH, &tattr);
}

void SIGPIPE_handler(int signum) {
}

void * thread_func(void *fd) {
  char buf[128];
  int size; 
  while ((size = read(*(int *) fd, buf, 128)) > 0) {
    if (buf[0] == '\004') {
      // restore terminal mode and exit 1
    }
    write(1, buf, size);
  }


}


int main(int argc, char **argv)
{
  static int shell;
  static struct option long_options[] = {
    { "shell", no_argument, &shell, 1 }
  };
  int o;
  int options_index = 0;

  while((o = getopt_long(argc, argv,"", long_options, &options_index)) != -1) {
  }

  if (shell) {
    int pipe_from_shell[2]; 		// shell is the child process
    int pipe_to_shell[2];
    pid_t pid = -1;

    if (pipe(pipe_from_shell) == -1) {
      fprintf(stderr, "ERROR: pipe() failed!\n");
      exit(1);
    }
    if (pipe(pipe_to_shell) == -1) {
      fprintf(stderr, "ERROR: pipe() failed!\n");
      exit(1);
    }
   
    pid = fork();
    if (pid == 0) { 			// shell process
      close(pipe_from_shell[0]);
      dup2(pipe_from_shell[1], 1); 
      close(pipe_to_shell[1]);
      dup2(pipe_to_shell[0], 0);
      execl("/bin/bash", "/bin/bash", NULL); 
    }
    else if (pid > 0) {			// terminal process
      // setup SIGPIPE handler using signal
      // signal(SIGPIPE, SIGPIPE_handler);
      // terminal doesn't use read end of pipe_to_shell
      close(pipe_to_shell[0]);
      // terminal doesn't use write end of pipe_from_shell
      close(pipe_from_shell[1]); 
      
      pthread_t t1;
      // int fd = (int) pipe_to_shell[1];
      int fd = (int) pipe_from_shell[0];
      int *arg = malloc(sizeof(*arg));
      *arg = fd; 

      if (pthread_create(&t1, NULL, thread_func, arg) != 0) {
         printf("ERROR: Unable to create new thread");
         exit(-1);
      }

      char buf[128];
      int size = 0;
      char crlf[2];
      crlf[0] = '\015';
      crlf[1] = '\012'; 

      while ((size = read(0, buf, 128)) > 0) {
        if (buf[0] == '\004')
          break;
        else if (buf[0] == '\015' || buf[0] == '\012') {
          write(1, crlf, 2);
          write(pipe_to_shell[1], crlf, 2);
        }
        else {
          write(1, buf, size);
          write(pipe_to_shell[1], buf, size);
        }
      } 
      // while ((size = read(0, buf, 128)) > 0) {
      //   write(pipe_to_shell[1], buf, size);
      //   size = read(pipe_from_shell[0], buf, 128);
      //   write(1, buf, size);
      // }


      // pthread_join(thread1, NULL);
      // pthread_exit(NULL);
      
      /*
      char buf[128];    
      int size = 0;
      while ((size = read(0, buf, 128)) > 0) {
        write(pipe_to_shell[1], buf, size);
        size = read(pipe_from_shell[0], buf, 128);
        write(1, buf, size);
      }
      */
    }
    else {
      fprintf(stderr, "ERROR: fork() failed!\n");
      exit(1);
    }
    

    exit(0);
  }

  char c;

  set_input_mode();

  char crlf[2];
  crlf[0] = '\015';
  crlf[1] = '\012'; 
  char buf[128];
  int size = 0;
  while ((size = read(0, buf, 128)) > 0) {
    if (buf[0] == '\004')
      break;
    else if (buf[0] == '\015' || buf[0] == '\012') {
      write(1, crlf, 2);
    }
    else 
      write(1, buf, size);
  } 

}
