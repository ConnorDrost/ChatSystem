#pragma once
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

	Client_Socket(int argc, char** argv);
	~Client_Socket() {};

	void CreateSocket(int argc, char** argv);

	void SendSocket();

	bool InitializeWindowsSocket();

	SOCKET connectSocket = INVALID_SOCKET;

	int iResult = 0;
};