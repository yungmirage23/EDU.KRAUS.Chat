#include "pch.h"
#include "tcpClient.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "5000"

int StartClient() {
    // Create a socket
    SOCKET clientSocket;
    clientSocket = INVALID_SOCKET;
    // Create a socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "[Client] Socket creation failed." << WSAGetLastError() << std::endl;
        return 1;
    }

    // Check for socket creation success
    if (clientSocket == INVALID_SOCKET) {
        std::cout << "[Client] Error at socket(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 0;
    }
    else {
        std::cout << "[Client] Socket is OK!" << std::endl;
    }

    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    inet_pton(AF_INET, "192.168.5.4", &clientService.sin_addr);
    clientService.sin_port = htons(5000);  // Use the same port as the server

    // Use the connect function
    if (connect(clientSocket, reinterpret_cast<SOCKADDR*>(&clientService), sizeof(clientService)) == SOCKET_ERROR) {
        std::cout << "[Client] Client: connect() - Failed to connect: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 0;
    }
    else {
        std::cout << "[Client] Client: Connect() is OK!" << std::endl;
        std::cout << "[Client] Client: Can start sending and receiving data..." << std::endl;
    }


    // Sending data to the server
    char buffer[200];
    std::cout << "Enter the message: ";
    std::cin.getline(buffer, 200);
    int sbyteCount = send(clientSocket, buffer, 200, 0);
    if (sbyteCount == SOCKET_ERROR) {
        std::cout << "[Client] Client send error: " << WSAGetLastError() << std::endl;
        return -1;
    }
    else {
        std::cout << "[Client] Client: Sent " << sbyteCount << " bytes" << std::endl;
    }

    // Receiving data from the server
    char receiveBuffer[200];
    int rbyteCount = recv(clientSocket, receiveBuffer, 200, 0);
    if (rbyteCount < 0) {
        std::cout << "[Client] Client recv error: " << WSAGetLastError() << std::endl;
        return 0;
    }
    else {
        std::cout << "[Client] Client: Received data: " << receiveBuffer << std::endl;
    }

    return 0;
}