#include "pch.h"
#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include "tcpServer.h"

#pragma comment(lib, "Ws2_32.lib")

TcpServer::TcpServer(int bufferSize)
    : ListenSocket(INVALID_SOCKET), result(nullptr), messageBufferSize(bufferSize) {
    recvbuf = new char[messageBufferSize];
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
}

TcpServer::~TcpServer() {
    delete[] recvbuf;
    Close();
}

bool TcpServer::Bind(const std::string& listenAddress, int listenPort) {
    
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

    return true;
}

bool TcpServer::Listen() {

    int iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        std::cout << "[Server] listen failed with error: " << WSAGetLastError() << std::endl;
        closesocket(ListenSocket);
        return false;
    }

    // Виводимо на яку адресу був привязаний сокет
    sockaddr_in boundAddr = {};
    int addrLen = sizeof(boundAddr);
    if (getsockname(ListenSocket, (sockaddr*)&boundAddr, &addrLen) == SOCKET_ERROR) {
        std::cerr << "[Server] getsockname failed: " << WSAGetLastError() << "\n";
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Выводим информацию о сокете
    char ipStr[INET_ADDRSTRLEN] = {};
    inet_ntop(AF_INET, &boundAddr.sin_addr, ipStr, sizeof(ipStr));
    std::cout << "[Server] Listening on: " << ipStr
        << ", port: " << ntohs(boundAddr.sin_port) << "\n";

    
    // Прослуховування нових підключень в окремому потоці
    listeningTask = std::thread(&TcpServer::RunConnectionPulling, this);
    listeningTask.detach();

    return true;
}

void TcpServer::RunConnectionPulling() {

    std::cout << "[Server] Server is running and waiting for connections..." << std::endl;
    running = true;
    while (running) {

        sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        SOCKET clientSocket = accept(ListenSocket, (sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == INVALID_SOCKET) {
            std::cout << "[Server] Accept failed with error: " << WSAGetLastError() << std::endl;
            continue; // Очікування нових підключень
        }
        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
        int clientPort = ntohs(clientAddr.sin_port);
        std::cout << "[Server] Accepted new client connection from! " << clientIP << ':'<< clientPort << std::endl;

        // Обробка клієнта в окремому потоці
        std::thread clientThread(&TcpServer::HandleClientConnection, this, clientSocket, clientIP, clientPort);
        clientThread.detach(); // Від'єднуємо потік для незалежного виконання 
    }
}

void TcpServer::HandleClientConnection(SOCKET clientSocket, char* clientAddress, int port) {
   
    char* buffer = new char[messageBufferSize];
    int bytesReceived;

    do {
        // Читання данних від клієнта
        bytesReceived = recv(clientSocket, buffer, messageBufferSize, 0);
        if (bytesReceived > 0) {
            //std::cout << "[Server] Received: " << std::string(buffer, bytesReceived) << std::endl;

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

void TcpServer::Close() {
    if (ListenSocket != INVALID_SOCKET) {
        closesocket(ListenSocket);
    }
    running = false;
}
