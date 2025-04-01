#define _POSIX_C_SOURCE 200809L
#include <arpa/inet.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 2020
#define UDP_SIZE 65507

int is_palindrome(const char *s) {
  int len = strlen(s);
  for (int i = 0; i < len / 2; i++) {
    if (tolower(s[i]) != tolower(s[len - i - 1])) {
      return false;
    }
  }
  return true;
}

int is_valid(const char *data, int len) {
  if (len == 0) {
    return true;
  }
  if (isspace(data[0]) || isspace(data[len - 1])) {
    return false;
  }

  for (int i = 0; i < len; i++) {
    if (!isalpha(data[i]) && data[i] != ' ') {
      return false;
    }
    if (data[i] == ' ' && data[i + 1] == ' ') {
      return false;
    }
  }
  return true;
}

void process_datagram(const char *data, int len, char *response) {
  char buffer[UDP_SIZE];
  strncpy(buffer, data, len);
  buffer[len] = '\0';

  char *word = strtok(buffer, " ");
  int total_words = 0, palindrome_count = 0;

  while (word) {
    total_words++;
    if (is_palindrome(word)) {
      palindrome_count++;
    }
    word = strtok(NULL, " ");
  }

  sprintf(response, "%d/%d", palindrome_count, total_words);
}

int main() {
  int svr_sock, res;
  char buffer[UDP_SIZE];
  buffer[0] = '\0';
  char response[UDP_SIZE];

  svr_sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (svr_sock == -1) {
    perror("Socket error");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in addr = {.sin_family = AF_INET,
                             .sin_addr = {.s_addr = htonl(INADDR_ANY)},
                             .sin_port = htons(PORT)};

  res = bind(svr_sock, (struct sockaddr *)&addr, sizeof(addr));
  if (res == -1) {
    perror("Bind error");
    close(svr_sock);
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in clt_addr;
  socklen_t len = sizeof(clt_addr);
  printf("Running on port %d...\n", PORT);

  while (true) {
    int n = recvfrom(svr_sock, buffer, UDP_SIZE, 0,
                     (struct sockaddr *)&clt_addr, &len);
    if (n < 0) {
      perror("Recvfrom error");
      continue;
    }

    if (buffer[0] == '\0') {
      printf("Received: ''\n");
      strcpy(response, "0/0");
      buffer[n] = '\0';
    } else if (!is_valid(buffer, n)) {
      printf("[ Received invalid data ]\n");
      strcpy(response, "ERROR");
      buffer[n] = '\0';
    } else {
      buffer[n] = '\0';
      printf("Received: '%s'\n", buffer);
      process_datagram(buffer, n, response);
    }

    sendto(svr_sock, response, strlen(response), 0,
           (struct sockaddr *)&clt_addr, len);
    printf("Sent: '%s'\n", response);
  }

  close(svr_sock);
  return 0;
}
