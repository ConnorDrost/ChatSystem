#include "Client.h"

Client_Socket::Client_Socket(int argc, char** argv)
{
    CreateSocket(argc, argv);
}

void Client_Socket::CreateSocket(int argc, char** argv)
{
    if (InitializeWindowsSocket() == false)
    {
        return;
    }

    connectSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if (connectSocket == INVALID_SOCKET)
    {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(argv[1]);
    serverAddress.sin_port = htons(DEFAULT_PORT);
    // connect to server specified in serverAddress and socket connectSocket
    if (connect(connectSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
    {
        printf("Unable to connect to server.\n");
        closesocket(connectSocket);
        WSACleanup();
    }

    return;
}

void Client_Socket::SendSocket()
{
    const char *messageToSend = "this is a test";

    iResult = send(connectSocket, messageToSend, (int)strlen(messageToSend) + 1, 0);

    if (iResult == SOCKET_ERROR)
    {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(connectSocket);
        WSACleanup();
        return;
    }

    printf("Bytes Sent: %ld\n", iResult);
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
};