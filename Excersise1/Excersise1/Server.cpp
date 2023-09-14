#include "Server.h"

Server_Socket::Server_Socket()
{

}

bool Server_Socket::InitializeWindowsSocket()
{
	WSADATA wsaData;
	// Initialize windows sockets library for this process
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup failed with error: %d\n", WSAGetLastError());
		return false;
	}
	return true;
}
int Server_Socket::CreateSocket()
{
	if (InitializeWindowsSocket() == false)
	{
		return 1;
	}

	memset(&sockInfo, 0, sizeof(sockInfo));

	sockInfo.ai_family = AF_INET;          // IPv4 address
	sockInfo.ai_socktype = SOCK_STREAM;    // Provide reliable data streaming
	sockInfo.ai_protocol = IPPROTO_TCP;    // Use TCP
	sockInfo.ai_flags = AI_PASSIVE;        // 


	iResult = getaddrinfo(NULL, DEFAULT_PORT, &sockInfo, &addrResult);

	if (iResult != 0)
	{
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	serverListenSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);

	if (serverListenSocket == INVALID_SOCKET)
	{
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}

	//BIND
	//Setup TCP listening socket
	iResult = bind(serverListenSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);

	if (iResult == SOCKET_ERROR)
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(addrResult);
		closesocket(serverListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(addrResult);

	//LISTEN


	if (listen(serverListenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(serverListenSocket);
		WSACleanup();
		return 1;
	}

	//ACCEPT

	acceptedSocket = accept(serverListenSocket, NULL, NULL);

	if (acceptedSocket == INVALID_SOCKET)
	{
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(serverListenSocket);
		WSACleanup();
		return 1;
	}





	return 0;
}
;
