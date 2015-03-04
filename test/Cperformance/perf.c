/*
  Compile with:
    gcc -Wall -Werror -O3 -o perf perf.c

  Run with:
    time ./perf 1000000
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  if (argc != 2) {
    printf("Usage: perf <loopcnt>\n");
    return 1;
  }

  unsigned long long const loopcnt = atoll(argv[1]);

  unsigned long long i;
  for (i = 0; i < loopcnt; ++i) {
    int const fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1) {
      perror("socket");
      return 1;
    }
    close(fd);
  }

  return 0;
}
