#include <stdio.h>
#include <unistd.h>
#include <poll.h>

int main() {
  struct pollfd fds[1];
  
  // Monitor standard input
  fds[0].fd = STDIN_FILENO;
  fds[0].events = POLLIN;

  printf("Waiting for input (5 seconds)... \n");
  
  int ret = poll(fds, 1, 5000);

  if (ret == -1) {
    perror("poll\n");
    return 1;
  }

  else if (ret == 0) {
    printf("Timeout! no input recieved.\n");
  }

  else {
    if (fds[0].revents & POLLIN) {
      printf("Data is available to read!\n");

      char buffer[100];
      ssize_t bytes = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
      if (bytes > 0) {
        buffer[bytes] = '\0';
        printf("You typed: %s\n", buffer);
      }
    }
  }
  return 0;
}
