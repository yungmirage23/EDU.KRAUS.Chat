#include "pch.h"
#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include "tcpServer.h"

#pragma comment(lib, "Ws2_32.lib")

TcpServer::TcpServer(int bufferSize)
    : ListenSocket(INVALID_SOCKET), ClientSocket(INVALID_SOCKET), result(nullptr), messageBufferSize(bufferSize) {
    recvbuf = new char[messageBufferSize];
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
}

TcpServer::~TcpServer() {
    delete[] recvbuf;
    Cleanup();
}

bool TcpServer::Initialize() {
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "[Server] WSAStartup failed!" << std::endl;
        return false;
    }
    return true;
}

bool TcpServer::BindAndListen(const std::string& listenAddress, int listenPort) {
    
    std::string portString = std::to_string(listenPort);
    const char* portChar = portString.c_str();

    int iResult = getaddrinfo(listenAddress.c_str(), portChar, &hints, &result);
    if (iResult != 0) {
        std::cout << "[Server] getaddrinfo failed with error: " << iResult << std::endl;
        return false;
    }

    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        std::cout << "[Server] socket failed with error: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        return false;
    }

    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    freeaddrinfo(result);
    if (iResult == SOCKET_ERROR) {
        std::cout << "[Server] bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(ListenSocket);
        return false;
    }

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        std::cout << "[Server] listen failed with error: " << WSAGetLastError() << std::endl;
        closesocket(ListenSocket);
        return false;
    }

    std::cout << "[Server] Server is listening on port " << listenPort << std::endl;
    return true;
}

void TcpServer::RunServer() {

    std::cout << "[Server] Server is running and waiting for connections..." << std::endl;

    while (true) {
        SOCKET clientSocket = accept(ListenSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            std::cout << "[Server] Accept failed with error: " << WSAGetLastError() << std::endl;
            continue; // Очікування нових підключень
        }

        std::cout << "[Server] Accepted new client connection!" << std::endl;

        // Обробка клієнта в окремому потоці
        std::thread clientThread(&TcpServer::HandleClientConnection, this, clientSocket);
        clientThread.detach(); // Від'єднуємо потік для незалежного виконання 
    }
}

void TcpServer::HandleClientConnection(SOCKET clientSocket) {
   
    char* buffer = new char[messageBufferSize];
    int bytesReceived;

    do {
        // Читання данних від клієнта
        bytesReceived = recv(clientSocket, buffer, messageBufferSize, 0);
        if (bytesReceived > 0) {
            std::cout << "[Server] Received: " << std::string(buffer, bytesReceived) << std::endl;

            // Надсилаємо клієнту повідомлення назад
            int iSendResult = send(clientSocket, buffer, bytesReceived, 0);
            if (iSendResult == SOCKET_ERROR) {
                std::cout << "[Server] Send failed with error: " << WSAGetLastError() << std::endl;
                break;
            }
        }
        else if (bytesReceived == 0 || WSAGetLastError() != WSAEWOULDBLOCK) {
            std::cerr << "[Server] Connection closed or error occurred." << std::endl;
        }
        else {
            std::cout << "[Server] Receive failed with error: " << WSAGetLastError() << std::endl;
            break;
        }
    } while (bytesReceived > 0);

    // Закриття сокета клієнта
    closesocket(clientSocket);
    delete[] buffer;
}

void TcpServer::Cleanup() {
    if (ListenSocket != INVALID_SOCKET) {
        closesocket(ListenSocket);
    }
    if (ClientSocket != INVALID_SOCKET) {
        closesocket(ClientSocket);
    }
}
