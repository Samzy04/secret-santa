#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/types.h>
#include <pthread.h>

#define PORT 12345
#define BUFFER_SIZE 100

// Linked list to store clients' names
typedef struct client
{
  char *name;
  int sd;
} Client;

typedef struct client_list
{
  Client client;
  struct client_list *next;
} Client_list;

int main()
{
  int server_socket, client_socket;
  struct sockaddr_in server_address, client_address;

  socklen_t client_addr_len = sizeof(client_address);
  socklen_t server_addr_len = sizeof(server_address);

  char buffer[BUFFER_SIZE];
  int received_zero;

  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);

  // Bind server socket to address
  if (bind(server_socket, (struct sockaddr *)&server_address, server_addr_len) == -1)
  {
    perror("Socket binding failed");
    exit(EXIT_FAILURE);
  }
  // Prepare the server for incoming client requests
  // 10 is the maximum length for the queue of pending connections
  if (listen(server_socket, 10) == -1)
  {
    perror("Listening failed");
    exit(EXIT_FAILURE);
  }

  printf("Server listening on port %d...\n", ntohs(server_address.sin_port));

  while (1)
  {
    // Accepting a new connection
    if ((client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_addr_len)) < 0)
    {
      perror("Accept failed");
    };
    printf("New connection, socket is %d, IP is : %s, port : %d\n", client_socket,
           inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

    while (1)
    {
      // Receive number from client
      if (recv(client_socket, &received_zero, sizeof(received_zero), 0) == -1)
      {
        perror("Receiving number failed");
        exit(EXIT_FAILURE);
      }

      printf("Received from client: %d \n", received_zero);

      strcpy(buffer, "hello");
      if (send(client_socket, buffer, strlen(buffer), 0) == -1)
      {
        perror("Sending data failed");
        exit(EXIT_FAILURE);
      }
      sleep(10);
    }
    close(client_socket);
    close(server_socket);
  }
  return 0;
}
