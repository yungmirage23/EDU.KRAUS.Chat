#define NOMINMAX // Відімкнення макросів min и max из Windows API
#include <iostream>
#include <thread>
#include "WinSock2.h"
#include <limits> // Для std::numeric_limits
#include <tcpServer.h>
#include <tcpClient.h>

char address[] = "127.0.0.1";
int port = 5000;
const int bufferSize = 1024;

TcpServer room(bufferSize);
TcpChatClient client(bufferSize);

void PrintMenu1() {
	std::cout << "===RoomMenu===\n";
	std::cout << "1. Create new room\n";
	std::cout << "2. Delete room\n";
	std::cout << "3. <--Back\n";
	std::cout << "Choose an option: ";
}
void Menu1() {
	bool running = true;
	while (running) {
		std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Пауза на 200 мс
		PrintMenu1();

		int choice;
		std::cin >> choice;

		// Перевірка на валідність
		if (std::cin.fail()) {
			std::cin.clear(); // Скидання флага помилки
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка ввода
			std::cout << "Invalid input. Please try again.\n";
			continue;
		}

		switch (choice) {
		case 1: {
			room.Bind(address, port);
			room.Listen();
			break;
		}
		case 2: {
			room.Close();
			break;
		}
		case 3: {
			running = false;
			break;
		}
		default:
			std::cout << "Invalid option. Please choose again.\n";
			break;
		}
	}
}

void PrintMenu2() {
	std::cout << "===ChatClient===\n";
	std::cout << "1. Connect to room\n";
	std::cout << "2. Disconnect\n";
	std::cout << "3. <--Back\n";
	std::cout << "Choose an option: ";
}
void Menu2() {
	bool running = true;
	while (running) {
		std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Пауза на 200 мс
		PrintMenu2();

		int choice;
		std::cin >> choice;

		// Перевірка на валідність
		if (std::cin.fail()) {
			std::cin.clear(); // Скидання флага помилки
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка ввода
			std::cout << "Invalid input. Please try again.\n";
			continue;
		}

		switch (choice) {
		case 1: {
			//const int SIZE = 16; // IPv4 адрес максимум 15 символов + 1 для '\0'
			//char connectionIp[SIZE];
			//int connectionPort;
			//std::cout << "Enter room IP address: ";
			//std::cin >> connectionIp;
			//// Перевірка на валідність
			//if (std::cin.fail()) {
			//    std::cin.clear(); // Скидання флага помилки
			//    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка ввода
			//    std::cout << "Invalid input. Please try again.\n";
			//    continue;
			//}
			//std::cout << "Enter room port: ";
			//std::cin >> connectionPort;
			//// Перевірка на валідність
			//if (std::cin.fail()) {
			//    std::cin.clear(); // Скидання флага помилки
			//    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка ввода
			//    std::cout << "Invalid input. Please try again.\n";
			//    continue;
			//}
			client.Connect(address, port);
			//client.Connect(connectionIp, connectionPort);

			char message[bufferSize];
			while (true) {
				std::cin.getline(message, bufferSize);  // Читаємо увесь рядок

				// Перевірка на команду для виходу
				if (std::strcmp(message, "/q") == 0) {
					std::cout << "Exiting...\n";
					break;
				}

				ChatMessage chatMessage("Retard", message);
				client.Send(chatMessage);
			}
			break;
		}
		case 2: {
			client.Disconnect();
			break;
		}
		case 3: {
			running = false;
			break;
		}
		default:
			std::cout << "Invalid option. Please choose again.\n";
			break;
		}
	}
}

void PrintMenu() {
	std::cout << "===MainWindow===\n";
	std::cout << "1. Room manager\n";
	std::cout << "2. Room client\n";
	std::cout << "3. Exit\n";
	std::cout << "Choose an option: ";
}
void Menu() {
	bool running = true;
	while (running) {
		std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Пауза на 200 мс
		PrintMenu();

		int choice;
		std::cin >> choice;
		// Перевірка на валідність
		if (std::cin.fail()) {
			std::cin.clear(); // Скидання флага помилки
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка ввода
			std::cout << "Invalid input. Please try again.\n";
			continue;
		}
		switch (choice) {
		case 1: {
			Menu1();
			break;
		}
		case 2: {
			Menu2();
			break;
		}
		case 3: {
			running = false;
			break;
		}
		default:
			std::cout << "Invalid option. Please choose again.\n";
			break;
		}
	}
}

int main()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cout << "[Server] WSAStartup failed!" << std::endl;
		return false;
	}
	Menu();

	return 0;
}