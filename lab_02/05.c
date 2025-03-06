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

  int svr_sock, clt_sock, res;
  int port = atoi(argv[1]);

  svr_sock = socket(AF_INET, SOCK_STREAM, 0);
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

  res = listen(svr_sock, 10);
  if (res == -1) {
    perror("Listen error");
    close(svr_sock);
    exit(EXIT_FAILURE);
  }

  printf("Listening on port %d...\n", port);
  while (1) {
    clt_sock = accept(svr_sock, NULL, NULL);
    if (clt_sock == -1) {
      perror("Accept error");
      exit(EXIT_FAILURE);
    }

    char message[] = "[TCP] Hello, world!\r\n";
    send(clt_sock, message, strlen(message), 0);
    close(clt_sock);
  }

  close(svr_sock);
  return 0;
}
