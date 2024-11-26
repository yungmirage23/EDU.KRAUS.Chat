#pragma once

#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

class TcpServer {
private:
    SOCKET ListenSocket;        // ����� ��� ���������������
    struct addrinfo* result;    // ��������� ������������ ������
    struct addrinfo hints;      // ��������� ��� getaddrinfo

    char* recvbuf;              // ����� ��� ��������� �����
    int messageBufferSize;      // ����� ������

    std::atomic<bool> running;
    std::thread listeningTask;
    void RunConnectionPulling();
    /**
    * ������� �����䳿 � �볺����.
    */
    void HandleClientConnection(SOCKET clientSocket, char* clientAddress, int port);

public:
    /**
     * �����������.
     * @param bufferSize ����� ������ ��� ��������� ����������.
     */
    TcpServer(int bufferSize);

    /**
     * ����������.
     */
    ~TcpServer();

    /**
     * �������� ������ �� ������� � ����� ���������������.
     * @param listenAddress IP-������ ��� ���������������.
     * @param listenPort ���� ��� ���������������.
     * @return true, ���� ������, ������ false.
     */
    bool Bind(const std::string& listenAddress, int listenPort);
    bool Listen();

    /**
     * �������� �������.
     */
    void Close();
};
