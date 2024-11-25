#ifndef TCP_CLIENT_H  // Якщо макрос TCP_CLIENT_H не визначено
#define TCP_CLIENT_H // Визначити макрос TTCP_CLIENT_H

// Тіло заголовочного файлу
int StartClient(char* serverAddres, int port, int messageBufferSize);
int StopClient();
#endif  // Завершити умовну перевірку