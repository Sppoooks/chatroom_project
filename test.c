#include <stdio.h>
#include <stdlib.h>

int main() {
  char *line = NULL;
  size_t size = 0;
  ssize_t char_count = getline(&line, &size, stdin);
  printf("char_count %zd:\n", char_count);
  free(line);
}
