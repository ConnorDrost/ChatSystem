#include "Client.h"

int main()
{
	Client_Socket Client;

	Client.ConnectSocket();

	for (int i = 0; i < 10; i++)
	{
		Client.Send();
		Client.Receive();
	}

	Client.CloseSocket();

	system("pause");
	
	return 0;
}

Client_Socket::Client_Socket()
{
	CreateSocket();
}

void Client_Socket::CreateSocket()
{
	if (InitializeWindowsSocket() == false)
	{
		return;
	}

	connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (connectSocket == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return;
	}

	serverAddress.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &(serverAddress.sin_addr.s_addr));
	serverAddress.sin_port = htons(DEFAULT_PORT);

	return;
}

bool Client_Socket::Send()
{

	const char* messageToSend = "ping";

	iResult = send(connectSocket, messageToSend, (int)strlen(messageToSend) + 1, 0);

	if (iResult == SOCKET_ERROR)
	{
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(connectSocket);
		WSACleanup();
		return false;
	}

	return true;
}

bool Client_Socket::ConnectSocket()
{
	// connect to server specified in serverAddress and socket connectSocket
	if (connect(connectSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
	{
		printf("Unable to connect to server.\n");
		closesocket(connectSocket);
		WSACleanup();
		return false;
	}

	printf("Socket Connected\n");

	return true;
}

bool Client_Socket::InitializeWindowsSocket()
{
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup failed with error: %d\n", WSAGetLastError());
		return false;
	}
	return true;
}
void Client_Socket::CloseSocket()
{
	closesocket(connectSocket);
	WSACleanup();
}
void Client_Socket::Receive()
{

	// Receive data until the client shuts down the connection

	iResult = recv(connectSocket, recvBuff, DEFAULT_BUFLEN, 0);

	if (iResult > 0)
	{
		printf("%s\n", recvBuff);
	}

}
;