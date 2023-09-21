#pragma once
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27016"



class Server_Socket
{
public:

	Server_Socket();
	~Server_Socket() {};

	bool InitializeWindowsSocket();
	void CreateSocket();
	
	bool BindSocket();
	bool ListenSocket();
	bool AcceptSocket();

	void ShutdownSocket();

	void Receive();

	SOCKET serverListenSocket = INVALID_SOCKET;
	SOCKET acceptedSocket = INVALID_SOCKET;

	struct addrinfo* addrResult = NULL, sockInfo;


	int iResult = 0;
	char recvBuff[DEFAULT_BUFLEN];
};