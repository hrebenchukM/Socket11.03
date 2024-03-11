#include <iostream>
#include <string>
#include "WinSock2.h" //����� ��������� ����������, �����������
//��� Winsock 2 API.
#include <ws2tcpip.h> //�������� ������� ��� ������ � ��������
 //����. inet_pton
#pragma comment(lib, "Ws2_32.lib") //������� ���������� Windows Sockets


#include <windowsX.h>
#include <tchar.h>
#include "resource.h"
using namespace std;


SOCKET _socket;

BOOL CALLBACK ClientDlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_CLIENTDIALOG), NULL, (DLGPROC)ClientDlgProc);
	return 0;
}
/*
int main()
{
	const int MAXSTRLEN = 255;
	WSADATA wsaData;//��������� ��� �������� ����������
					//� ������������� �������
	SOCKET _socket; //���������� ������
	sockaddr_in addr; //��������� ����� � ����

	//1 ������������� �������
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//2 ������� ��������� �����, �������� TCP
	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


	//3 ��������� ������� IPv4
	addr.sin_family = AF_INET;
	//����������� ����� � ���������� ��������� �������� �������, ��������� ��������� � ���� sin_addr
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
	//��������� ����.
	//������� htons ��������� �������������� ����� �
	//������� ������� ����, �.�. ����������� �������
	//�������� ������� �� �������� � ��������
	addr.sin_port = htons(20000);


	//��������� ����������� � �������, ��������
	//���������� ������, ����� � ������ ���������
	connect(_socket, (SOCKADDR*)&addr, sizeof(addr));

	char buf[MAXSTRLEN];
	char str[MAXSTRLEN];
	cout << "\nEnter text\n";
	//������� ������������� �������� ��������� �������������
	//(������� �������)
	cin.getline(str, MAXSTRLEN);

	//���������� ��������� �� ��������� �����
	send(_socket, str, strlen(str), 0);

	//��������� ��������� �� �������
	//������� �������� �����������
	int i = recv(_socket, buf, MAXSTRLEN, 0);
	buf[i] = '\0';

	cout << buf << endl;

	//��������� �����
	closesocket(_socket);
	//����������� �������
	WSACleanup();
	system("pause");
}*/
BOOL CALLBACK ClientDlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_CONNECT:
			// ��� ��� ����������� � �������
			WSADATA wsaData;
			WSAStartup(MAKEWORD(2, 2), &wsaData);
			_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

			sockaddr_in serverAddr;
			serverAddr.sin_family = AF_INET;
			serverAddr.sin_port = htons(20000);
			inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

			connect(_socket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
			break;
		case IDC_SEND:
			// ��� ��� �������� ��������� �������
			wchar_t buffer[256];
			GetDlgItemTextW(hwnd, IDC_EDIT1, buffer, 256);

			send(_socket, reinterpret_cast<char*>(buffer), wcslen(buffer) * sizeof(wchar_t), 0);
			break;
		case IDC_RECEIVE:
			wchar_t receiveBuffer[256];
			int bytesReceived = recv(_socket, reinterpret_cast<char*>(receiveBuffer), sizeof(receiveBuffer), 0);
			if (bytesReceived > 0) {
				receiveBuffer[bytesReceived / sizeof(wchar_t)] = L'\0'; 
				SetDlgItemTextW(hwnd, IDC_EDIT2, receiveBuffer);
			}

			break;
		}
		break;
	case WM_CLOSE:
		closesocket(_socket);
		WSACleanup();
		EndDialog(hwnd, 0);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}


