#pragma once

#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

class TcpServer {
private:
    SOCKET ListenSocket;        // ����� ��� ���������������
    SOCKET ClientSocket;        // ����� ��� �볺���
    WSADATA wsaData;            // ��� WSA
    struct addrinfo* result;    // ��������� ������������ ������
    struct addrinfo hints;      // ��������� ��� getaddrinfo
    char* recvbuf;              // ����� ��� ��������� �����
    int messageBufferSize;      // ����� ������

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
     * ����������� WSA.
     * @return true, ���� ������, ������ false.
     */
    bool Initialize();

    /**
     * �������� ������ �� ������� � ����� ���������������.
     * @param listenAddress IP-������ ��� ���������������.
     * @param listenPort ���� ��� ���������������.
     * @return true, ���� ������, ������ false.
     */
    bool BindAndListen(const std::string& listenAddress, int listenPort);

    /**
     * ���������� ���������� �볺���.
     * @return true, ���� �볺�� ������ ����������, ������ false.
     */
    void RunServer();

    /**
     * ������� �����䳿 � �볺����.
     */
    void HandleClientConnection(SOCKET clientSocket);

    /**
     * �������� �������.
     */
    void Cleanup();
};
