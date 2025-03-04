#include <stdbool.h>
#include <stdio.h>

bool is_printable_buf(const void *buf, int len) {
  const unsigned char *ptr = (const unsigned char *)buf;
  const unsigned char *end = ptr + len;

  while (ptr < end) {
    if (*ptr < 32 || *ptr > 126) {
      return false;
    }
    ptr++;
  }
  return true;
}

int main() {

  const char test1[] = "Fully printable";
  const char test2[] = "Not\nprintable";

  printf("test1: %s\n",
         is_printable_buf(test1, sizeof(test1) - 1) ? "true" : "false");
  printf("test2: %s\n",
         is_printable_buf(test2, sizeof(test2) - 1) ? "true" : "false");

  return 0;
}
