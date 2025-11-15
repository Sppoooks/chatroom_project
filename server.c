#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>

int main() {

  int port_number = 9999; // server listening on port 2000
  char *server_ip = "";   // empty string for local host

  int server_socket_fd =
      socket(AF_INET, SOCK_STREAM, 0); // server side socket file descriptor

  struct sockaddr_in address;

  address.sin_family = AF_INET;
  address.sin_port = htons(2000);
  inet_pton(
      AF_INET, server_ip,
      &address.sin_addr.s_addr); // convert ipv4 address to network convention

  int result =
      bind(server_socket_fd, (struct sockaddr *)&address, sizeof(address));

  if (result == 0)
    printf("Socket was bound successfully\n");

  listen(server_socket_fd,
         10); // marks server_socket as a passive listening socket

  int client_socket_fd = accept(server_socket_fd, 0, 0);

  char
      buffer[1024]; // hold up to 1024 ASCII characters, less than 1024 in utf-8

  recv(client_socket_fd, buffer, 1024, 0);

  printf("Response was:\n%s", buffer);

  return 0;
}
