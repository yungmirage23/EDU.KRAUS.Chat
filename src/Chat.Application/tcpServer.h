#ifndef TCP_SERVER_H  // Якщо макросTCP_SERVER_H не визначено
#define TCP_SERVER_H // Визначити макрос TCP_SERVER_H
#define MATH_UTILS_H 2
// Тіло заголовочного файлу
int StartServer(char* listenAddress, int listenPort, int messageBufferSize);
int StopServer();
#endif  // Завершити умовну перевірку