#include <tcpClient.h>
#include <tcpServer.h>
#include <iostream>
#include <thread> 
#include "WinSock2.h"
int main()
{
    WSADATA wsaData;
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    char address[] = "127.0.0.1";
    int port = 5000;
    int bufferSize = 1024;

    StartClient(address, port, bufferSize);

    StartServer(address, port, bufferSize);

    WSACleanup();
}