#ifndef TCP_CLIENT_H  // ���� ������ TCP_CLIENT_H �� ���������
#define TCP_CLIENT_H // ��������� ������ TTCP_CLIENT_H

// ҳ�� ������������� �����
int StartClient(char* serverAddres, int port, int messageBufferSize);
int StopClient();
#endif  // ��������� ������ ��������