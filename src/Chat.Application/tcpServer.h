#pragma once

#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

class TcpServer {
private:
    SOCKET ListenSocket;        // Сокет для прослуховування
    struct addrinfo* result;    // Результат налаштування адреси
    struct addrinfo hints;      // Параметри для getaddrinfo

    char* recvbuf;              // Буфер для отримання даних
    int messageBufferSize;      // Розмір буфера

    std::atomic<bool> running;
    std::thread listeningTask;
    void RunConnectionPulling();
    /**
    * Обробка взаємодії з клієнтом.
    */
    void HandleClientConnection(SOCKET clientSocket, char* clientAddress, int port);

public:
    /**
     * Конструктор.
     * @param bufferSize Розмір буфера для отримання повідомлень.
     */
    TcpServer(int bufferSize);

    /**
     * Деструктор.
     */
    ~TcpServer();

    /**
     * Прив’язка сокета та перехід у режим прослуховування.
     * @param listenAddress IP-адреса для прослуховування.
     * @param listenPort Порт для прослуховування.
     * @return true, якщо успішно, інакше false.
     */
    bool Bind(const std::string& listenAddress, int listenPort);
    bool Listen();

    /**
     * Очищення ресурсів.
     */
    void Close();
};
