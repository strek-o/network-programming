#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
    return EXIT_FAILURE;
  }

  int src = open(argv[1], O_RDONLY);
  if (src == -1) {
    perror("Error opening source file");
    return EXIT_FAILURE;
  }

  int dest = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (dest == -1) {
    perror("Error opening destination file");
    close(src);
    return EXIT_FAILURE;
  }

  char buffer[BUFFER_SIZE];
  ssize_t bytes_read, bytes_written;
  while ((bytes_read = read(src, buffer, BUFFER_SIZE)) > 0) {
    if (bytes_read == -1) {
      perror("Error reading source file");
      close(src);
      close(dest);
      return EXIT_FAILURE;
    }

    bytes_written = write(dest, buffer, bytes_read);
    if (bytes_written == -1) {
      perror("Error writing to destination file");
      close(src);
      close(dest);
      return EXIT_FAILURE;
    }
  }

  close(src);
  close(dest);

  return 0;
}
