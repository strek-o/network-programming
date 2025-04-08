#include <arpa/inet.h>
#include <cctype>
#include <chrono>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <vector>

#define PORT 2020
#define BUFF_SIZE 1024

using namespace std;

bool is_palindrome(const char *word, size_t len) {
  for (size_t i = 0, j = len - 1; i < j; ++i, --j) {
    if (word[i] != word[j])
      return false;
  }
  return true;
}

pair<int, int> analyze_line(char *buffer, size_t len) {
  if (len == 0)
    return {0, 0};

  while (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r'))
    --len;

  if (len == 0)
    return {0, 0};

  if (isspace(buffer[0]) || isspace(buffer[len - 1]))
    return {-1, -1};

  int palindromes = 0, words = 0;
  char word[BUFF_SIZE];
  size_t word_len = 0;

  for (size_t i = 0; i <= len; ++i) {
    char c = (i < len) ? buffer[i] : ' ';

    if (c > 127)
      return {-1, -1};

    if (isalpha(c)) {
      word[word_len++] = toupper(c);
    } else if (c == ' ' || i == len) {
      if (word_len == 0)
        return {-1, -1};

      word[word_len] = '\0';
      if (is_palindrome(word, word_len))
        ++palindromes;
      ++words;
      word_len = 0;
    } else {
      return {-1, -1};
    }
  }

  return {palindromes, words};
}

void handle_client(int client_sock) {
  char buffer[BUFF_SIZE];
  ssize_t bytes_read;
  size_t data_len = 0;

  while ((bytes_read =
              read(client_sock, buffer + data_len, BUFF_SIZE - data_len)) > 0) {
    data_len += bytes_read;

    size_t line_end = 0;
    while (line_end + 1 < data_len) {
      if (buffer[line_end] == '\r' && buffer[line_end + 1] == '\n') {
        char line[BUFF_SIZE];
        memcpy(line, buffer, line_end + 2);
        auto [p, w] = analyze_line(line, line_end + 2);

        string response = (p == -1 || w == -1)
                              ? "ERROR\r\n"
                              : to_string(p) + "/" + to_string(w) + "\r\n";

        write(client_sock, response.c_str(), response.size());

        memmove(buffer, buffer + line_end + 2, data_len - (line_end + 2));
        data_len -= (line_end + 2);
        line_end = 0;
      } else {
        ++line_end;
      }
    }

    this_thread::sleep_for(chrono::milliseconds(50));
  }

  cout << "Client disconnected: " << client_sock << endl;
  close(client_sock);
}

int main() {
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    cerr << "Socket creation error\n";
    return 1;
  }

  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(PORT);

  if (bind(server_fd, (sockaddr *)&addr, sizeof(addr)) < 0) {
    cerr << "Bind failed\n";
    return 1;
  }

  if (listen(server_fd, 10) < 0) {
    cerr << "Listen failed\n";
    return 1;
  }

  cout << "Server listening on port " << PORT << "...\n";

  while (true) {
    int client_sock = accept(server_fd, nullptr, nullptr);
    if (client_sock < 0) {
      cerr << "Accept failed\n";
      continue;
    }
    thread(handle_client, client_sock).detach();
  }

  close(server_fd);
  return 0;
}
