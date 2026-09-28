#pragma once

#include "Channel.h"
#include "WindowsChannel.h"

#include <memory>
#include <string_view>

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#include <print>

inline auto Connect(std::string_view address, uint16_t port) -> std::unique_ptr<IChannel>
{
	auto wsa_data = WSADATA{};

	auto result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (result != 0)
	{
		std::println("== [client] WSAStartup failed with error code '{}'", result);
		throw std::runtime_error("WSAStartup failed");
	}

	auto client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client == INVALID_SOCKET)
	{
		std::println("== [client] socket failed with error code '{}'", WSAGetLastError());
		throw std::runtime_error("socket failed");
	}

	auto server_address = sockaddr_in{};
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);

	// convert IP address from text to binary form
	inet_pton(AF_INET, address.data(), &server_address.sin_addr);

	std::println("== [client] attempting connection to address '{}:{}'", address, port);

	result = connect(client, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address));
	if (result == SOCKET_ERROR)
	{
		std::println("== [client] connect failed with error code '{}'", WSAGetLastError());
		throw std::runtime_error("connect failed");
	}

	std::println("== [client] connected to server at address '{}:{}'", address, port);

	return std::make_unique<WindowsChannel>(client);
}
