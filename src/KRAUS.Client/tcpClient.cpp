#include "pch.h"
#include "tcpClient.h"
#include <iostream>
#include <WinSock2.h>

int StartClient() {
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;

    // Create a socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << WSAGetLastError() << std::endl;
        return 1;
    }

    // Connect to the server
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_LOOPBACK; // Localhost
    serverAddr.sin_port = htons(5000);

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed." << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        return 1;
    }

    std::cout << "Connected to server!" << std::endl;

    // Communicate
    const char* message = "Hello, Server!";
    send(clientSocket, message, strlen(message), 0);

    char buffer[1024];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "Received: " << buffer << std::endl;
    }

    // Cleanup
    closesocket(clientSocket);
    return 0;
}