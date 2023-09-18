#include "Client.h"

int __cdecl main(int argc, char** argv)
{
	Client_Socket* Client = new Client_Socket(argc, argv);

	Client->SendSocket();

	return 0;
}