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

    std::thread serverThread(StartServer);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::thread clientThread(StartClient);
    serverThread.join();
    clientThread.join();

    WSACleanup();
}