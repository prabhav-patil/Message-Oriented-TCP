#include "mysocket.c"

int main()
{
    int sock;  // Socket Descriptor
    struct sockaddr_in client_addr, server_addr;    // Structure to store address
    char buffer[100] = "Working.... :)\n";

    sock = my_socket(AF_INET, SOCK_MyTCP, 0);

    if(sock<0){
        perror("Could not create socket\n");
        exit(0);
    }

    // Filling the server address details
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(20009);

    // Binding server address to socket
    if(my_bind(sock, (struct sockaddr *) &server_addr, sizeof(server_addr))<0){
        perror("Could not bind local address\n");
        exit(0);
    }

    printf("Server is up and running....\n");

    my_listen(sock, 5);

    int cli_addr_len = sizeof(client_addr);
    // Accept connection request
    sock = my_accept(sock, (struct sockaddr *)&client_addr, &cli_addr_len);

    if(sock<0){
        perror("Could not accept\n");
        exit(0);
    }

    // Send string back to the client
    my_send(sock, buffer, strlen(buffer)+1,0);
    // Close the socket
    my_close(sock);

    return 0;
}