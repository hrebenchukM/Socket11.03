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
	WSADATA wsaData;//структура для хранения информацию
					//о инициализации сокетов
	SOCKET _socket; //дескриптор слушающего сокета
	SOCKET acceptSocket;//дескриптор сокета, который связан с клиентом
	sockaddr_in addr; //локальный адрес и порт
	//1инициализация сокетов
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//2создаем потоковый сокет, протокол TCP
	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//Семейство адресов IPv4
	addr.sin_family = AF_INET;
	//Преобразуем адрес в правильную структуру хранения адресов, результат сохраняем в поле sin_addr

	inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);
	//3Указываем порт.
	//Функиця htons выполняет преобразование числа в
	//сетевой порядок байт, т.к. стандартным сетевым
	//является порядок от старшего к младшему
	addr.sin_port = htons(20000);



	//биндим сокет к указанному адресу и порту
	bind(_socket, (SOCKADDR*)&addr, sizeof(addr));
	//открываем сокет в режиме прослушивания входящих подключений
	listen(_socket, 1);
	cout << "Server is started\n";

	//5ожидаем подключения клиента.
	//в случае успешного подключения - получаем новый сокет
	//через который будет происходить обмен сообщениями
	//с клиентом.
	//Функция является блокирующей.
	acceptSocket = accept(_socket, NULL, NULL);

	char buf[MAXSTRLEN];
	//6Получаем сообщение от клиента
	//Функция является блокирующей
	int i = recv(acceptSocket, buf, MAXSTRLEN, 0);
	buf[i] = '\0';
	cout << buf << endl;
	const char * text = "Hello from server!";
	//Отправляем сообщение клиенту
	send(acceptSocket, text, strlen(text), 0);

	//Закрываем сокеты
	closesocket(acceptSocket);
	closesocket(_socket);
	//Освобождаем ресурсы
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
			// Код для запуска сервера
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
			// Код для отправки сообщения клиенту
			wchar_t buffer[256];
			GetDlgItemTextW(hwnd, IDC_EDIT1, buffer, 256);
			send(acceptSocket, reinterpret_cast<char*>(buffer), wcslen(buffer) * sizeof(wchar_t), 0);
			break;
		case IDC_RECEIVE:
			// Код для получения сообщения от клиента
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