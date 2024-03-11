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


SOCKET _socket, acceptSocket;

BOOL CALLBACK ServerDlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_SERVERDIALOG), NULL, (DLGPROC)ServerDlgProc);
	return 0;
}

/*
void main()
{
	const int MAXSTRLEN = 255;
	WSADATA wsaData;//��������� ��� �������� ����������
					//� ������������� �������
	SOCKET _socket; //���������� ���������� ������
	SOCKET acceptSocket;//���������� ������, ������� ������ � ��������
	sockaddr_in addr; //��������� ����� � ����
	//1������������� �������
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//2������� ��������� �����, �������� TCP
	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//��������� ������� IPv4
	addr.sin_family = AF_INET;
	//����������� ����� � ���������� ��������� �������� �������, ��������� ��������� � ���� sin_addr

	inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);
	//3��������� ����.
	//������� htons ��������� �������������� ����� �
	//������� ������� ����, �.�. ����������� �������
	//�������� ������� �� �������� � ��������
	addr.sin_port = htons(20000);



	//������ ����� � ���������� ������ � �����
	bind(_socket, (SOCKADDR*)&addr, sizeof(addr));
	//��������� ����� � ������ ������������� �������� �����������
	listen(_socket, 1);
	cout << "Server is started\n";

	//5������� ����������� �������.
	//� ������ ��������� ����������� - �������� ����� �����
	//����� ������� ����� ����������� ����� �����������
	//� ��������.
	//������� �������� �����������.
	acceptSocket = accept(_socket, NULL, NULL);

	char buf[MAXSTRLEN];
	//6�������� ��������� �� �������
	//������� �������� �����������
	int i = recv(acceptSocket, buf, MAXSTRLEN, 0);
	buf[i] = '\0';
	cout << buf << endl;
	const char * text = "Hello from server!";
	//���������� ��������� �������
	send(acceptSocket, text, strlen(text), 0);

	//��������� ������
	closesocket(acceptSocket);
	closesocket(_socket);
	//����������� �������
	WSACleanup();
	system("pause");
}*/

BOOL CALLBACK ServerDlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_START:
			// ��� ��� ������� �������
			WSADATA wsaData;
			WSAStartup(MAKEWORD(2, 2), &wsaData);
			_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

			sockaddr_in addr;
			addr.sin_family = AF_INET;
			addr.sin_port = htons(20000);
			inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);

			bind(_socket, (SOCKADDR*)&addr, sizeof(addr));
			listen(_socket, 1);
			acceptSocket = accept(_socket, NULL, NULL);
			break;
		case IDC_SEND:
			// ��� ��� �������� ��������� �������
			wchar_t buffer[256];
			GetDlgItemTextW(hwnd, IDC_EDIT1, buffer, 256);
			send(acceptSocket, reinterpret_cast<char*>(buffer), wcslen(buffer) * sizeof(wchar_t), 0);
			break;
		case IDC_RECEIVE:
			// ��� ��� ��������� ��������� �� �������
			wchar_t receiveBuffer[256];
			int bytesReceived = recv(acceptSocket, reinterpret_cast<char*>(receiveBuffer), sizeof(receiveBuffer), 0);
			if (bytesReceived > 0) {
				receiveBuffer[bytesReceived / sizeof(wchar_t)] = L'\0'; 
				SetDlgItemTextW(hwnd, IDC_EDIT2, receiveBuffer);
			}

			break;
		}
		break;
	case WM_CLOSE:
		closesocket(acceptSocket);
		closesocket(_socket);
		WSACleanup();
		EndDialog(hwnd, 0);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}