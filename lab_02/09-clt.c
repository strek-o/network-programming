#define _POSIX_C_SOURCE 200809L
#include <arpa/inet.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <IP address> <port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int clt_sock, res;
  int port = atoi(argv[2]);

  clt_sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (clt_sock == -1) {
    perror("Socket error");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in addr = {
      .sin_family = AF_INET, .sin_addr = {0}, .sin_port = htons(port)};

  if (inet_pton(AF_INET, argv[1], &addr.sin_addr) <= 0) {
    close(clt_sock);
    exit(EXIT_FAILURE);
  }

  res = sendto(clt_sock, "", 0, 0, (struct sockaddr *)&addr, sizeof(addr));
  if (res == -1) {
    perror("Sendto error");
    close(clt_sock);
    exit(EXIT_FAILURE);
  }

  int size = 1024;
  unsigned char buffer[size];
  unsigned int len = sizeof(addr);

  int rcv = recvfrom(clt_sock, buffer, size, 0, (struct sockaddr *)&addr, &len);
  if (rcv == -1) {
    perror("Recvfrom error");
    exit(EXIT_FAILURE);
  }

  int idx = 0;
  for (ssize_t i = 0; i < size; i++) {
    if ((buffer[i] < 32 || buffer[i] > 126) && buffer[i] != '\r' &&
        buffer[i] != '\n') {
      idx = i;
      break;
    }
  }

  for (ssize_t i = 0; i < idx; i++) {
    putchar(buffer[i]);
  }

  close(clt_sock);
  return 0;
}
