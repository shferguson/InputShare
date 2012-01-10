#include <Windows.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char* argv[])
{
	int port = 5555;
	if (argc > 1)
		port = atoi(argv[1]);

	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);

	SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	sockaddr_in addr;
	addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
	bind(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));

	while (true)
	{
		char buffer[4096];
		sockaddr_in senderAddr;
		int senderAddrLen = sizeof(senderAddr);
		int recvSize = recvfrom(s, buffer, sizeof(buffer), 0, reinterpret_cast<sockaddr*>(&senderAddr), &senderAddrLen);

		if (recvSize == sizeof(INPUT))
		{
			INPUT *input = reinterpret_cast<INPUT*>(buffer);
			SendInput(1, input, sizeof(INPUT));
		}

		if (recvSize == SOCKET_ERROR)
		{
			int lastError = WSAGetLastError();
			printf("Socket error: %d\r\n", lastError);
		}
	}

	closesocket(s);
	WSACleanup();
	
	return 0;
}