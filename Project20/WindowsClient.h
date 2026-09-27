#pragma once

#include "Client.h"

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#include <print>

class WindowsClient : public IClient
{
private:

	SOCKET m_client;

public:

	~WindowsClient()
	{
		closesocket(m_client);
		WSACleanup();
		std::println("== [client] closed connection");
	}

	auto Connect() -> std::unique_ptr<IChannel> override
	{
		auto wsa_data = WSADATA{};

		auto result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
		if (result != 0)
		{
			std::println("== [client] WSAStartup failed with error code '{}'", result);
			throw std::runtime_error("WSAStartup failed");
		}

		m_client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_client == INVALID_SOCKET)
		{
			std::println("== [client] socket failed with error code '{}'", WSAGetLastError());
			throw std::runtime_error("socket failed");
		}

		PCSTR addr = "127.0.0.1";
		u_short port = 8080;

		auto server_address = sockaddr_in{};
		server_address.sin_family = AF_INET;
		server_address.sin_port = htons(port);

		// convert IP address from text to binary form
		inet_pton(AF_INET, addr, &server_address.sin_addr);

		std::println("== [client] attempting connection to address '{}:{}'", addr, port);

		result = connect(m_client, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address));
		if (result == SOCKET_ERROR)
		{
			std::println("== [client] connect failed with error code '{}'", WSAGetLastError());
			throw std::runtime_error("connect failed");
		}

		std::println("== [client] connected to server at address '{}:{}'", addr, port);

		return std::make_unique<WindowsChannel>(m_client);
	}

};
