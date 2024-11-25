#pragma once

#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

class TcpServer {
private:
    SOCKET ListenSocket;        // Сокет для прослуховування
    SOCKET ClientSocket;        // Сокет для клієнта
    WSADATA wsaData;            // Дані WSA
    struct addrinfo* result;    // Результат налаштування адреси
    struct addrinfo hints;      // Параметри для getaddrinfo
    char* recvbuf;              // Буфер для отримання даних
    int messageBufferSize;      // Розмір буфера

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
     * Ініціалізація WSA.
     * @return true, якщо успішно, інакше false.
     */
    bool Initialize();

    /**
     * Прив’язка сокета та перехід у режим прослуховування.
     * @param listenAddress IP-адреса для прослуховування.
     * @param listenPort Порт для прослуховування.
     * @return true, якщо успішно, інакше false.
     */
    bool BindAndListen(const std::string& listenAddress, int listenPort);

    /**
     * Очікування підключення клієнта.
     * @return true, якщо клієнт успішно підключений, інакше false.
     */
    void RunServer();

    /**
     * Обробка взаємодії з клієнтом.
     */
    void HandleClientConnection(SOCKET clientSocket);

    /**
     * Очищення ресурсів.
     */
    void Cleanup();
};
