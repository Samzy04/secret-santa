#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>



#define PORT 12345

int main(){
    int sock = 0;
    struct sockaddr_in serv_addr;
    char name[50];

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
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
    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        printf("\nConnection Failed\n");
        return -1;
    }

    // Send registration request
    int draw = 0;
    while(draw != 2){
        printf("Enter your name else enter draw to pick a name: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = 0;

        int nameLength = strlen(name);
        send(sock, &nameLength, sizeof(int), 0); 
        send(sock, name, nameLength, 0); 

        printf("Registration request sent\n");
        if(strcmp(name,"draw\n")==0){
            printf("Draw requesnt sent\n");
            send(sock,&draw,sizeof(int),0);
            draw = 2;
        }

    }
    

    // Close socket
    close(sock);
    return 0;
}
