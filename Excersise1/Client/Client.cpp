#include "Client.h"

int main()
{
    Client_Socket Client;

    if (!Client.ConnectSocket())
    {
        return 0;
    }

    if (!Client.Send())
    {
        return 0;
    }

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

    connectSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

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

    const char *messageToSend = "ping";

    for (int i = 0; i < 10; i++)
    {
        iResult = send(connectSocket, messageToSend, (int)strlen(messageToSend) + 1, 0);

        if (iResult == SOCKET_ERROR)
        {
            printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(connectSocket);
            WSACleanup();
            return false;
        }
    }


    printf("Bytes Sent: %ld\n", iResult);

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

    int bp = 0;

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
void Client_Socket::Receive()
{
    do
    {
        // Receive data until the client shuts down the connection
        iResult = recv(connectSocket, recvBuff, DEFAULT_BUFLEN, 0);


        if (iResult > 0)
        {
            printf("Message received from client: %s.\n", recvBuff);
        }
        else if (iResult == 0)
        {
            // connection was closed gracefully
            printf("Connection with client closed.\n");
            closesocket(connectSocket);
        }
        else
        {
            // there was an error during recv
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(connectSocket);
        }

    } while (iResult > 0);
}
;