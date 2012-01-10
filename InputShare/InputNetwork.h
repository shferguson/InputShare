#pragma once

#pragma comment(lib, "ws2_32.lib")

#include <Windows.h>

bool NetworkStartup(const char* ip, int port);
bool NetworkSendInput(const INPUT& input);
void NetworkShutdown();