#include "Server.h"

int main(void)
{
	Server_Socket* Server = new Server_Socket();

	if (Server->iResult = 1)
	{
		return 0;
	}

	Server->BindSocket();

	if (Server->iResult = 1)
	{
		return 0;
	}

	Server->ListenSocket();

	if (Server->iResult = 1)
	{
		return 0;
	}

	Server->Receive();

	return 0;
};