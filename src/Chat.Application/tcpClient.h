#pragma once

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <atomic>

class TcpChatClient {
private:
	SOCKET clientSocket;
	int bufferSize;
	std::atomic<bool> running;
	std::thread receivingTask;
	bool RunReceivingLoop();

public:
	TcpChatClient(int bufferSize);
	/**
   * Деструктор.
   */
	~TcpChatClient();

	bool Connect(char* serverAddress, int port);
	bool Disconnect();

	bool Send(const char* message);
};
