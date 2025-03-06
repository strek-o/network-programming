#define _POSIX_C_SOURCE 200809L
#include <arpa/inet.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <IP address> <port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int clt_sock, res;
  int port = atoi(argv[2]);
  ssize_t cnt;

  clt_sock = socket(AF_INET, SOCK_STREAM, 0);
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

  res = connect(clt_sock, (struct sockaddr *)&addr, sizeof(addr));
  if (res == -1) {
    perror("Connect error");
    close(clt_sock);
    exit(EXIT_FAILURE);
  }

  int size = 16;
  unsigned char buffer[size];
  while ((cnt = recv(clt_sock, buffer, size - 1, 0)) > 0) {
    buffer[cnt] = '\0';
    for (ssize_t i = 0; i < cnt; i++) {
      if (isprint(buffer[i]) || buffer[i] == '\n' || buffer[i] == '\r' ||
          buffer[i] == '\t') {
        putchar(buffer[i]);
      }
    }
  }

  if (cnt == -1) {
    perror("Recv error");
    close(clt_sock);
    exit(EXIT_FAILURE);
  }

  close(clt_sock);
  return 0;
}
