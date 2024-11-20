#define WIN32_LEAN_AND_MEAN
#include "pch.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "tcpServer.h"
#include <iostream>
using namespace std;

// Need to link with Ws2_32.lib
//#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "5000"

int StartServer()
{
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    //// Initialize Winsock
    //iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    //if (iResult != 0) {
    //    cout << "[Server] WSAStartup failed with error: " << iResult << endl;
    //    return 1;
    //}

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo("192.168.5.4", DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        cout << "[Server] getaddrinfo failed with error: "<< iResult << endl;
        return 1;
    }
    

    // Create a SOCKET for the server to listen for client connections.
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        cout << "[Server] socket failed with error: " << WSAGetLastError() << endl;
        freeaddrinfo(result);
        return 1;
    }
    else {
        cout << "[Server] socket is OK!" << endl;
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        cout << "[Server] bind failed with error: " << WSAGetLastError() << endl;
        freeaddrinfo(result);
        closesocket(ListenSocket);
        return 1;
    }
    else {
        cout << "[Server] binded socket!" << endl;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        cout << "[Server] listen failed with error : " << WSAGetLastError() << endl;
        closesocket(ListenSocket);
        return 1;
    }
    else {
        cout << "[Server] listening to new connections!" << endl;
    }
    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        cout << "[Server] accept failed with error: " << WSAGetLastError() << endl;
        closesocket(ListenSocket);
        return 1;
    }
    else {
        cout << "[Server] accepted new client!" << endl;
    }

    // No longer need server socket
    closesocket(ListenSocket);

    // Receive until the peer shuts down the connection
    do {

        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            cout << "[Server] Bytes received: " << iResult << endl;
            // Echo the buffer back to the sender
            iSendResult = send(ClientSocket, recvbuf, iResult, 0);
            if (iSendResult == SOCKET_ERROR) {
                cout << "[Server] send failed with error: " << WSAGetLastError() << endl;
                closesocket(ClientSocket);
                return 1;
            }
            cout << "[Server] Bytes sent: " << iSendResult << endl;
        }
        else if (iResult == 0)
            cout << "[Server] Connection closing... "<< endl;
        else {
            cout << "[Server] recv failed with error: " << WSAGetLastError << endl;
            closesocket(ClientSocket);
            return 1;
        }

    } while (iResult > 0);

    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        cout << "[Server] shutdown failed with error :" << WSAGetLastError() << endl;
        closesocket(ClientSocket);
        return 1;
    }

    // cleanup
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}
