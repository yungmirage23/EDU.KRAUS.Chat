#include <tcpClient.h>
#include <tcpServer.h>
#include <iostream>
#include <thread> 
#include "WinSock2.h"
int main()
{ 
    char address[] = "127.0.0.1";
    int port = 5000;
    int bufferSize = 1024;

    TcpServer server(bufferSize);
    server.Initialize();
    server.BindAndListen(address, port);

    std::thread listeningTask(&TcpServer::RunServer,&server);
    listeningTask.detach();


    StartClient(address, port, bufferSize);

    WSACleanup();
}