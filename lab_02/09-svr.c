#define _POSIX_C_SOURCE 200809L
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int svr_sock, res;
  int port = atoi(argv[1]);

  svr_sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (svr_sock == -1) {
    perror("Socket error");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in addr = {.sin_family = AF_INET,
                             .sin_addr = {.s_addr = htonl(INADDR_ANY)},
                             .sin_port = htons(port)};

  res = bind(svr_sock, (struct sockaddr *)&addr, sizeof(addr));
  if (res == -1) {
    perror("Bind error");
    close(svr_sock);
    exit(EXIT_FAILURE);
  }

  int size = 1024;
  unsigned char buffer[size];
  char message[] = "[UDP] Hello, world!\r\n";
  struct sockaddr_in clt_addr;

  printf("Running on port %d...\n", port);
  while (1) {
    unsigned int len = sizeof(clt_addr);
    int rcv = recvfrom(svr_sock, (char *)buffer, size, 0,
                       (struct sockaddr *)&clt_addr, &len);
    if (rcv == -1) {
      perror("Recvfrom error");
      exit(EXIT_FAILURE);
    }

    int snd = sendto(svr_sock, message, strlen(message), 0,
                     (struct sockaddr *)&clt_addr, len);
    if (snd == -1) {
      perror("Sendto error");
      exit(EXIT_FAILURE);
    }
  }

  close(svr_sock);
  return 0;
}
