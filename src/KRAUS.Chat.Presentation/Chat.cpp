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

    StartServer(address, port, bufferSize);
    StartClient(address,port,bufferSize);

    //std::thread serverThread(StartServer, address, port, bufferSize);
    //std::this_thread::sleep_for(std::chrono::seconds(1));
    //std::thread clientThread(StartClient, address, port, bufferSize);
    //serverThread.join();
    //clientThread.join();

    WSACleanup();
}