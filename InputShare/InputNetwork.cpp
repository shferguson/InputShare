#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

static SOCKET s;
static sockaddr_in serverAddr;

bool NetworkStartup(const char* ip, int port)
{
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);
	s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ip);

	return true;
}

bool NetworkSendInput(const INPUT& input)
{
	sendto(s, reinterpret_cast<const char*>(&input), sizeof(input), 0, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));

	return true;
}

void NetworkShutdown()
{
	closesocket(s);
	WSACleanup();
}

