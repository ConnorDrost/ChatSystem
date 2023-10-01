#pragma once
#include <iostream>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27016"

#pragma pack(1)
typedef struct packet {
	int num;
	int cnt;
}PAKET;


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

	bool Send();
	void Receive();

	SOCKET serverListenSocket = INVALID_SOCKET;
	SOCKET acceptedSocket = INVALID_SOCKET;

	struct addrinfo* addrResult = NULL, sockInfo;


	int iResult = 0;
	char recvBuff[DEFAULT_BUFLEN];
};