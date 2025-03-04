#include <stdbool.h>
#include <stdio.h>

bool is_printable_str(const char *str) {
  while (*str) {
    if (*str < 32 || *str > 126) {
      return false;
    }
    str++;
  }
  return true;
}

int main() {

  const char test1[] = "Fully printable";
  const char test2[] = "Not\nprintable";

  printf("test1: %s\n", is_printable_str(test1) ? "true" : "false");
  printf("test2: %s\n", is_printable_str(test2) ? "true" : "false");

  return 0;
}
