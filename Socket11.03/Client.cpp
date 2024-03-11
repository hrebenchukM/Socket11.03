#include <iostream>
#include <string>
#include "WinSock2.h" //здесь находятся объявления, необходимые
//для Winsock 2 API.
#include <ws2tcpip.h> //содержит функции для работы с адресами
 //напр. inet_pton
#pragma comment(lib, "Ws2_32.lib") //линкуем библиотеку Windows Sockets


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
	WSADATA wsaData;//структура для хранения информацию
					//о инициализации сокетов
	SOCKET _socket; //дескриптор сокета
	sockaddr_in addr; //локальный адрес и порт

	//1 инициализация сокетов
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//2 создаем потоковый сокет, протокол TCP
	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


	//3 Семейство адресов IPv4
	addr.sin_family = AF_INET;
	//Преобразуем адрес в правильную структуру хранения адресов, результат сохраняем в поле sin_addr
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
	//Указываем порт.
	//Функция htons выполняет преобразование числа в
	//сетевой порядок байт, т.к. стандартным сетевым
	//является порядок от старшего к младшему
	addr.sin_port = htons(20000);


	//выполняем подключение к серверу, указывая
	//дескриптор сокета, адрес и размер структуры
	connect(_socket, (SOCKADDR*)&addr, sizeof(addr));

	char buf[MAXSTRLEN];
	char str[MAXSTRLEN];
	cout << "\nEnter text\n";
	//Ожидаем подтверждения отправки сообщения пользователем
	//(нажатие клавиши)
	cin.getline(str, MAXSTRLEN);

	//Отправляем сообщение на указанный сокет
	send(_socket, str, strlen(str), 0);

	//Принимаем сообщение от сервера
	//Функция является блокирующей
	int i = recv(_socket, buf, MAXSTRLEN, 0);
	buf[i] = '\0';

	cout << buf << endl;

	//Закрываем сокет
	closesocket(_socket);
	//Освобождаем ресурсы
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
			// Код для подключения к серверу
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
			// Код для отправки сообщения серверу
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


