#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <sys/stat.h>
#include <getopt.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <mcrypt.h>

struct termios saved_attributes;
int log_fd = -1; 
int encrypt; 
int ENCRYPTMSG = 420; 
int DECRYPTMSG = -420;
char key[100];

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

// read from server
void * thread_func(void *fd) {
  char buf[128];
  int size; 
  while ((size = read(*(int *) fd, buf, 1)) > 0) {
    if (log_fd != -1) {
      dprintf(log_fd, "RECEIVED %d bytes: %s\n", size, buf); 
    }
    if (encrypt) {
      msg_crypt(buf, key, size, DECRYPTMSG);
    }

    write(1, buf, size);

  }
}

void error(char *msg)
{
  perror(msg);
  exit(0);
}

void reset_input_mode(void) {
  tcsetattr (0, TCSANOW, &saved_attributes);
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
  tattr.c_lflag &= ~(ICANON|ECHO);  // clear ICANON and ECHO 
  tattr.c_cc[VMIN] = 1;     // min bytes that must be available for input 
  tattr.c_cc[VTIME] = 0;    // how long to wait for input
  tcsetattr (0, TCSAFLUSH, &tattr);
}


int main(int argc, char *argv[])
{
  set_input_mode();

  int sockfd, portno, n;

  struct sockaddr_in serv_addr;
  struct hostent *server;

  char buf[256];

  static struct option long_options[] = {
    { "port", required_argument, NULL, 'p' },
    { "log" , required_argument, NULL, 'l' },
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
      break;
      case 'l':
      if (optarg) {
        log_fd = creat(optarg, 0666);
        if (log_fd >= 0) {
        
        }
        else {
          fprintf(stderr, "ERROR: Log file could not be created\n");
          perror("");
          exit(2);
        }
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

  char hostname[] = "localhost";
  server = gethostbyname(hostname);

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) 
    error("ERROR opening socket");
  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, 
    (char *)&serv_addr.sin_addr.s_addr,
    server->h_length);
  serv_addr.sin_port = htons(portno);
  if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
    error("ERROR connecting");

  bzero(buf,256);

  int size = 0;

  pthread_t t1;
  int fd = (int) sockfd;
  int *arg = malloc(sizeof(*arg));
  *arg = fd; 

  if (pthread_create(&t1, NULL, thread_func, arg) != 0) {
    printf("ERROR: Unable to create new thread\n");
    exit(-1);
  }

  // \015 is <cr>, \012 is <lf>
  while ((size = read(0, buf, 256)) > 0) {
    if (buf[0] == '\004') {
      close(sockfd);
      exit(0);
    }
    else if (buf[0] == '\015' || buf[0] == '\012') {
      write(1, "\015\012", 2);
      if (encrypt) {
        char c[1] = "\012"; 
        msg_crypt(c, key, 1, ENCRYPTMSG);
        write(sockfd, c, 1);
      }
      else {
        write(sockfd, "\012", 1);
      }
    }
    else {
      write(1, buf, size);
      if (encrypt) {
        msg_crypt(buf, key, size, ENCRYPTMSG);
      }
      if (log_fd != -1) {
        dprintf(log_fd, "SENT %d bytes: %s\n", size, buf); 
      }
      write(sockfd, buf, size);

    }
  }

  // read error / EOF
  close(sockfd);
  exit(1);
}