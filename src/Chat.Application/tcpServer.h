#ifndef TCP_SERVER_H  // ���� ������TCP_SERVER_H �� ���������
#define TCP_SERVER_H // ��������� ������ TCP_SERVER_H
#define MATH_UTILS_H 2
// ҳ�� ������������� �����
int StartServer(char* listenAddress, int listenPort, int messageBufferSize);
int StopServer();
#endif  // ��������� ������ ��������