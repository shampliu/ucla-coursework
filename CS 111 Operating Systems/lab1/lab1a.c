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
pid_t CHILD_PID; 

void reset_input_mode(void) {
  // printf("RESETTING INPUT MODES\n");
  tcsetattr (0, TCSANOW, &saved_attributes);
}

void shell_exit(void) {
  // printf("SHELL EXIT FUNCTION IS CALLED\n");
  int chld_state; 
  pid_t rc_pid = waitpid( CHILD_PID, &chld_state, 0);
  if (rc_pid > 0)
  {
    if (WIFEXITED(chld_state)) {
      printf("Child exited with RC=%d\n", WEXITSTATUS(chld_state));
    }
    else {
      printf("Child process aborted abnormally\n"); 
    }
  }

}

void set_input_mode(void) {
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
  exit(1);
}

void SIGINT_handler(int signum) {}

void SIGHUP_handler(int signum) {
  exit(1);
}

// thread reads input from shell pipe and writes to stdout
void * thread_func(void *fd) {
  char buf[128];
  int size; 
  while ((size = read(*(int *) fd, buf, 128)) > 0) {
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

  set_input_mode();

  while((o = getopt_long(argc, argv,"", long_options, &options_index)) != -1) {
  }

  if (shell) {

    int pipe_from_shell[2]; 		
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

    signal(SIGINT, SIGINT_handler);
    signal(SIGHUP, SIGHUP_handler);
    signal(SIGPIPE, SIGPIPE_handler);
   
    pid = fork();
    // shell process, child process
    if (pid == 0) { 			
      

      close(pipe_from_shell[0]);
      dup2(pipe_from_shell[1], 1); 
      close(pipe_to_shell[1]);
      dup2(pipe_to_shell[0], 0);
      // execl("/bin/bash", "/bin/bash", NULL); 
      char *execvp_argv[2];
      char execvp_filename[] = "/bin/bash";
      execvp_argv[0] = execvp_filename;
      execvp_argv[1] = NULL;
      execvp(execvp_filename, execvp_argv); 
    }
    // terminal process, parent process
    // pid is pid of child process
    else if (pid > 0) {	
      CHILD_PID = pid; 	
      atexit(shell_exit);	

      // terminal doesn't use read end of pipe_to_shell
      close(pipe_to_shell[0]);
      // terminal doesn't use write end of pipe_from_shell
      close(pipe_from_shell[1]); 
      
      pthread_t t1;
      int fd = (int) pipe_from_shell[0];
      int *arg = malloc(sizeof(*arg));
      *arg = fd; 

      if (pthread_create(&t1, NULL, thread_func, arg) != 0) {
         printf("ERROR: Unable to create new thread\n");
         exit(-1);
      }

      char buf[128];
      int size = 0;

      // thread reads from keyboard, echoes to stdout and forwards to shell
      while ((size = read(0, buf, 128)) > 0) {
        // CTRL-C
        if (buf[0] == '\003') {
          kill(pid, SIGINT);
        }
        // CTRL-D
        else if (buf[0] == '\004') {
          close(pipe_to_shell[1]);
          close(pipe_from_shell[0]); 
          // sleep(2);
          kill(pid, SIGHUP);
          exit(0);
        }
        // <cr> or <lf>
        else if (buf[0] == '\015' || buf[0] == '\012') {
          write(1, "\015\012", 2);
          write(pipe_to_shell[1], "\012", 1); // goes to shell only as <lf>
        }
        else {
          write(1, buf, size);
          write(pipe_to_shell[1], buf, size);
        }
      } 
    }
    else {
      fprintf(stderr, "ERROR: fork() failed!\n");
      exit(1);
    }

    exit(0);
  }

  char buf[128];
  int size = 0;

  // \015 is <cr>, \012 is <lf>
  while ((size = read(0, buf, 128)) > 0) {
    if (buf[0] == '\004')
      break;
    else if (buf[0] == '\015' || buf[0] == '\012') {
      write(1, "\015\012", 2);
    }
    else 
      write(1, buf, size);
  } 
}
