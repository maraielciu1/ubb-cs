#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void handleClient(int socket);

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1234);
    server_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    if (bind(serverSocket, (sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }
    if (listen(serverSocket, 20) == -1) {
        std::cerr << "Error listening on socket" << std::endl;
        return 1;
    }
    while(true) {
        sockaddr_in client_addr;
        socklen_t client_addr_size = sizeof(client_addr);
        int client_socket = accept(serverSocket, (struct sockaddr*)&client_addr, &client_addr_size);
        pid_t pid = fork();
        if(pid == 0) { // client
            close(serverSocket);
            handleClient(client_socket);
            exit(1);
        } else {
            close(client_socket);
        }
    }
    close(serverSocket);

    return 0;
}

void handleClient(int client_socket) {
    int received;
    recv(client_socket, &received, sizeof(received), 0);
    std::cout << received << "\n";
    send(client_socket, &received, sizeof(received), 0);
}
