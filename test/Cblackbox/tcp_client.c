#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {

  int client_port = 0;
  int client_socket = 0;

  while (!feof(stdin)) {
    int const c = getchar();
    if (c == EOF) {
      break;
    }
    switch (c) {
    case 'c': {
      char str_port[256];
      char const *fs = fgets(str_port, 256, stdin);
      if (fs == 0) {
        perror("fgets port number");
        exit(1);
      }
      client_port = atoi(str_port);
      // printf("%d\n", client_port);
      client_socket = socket(AF_INET, SOCK_STREAM, 0);
      struct sockaddr_in servaddr;

      bzero(&servaddr, sizeof(servaddr));
      servaddr.sin_family = AF_INET;
      servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
      servaddr.sin_port = htons(client_port);

      connect(client_socket, (struct sockaddr *)&servaddr, sizeof(servaddr));
      printf("1\n");
    } break;
    case 'w': {
      char text[256];
      char const *fs = fgets(text, 256, stdin);
      if (fs == 0) {
        perror("text");
        exit(1);
      }
      size_t const cnt = strlen(text) - 1;
      ssize_t const w = write(client_socket, text, cnt);
      if (w == -1) {
        perror("write");
        exit(1);
      }
    } break;
    case 'r': {
      char buffer[256];
      bzero(buffer, 256);
      int const n = read(client_socket, buffer, 255);
      if (n < 0) {
        perror("ERROR reading from socket");
        exit(1);
      }
      printf("%s\n", buffer);
    } break;
    case 'l': {
       close(client_socket);
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
