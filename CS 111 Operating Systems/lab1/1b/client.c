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

void error(char *msg)
{
  perror(msg);
  exit(0);
}

struct termios saved_attributes;

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
    { "log" , required_argument, NULL, 'l' }
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
            // HANDLE LOG OPTION
      }
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

  // \015 is <cr>, \012 is <lf>
  while ((size = read(0, buf, 128)) > 0) {
    if (buf[0] == '\004')
      break;
    else if (buf[0] == '\015' || buf[0] == '\012') {
      write(1, "\015\012", 2);
      write(sockfd, "\015\012", 2);
    }
    else {
      write(1, buf, size);
      write(sockfd, buf, size);
    }
  }
  // fgets(buf,255,stdin);
  // n = write(sockfd,buf,strlen(buf));
  // if (n < 0) 
  //   error("ERROR writing to socket");
  // bzero(buf,256);
  // n = read(sockfd,buf,255);
  // if (n < 0) 
  //   error("ERROR reading from socket");
  // printf("%s\n",buf);
  return 0;
}