#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 100

// Function to send data to the server
void send_to_server(int sockfd, const char* message) {
    send(sockfd, message, strlen(message), 0);
}

// Function to receive data from the server
void receive_from_server(int sockfd, char* buffer) {
    read(sockfd, buffer, BUFFER_SIZE);
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char name[BUFFER_SIZE];

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    // Server address setup
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Connect to server
    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed\n");
        return -1;
    }

    // Send registration request
    while (1) {
        printf("Enter your name or enter 'draw' to initiate the draw: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = 0;

        if (strlen(name) > 0) {
            // Sending registration request to the server
            char message[BUFFER_SIZE];
            snprintf(message, sizeof(message), "REGISTER %s", name);
            send_to_server(sock, message);

            // Receive response from the server
            char buffer[BUFFER_SIZE] = {0};
            receive_from_server(sock, buffer);
            printf("Server Response: %s\n", buffer);

            // Check if draw request
            if (strcmp(name, "draw") == 0) {
                printf("Draw request sent\n");
                send_to_server(sock, "DRAW");
                break;
            }
        }
    }

    // Close socket
    close(sock);
    return 0;
}
