#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

int main() {
  int client_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  char *ip = ""; // local host IP
  int port = 9999;

  struct sockaddr_in
      address; // packaged information to give connect() function (IPv4)

  address.sin_family = AF_INET;   // AF_INET means IPv4
  address.sin_port = htons(port); // need to convert from host byte order
  inet_pton(AF_INET, ip,
            &address.sin_addr.s_addr); // converts IP address format from dotted
                                       // decimal format to struct in_addr

  int result =
      connect(client_socket_fd, (struct sockaddr *)&address, sizeof(address));
  if (result == 0)
    printf("Connection was successful\n");
  else
    printf("Connection was unsuccessful\n");

  char *line = NULL;
  size_t size = 0;

  while (true) {
    ssize_t charCount = getline(&line, &size, stdin);

    if (charCount > 0) {

      if (strcmp(line, "exit\n") == 0) {
        break;
      }

      ssize_t amount_was_sent = send(client_socket_fd, line, charCount, 0);
    }
  }

  char buffer[1024];
  recv(client_socket_fd, buffer, 1024, 0);

  printf("Response was:\n%s\n", buffer);

  return 0;
}
