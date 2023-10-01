#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT 27016

class Client_Socket
{
public:

	Client_Socket();
	~Client_Socket() {};

	void CreateSocket();

	bool Send();

	bool ConnectSocket();

	bool InitializeWindowsSocket();
	void CloseSocket();

	void Receive();

	SOCKET connectSocket = INVALID_SOCKET;
	sockaddr_in serverAddress;

	int iResult = 0;
	char recvBuff[DEFAULT_BUFLEN];
};