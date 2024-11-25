#define NOMINMAX // Відімкнення макросів min и max из Windows API
#include <tcpClient.h>
#include <tcpServer.h>
#include <iostream>
#include <thread> 
#include "WinSock2.h"
#include <limits> // Для std::numeric_limits

void PrintMenu() {
    std::cout << "=== TcpChatClient Menu ===\n";
    std::cout << "1. Connect to server\n";
    std::cout << "2. Send message\n";
    std::cout << "3. Disconnect\n";
    std::cout << "4. Exit\n";
    std::cout << "Choose an option: ";
}

int main()
{
    char address[] = "127.0.0.1";
    int port = 5000;
    int bufferSize = 1024;

    TcpServer server(bufferSize);
    server.Initialize();
    server.BindAndListen(address, port);

    std::thread listeningTask(&TcpServer::RunServer, &server);
    listeningTask.detach();

    TcpChatClient client(bufferSize);

    bool running = true;
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Пауза на 500 мс
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
            /*std::string serverAddress;
            int port;

            std::cout << "Enter server address (e.g., 127.0.0.1): ";
            std::cin >> serverAddress;

            std::cout << "Enter port: ";
            std::cin >> port;*/

            if (client.Connect(address, port)) {
                std::cout << "Successfully connected to the server.\n";
            }
            else {
                std::cout << "Failed to connect to the server.\n";
            }
            break;
        }
        case 2: {
            std::cin.ignore(); // Очищення перед отриманням рядка
            std::string message;

            std::cout << "Enter your message: ";
            std::getline(std::cin, message);

            if (client.Send(message.c_str())) {
                std::cout << "Message sent successfully.\n";
            }
            else {
                std::cout << "Failed to send message.\n";
            }
            break;
        }
        case 3: {
            if (client.Disconnect()) {
                std::cout << "Disconnected from the server.\n";
            }
            else {
                std::cout << "Failed to disconnect.\n";
            }
            break;
        }
        case 4:
            running = false;
            client.Disconnect();
            std::cout << "Exiting the program.\n";
            break;
        default:
            std::cout << "Invalid option. Please choose again.\n";
            break;
        }
    }

    return 0;
}