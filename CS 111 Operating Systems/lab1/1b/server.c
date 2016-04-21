#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <getopt.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>
#include <string.h>
#include <mcrypt.h>
#include <signal.h>
#include <sys/wait.h>

// globals
int newsockfd;
int sockfd; 
int encrypt; 
int ENCRYPTMSG = 420; 
int DECRYPTMSG = -420;
char key[100]; 
pid_t pid = -1;


// code from http://cboard.cprogramming.com/c-programming/127454-mcrypt-function.html
int msg_crypt(char *message,char *ukey,int length,int action)
{
  /*Needed variables*/
  MCRYPT td;
  int i,size;
  char *key;
  char *IV; /*Initialization Vector*/
  int keysize = 16; /*256 bits*/

  key = calloc(1,keysize);
  /*Open the mcrypt library for work*/
  td = mcrypt_module_open("twofish",NULL,"cfb",NULL);
  if(td == MCRYPT_FAILED)
  {
    return MCRYPT_FAILED;
  }
  size = mcrypt_enc_get_iv_size(td);
  IV = malloc(size);
  for(i=0;i<size;i++)
  {
    IV[i] = 1;
  }
  memmove(key,ukey,strlen(ukey));
  i = mcrypt_generic_init(td, key, keysize, IV);
  if(i < 0)
  {
    mcrypt_perror(i);
    return MCRYPT_FAILED;
  }
  if(action == ENCRYPTMSG)
  {
    i = mcrypt_generic(td,message,length);
    if(i != 0)
    {
      printf("Error encrypting\n");
    }
  }
  if(action == DECRYPTMSG)
  {
    mdecrypt_generic(td,message,length);
  }
  mcrypt_generic_end(td);
  return length;
}

// reads from shell and writes to socket back to client
void * thread_func(void *fd) {
  char buf[1];
  int size; 
  while ((size = read(*(int *) fd, buf, 1)) > 0) {
    if (encrypt) {
      msg_crypt(buf, key, size, ENCRYPTMSG);
    }

    write(newsockfd, buf, size);
  }

  // read error / EOF from shell
  close(newsockfd);
  close(sockfd);
  kill(pid, SIGKILL);
  exit(2);

}

void error(char *msg)
{
  perror(msg);
  exit(1);
}

void SIGPIPE_handler(int signum) {
  close(sockfd);
  close(newsockfd);
  kill(pid, SIGKILL);
  exit(2);
}

int main(int argc, char *argv[])
{
  int portno, clilen;
  char buf[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n;

  static struct option long_options[] = {
    { "port", required_argument, NULL, 'p' }, 
    { "encrypt" , no_argument, &encrypt, 1 }
  };

  int options_index = 0;
  int c;

  while((c = getopt_long(argc, argv, "", long_options, &options_index)) != -1) {
    switch(c) {
      case 'p':
        if (optarg) { 
          portno = atoi(optarg);
        }
    }
  }

  if (encrypt) {
    FILE * pFile;

    pFile = fopen("my.key", "r");
    if (pFile == NULL) perror ("Error opening file");
    else {
      if ( fgets (key, 100, pFile) != NULL ) {

      }
      fclose (pFile);
    }
  }

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) 
    error("ERROR opening socket");

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
    error("ERROR on binding");
  listen(sockfd,5);
  clilen = sizeof(cli_addr);
  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
  if (newsockfd < 0) 
    error("ERROR on accept");
  bzero(buf,256);

  int pipe_from_shell[2];     
  int pipe_to_shell[2];

  if (pipe(pipe_from_shell) == -1) {
    fprintf(stderr, "ERROR: pipe() failed!\n");
    exit(1);
  }
  if (pipe(pipe_to_shell) == -1) {
    fprintf(stderr, "ERROR: pipe() failed!\n");
    exit(1);
  }

  signal(SIGPIPE, SIGPIPE_handler);

  pid = fork();
  // shell process, child process
  if (pid == 0) {       
    close(pipe_from_shell[0]);
    dup2(pipe_from_shell[1], 1); 
    close(pipe_to_shell[1]);
    dup2(pipe_to_shell[0], 0);

    char *execvp_argv[2];
    char execvp_filename[] = "/bin/bash";
    execvp_argv[0] = execvp_filename;
    execvp_argv[1] = NULL;
    execvp(execvp_filename, execvp_argv); 
  }
  // terminal process, parent process
  // pid is pid of child process
  else if (pid > 0) {  
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

    int size = 0;

    // reads from client socket and writes
    // \015 is <cr>, \012 is <lf>
    while ((size = read(newsockfd, buf, 128)) > 0) {
      if (encrypt) {
        msg_crypt(buf, key, size, DECRYPTMSG);
      }

      if (buf[0] == '\004')
        break;
      else {
        write(pipe_to_shell[1], buf, size);
      }
    }

    // read error / EOF 
    close(newsockfd);
    close(sockfd); 
    kill(pid, SIGKILL);
    exit(1);
  }
  else {
    fprintf(stderr, "ERROR: fork() failed!\n");
    exit(1);
  }

  return 0; 
}