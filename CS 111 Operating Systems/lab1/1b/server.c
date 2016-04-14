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
  exit(1);
}

int main(int argc, char *argv[])
{
  int sockfd, newsockfd, portno, clilen;
  char buf[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n;

  static struct option long_options[] = {
    { "port", required_argument, NULL, 'p' }
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
    }
  }

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) 
    error("ERROR opening socket");

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  if (bind(sockfd, (struct sockaddr *) &serv_addr,
    sizeof(serv_addr)) < 0) 
    error("ERROR on binding");
  listen(sockfd,5);
  clilen = sizeof(cli_addr);
  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
  if (newsockfd < 0) 
    error("ERROR on accept");
  bzero(buf,256);

  int size = 0;

  // \015 is <cr>, \012 is <lf>
  while ((size = read(newsockfd, buf, 128)) > 0) {
    if (buf[0] == '\004')
      break;
    // else if (buf[0] == '\015' || buf[0] == '\012') {
    //   write(1, "\015\012", 2);
    //   write(sockfd, "\015\012", 2);
    // }
    // else {
    //   write(1, buf, size);
    //   write(sockfd, buf, size);
    // }
    write(1, buf, size); 
  }

  // n = read(newsockfd,buf,255);
  // if (n < 0) error("ERROR reading from socket");
  // printf("Here is the message: %s\n",buf);
  // n = write(newsockfd,"I got your message",18);
  // if (n < 0) error("ERROR writing to socket");
  return 0; 
}