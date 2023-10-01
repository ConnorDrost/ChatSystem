#include "Server.h"

int main(void)
{
	Server_Socket Server;


	if (!Server.BindSocket())
	{
		return 0;
	}

	if (!Server.ListenSocket())
	{
		return 0;
	}

	Server.Receive();

	Server.ShutdownSocket();

	system("pause");

	return 0;
};

Server_Socket::Server_Socket()
{
	CreateSocket();
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

void Server_Socket::CreateSocket()
{
	if (InitializeWindowsSocket() == false)
	{
		return;
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
		return;
	}

	serverListenSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);

	if (serverListenSocket == INVALID_SOCKET)
	{
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(addrResult);
		WSACleanup();
		return;
	}

	printf("Socket Created\n");

	return;

}

bool Server_Socket::BindSocket()
{
	//BIND
	
	//Setup TCP listening socket
	iResult = bind(serverListenSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);

	if (iResult == SOCKET_ERROR)
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(addrResult);
		closesocket(serverListenSocket);
		WSACleanup();
		return false;
	}

	freeaddrinfo(addrResult);

	printf("Socket Bound\n");

	return true;
}

bool Server_Socket::ListenSocket()
{
	//LISTEN

	if (listen(serverListenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(serverListenSocket);
		WSACleanup();
		return false;
	}

	printf("Listening\n");

	return true;
}

	

bool Server_Socket::AcceptSocket()
{
	//ACCEPT

	acceptedSocket = accept(serverListenSocket, NULL, NULL);

	if (acceptedSocket == INVALID_SOCKET)
	{
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(serverListenSocket);
		WSACleanup();
		return false;
	}

	printf("Socket Accepted\n");

	return true;
}

void Server_Socket::ShutdownSocket()
{

	iResult = shutdown(acceptedSocket, SD_SEND);

	if (iResult == SOCKET_ERROR)
	{
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(acceptedSocket);
		WSACleanup();
		return;
	}

	printf("Socket Shutdown\n");

	return;
}

bool Server_Socket::Send()
{
	const char* messageToSend = "pong";

	iResult = send(acceptedSocket, messageToSend, (int)strlen(messageToSend) + 1, 0);

	if (iResult == SOCKET_ERROR)
	{
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(acceptedSocket);
		WSACleanup();
		return false;
	}

	return true;
}

void Server_Socket::Receive()
{
	do
	{
		// Wait for clients and accept client connections.
		// Returning value is acceptedSocket used for further
		// Client<->Server communication. This version of
		// server will handle only one client

		AcceptSocket();

		do
		{

			// Receive data until the client shuts down the connection
			iResult = recv(acceptedSocket, recvBuff, DEFAULT_BUFLEN, 0);


			if (iResult > 0)
			{
				printf("%s\n", recvBuff);
				Send();
			}
			else if (iResult == 0)
			{
				// connection was closed gracefully
				printf("Connection with client closed.\n");
				closesocket(acceptedSocket);
			}
			else
			{
				// there was an error during recv
				printf("recv failed with error: %d\n", WSAGetLastError());
				closesocket(acceptedSocket);
			}

		} while (iResult > 0);
		
		break;

	} while (1);

	return;
}

