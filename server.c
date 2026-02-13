#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <poll.h>
#include <unistd.h>

#define port 9999
#define MAX_CLIENTS 100

int main(void) {

  char *server_ip = "";   // empty string for local host

  int server_socket_fd =
      socket(AF_INET, SOCK_STREAM, 0); // server side socket file descriptor

  struct sockaddr_in address;

  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  inet_pton(
      AF_INET, server_ip,
      &address.sin_addr.s_addr); // convert ipv4 address to network convention

  int result =
      bind(server_socket_fd, (struct sockaddr *)&address, sizeof(address));

  if (result == 0)
    printf("Socket was bound successfully\n");

  listen(server_socket_fd,
         10); // marks server_socket as a passive listening socket

  struct pollfd fds[MAX_CLIENTS + 1];
  fds[0].fd = server_socket_fd;
  fds[0].events = POLLIN;

  int nfds = 1;

  while(1) {

    int ready = poll(fds, nfds, -1);

    if (ready < 0) {
      perror("poll");
      break;
    }

    for (int i = 0; i < nfds; i++) {
      if (fds[i].revents & POLLIN) {
        if (fds[i].fd == server_socket_fd) {
          int client_fd = accept(server_socket_fd, NULL, NULL);

          fds[nfds].fd = client_fd;
          fds[nfds].events = POLLIN;
          nfds++;

          printf("New client connected\n");
        }
      else {
          char buffer[1024];
          int bytes = read(fds[i].fd, buffer, sizeof(buffer));

          if (bytes <= 0) {

            printf("Client %d disconnected\n", fds[i].fd);
            close(fds[i].fd);
          }
          else {
            buffer[bytes] = '/0';

            printf("%s", buffer);
          }
        }
      }
    }
  }
  // char
  //     buffer[1024]; // hold up to 1024 ASCII characters, less than 1024 in utf-8

  // while (true) {
  //
  //   ssize_t amount_recieved = recv(client_socket_fd, buffer, 1024, 0);
  //
  //   if (amount_recieved > 0) {
  //     buffer[amount_recieved] = 0;
  //     printf("%s", buffer);
  //   }
  // }
  close(server_socket_fd);

  return 0;
}
