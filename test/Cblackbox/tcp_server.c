#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

struct sockaddr_in serv_addr;
struct sockaddr_in cli_addr;

static int open_socket() {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    perror("ERROR opening socket");
    exit(1);
  }

  /* Initialize socket structure */
  bzero((char *)&serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(0);

  /* Now bind the host address using bind() call.*/
  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("ERROR on binding");
    exit(1);
  }

  return sockfd;
}

static int listen_and_accept(int sockfd) {
  listen(sockfd, 5);
  socklen_t clilen = sizeof(cli_addr);

  /* Accept actual connection from the client */
  int newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
  if (newsockfd < 0) {
    perror("ERROR on accept");
    exit(1);
  }
  return newsockfd;
}

int main() {

  int server_socket = 0;
  int accepted_socket = 0;

  while (!feof(stdin)) {
    int const c = getchar();
    if (c == EOF) {
      break;
    }
    switch (c) {
    case 'o': {
      struct sockaddr_in loc_serv_addr;
      socklen_t addrlen = sizeof(loc_serv_addr);
      server_socket = open_socket();
      getsockname(server_socket, (struct sockaddr *)&loc_serv_addr, &addrlen);
      printf("%d\n", ntohs(loc_serv_addr.sin_port));
    } break;
    case 'a': {
      accepted_socket = listen_and_accept(server_socket);
      printf("1\n");
    } break;
    case 'r': {
      char buffer[256];
      bzero(buffer, 256);
      int const n = read(accepted_socket, buffer, 255);
      if (n < 0) {
        perror("ERROR reading from socket");
        exit(1);
      }
      printf("%s\n", buffer);
    } break;
    case 'w': {
      char text[256];
      char const *fs = fgets(text, 256, stdin);
      if (fs == 0) {
        perror("text");
        exit(1);
      }
      size_t const cnt = strlen(text) - 1;
      ssize_t const w = write(accepted_socket, text, cnt);
      if (w == -1) {
        perror("write");
        exit(1);
      }
    } break;
    case 'l': {
       close(accepted_socket);
    } break;
    case 'p':
      printf("pong\n");
      break;
    case '\n':
      break;
    default:
      printf("Unknown command [%c]\n", c);
      abort();
    }
  }

  return 0;
}
