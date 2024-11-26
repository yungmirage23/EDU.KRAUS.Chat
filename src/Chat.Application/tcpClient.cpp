#include "pch.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "tcpClient.h"
#include "json.hpp" // Підключаємо JSON-бібліотеку
#include "chatMessage.h"

TcpChatClient::TcpChatClient(int bufferSize) 
    : clientSocket(INVALID_SOCKET), bufferSize(bufferSize), running(false) {
}

TcpChatClient::~TcpChatClient() {
    Disconnect();
}

// Sending data to the server
bool TcpChatClient::Send(const ChatMessage& chatMessage) {
    
    if (!running) {
        std::cerr << "[Client] Cannot send message. Not connected to server." << std::endl;
        return false;
    }
    std::string serializedMessage;
    try {
        nlohmann::json jsonMessage = chatMessage.ToJson();
        serializedMessage = jsonMessage.dump(); // Перетворення JSON у рядок
    }
    catch (const std::exception& e) {
        std::cerr << "[Client] Error serrializing message: " << e.what() << std::endl;
        return false;
    }
   

    int sbyteCount = send(clientSocket, serializedMessage.c_str(), serializedMessage.size(), 0);
    if (sbyteCount == SOCKET_ERROR) {
        std::cout << "[Client] Send error: " << WSAGetLastError() << std::endl;
        return false;
    }
    return true;
}

bool TcpChatClient::Disconnect() {
    if (clientSocket != INVALID_SOCKET) {
        running = false;
        closesocket(clientSocket);
        if (receivingTask.joinable()) {
            receivingTask.join();
        }
       
        clientSocket = INVALID_SOCKET;
    }
    return true;
}

bool TcpChatClient::Connect(char* serverAddress, int port) {

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "[Client] Socket creation failed." << std::endl;
        return false;
    }

    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, serverAddress, &serverAddr.sin_addr);

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "[Client] Connection to server failed." << std::endl;
        closesocket(clientSocket);
        return false;
    }

    running = true;
    receivingTask = std::thread(&TcpChatClient::RunReceivingLoop, this);
    receivingTask.detach();

    return true;
}

bool TcpChatClient::RunReceivingLoop() {
    char* buffer = new char[bufferSize];
    while (running) {
        int bytesReceived = recv(clientSocket, buffer, bufferSize, 0);
        if (bytesReceived > 0) {
            try {
                std::string receivedData(buffer, bytesReceived);
                auto jsonMessage = nlohmann::json::parse(receivedData);
                ChatMessage chatMessage = ChatMessage::FromJson(jsonMessage);
                std::cout << "[" << chatMessage.timestamp << "] "
                    << "[" << chatMessage.username << "] "
                    << chatMessage.message << std::endl;
            }
            catch (const std::exception& e) {
                std::cerr << "[Client] Error parsing message: " << e.what() << std::endl;
            }
        }
        else if (bytesReceived == 0 || WSAGetLastError() != WSAEWOULDBLOCK) {
            std::cerr << "[Client] Connection closed or error occurred." << std::endl;
            running = false;
            break;
        }
    }
    delete[] buffer;
    return true;
}
