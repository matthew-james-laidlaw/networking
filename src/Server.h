#pragma once

#include <Channel.h>

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#include <print>

class Server
{
private:

	SOCKET m_server;

public:

	~Server()
	{
		Close();
	}

	auto Listen(uint16_t port) -> void
	{
		m_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_server == INVALID_SOCKET)
		{
			std::println("== [server] socket failed with error code '{}'", WSAGetLastError());
			throw std::runtime_error("socket failed");
		}
		
		sockaddr_in server_address{};
		server_address.sin_family = AF_INET;
		server_address.sin_port = htons(port);

		PCSTR addr = "127.0.0.1";
		inet_pton(AF_INET, addr, &server_address.sin_addr);

		auto result = bind(m_server, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address));
		if (result == SOCKET_ERROR)
		{
			std::println("== [server] bind failed with error code '{}'", WSAGetLastError());
			throw std::runtime_error("bind failed");
		}

		result = listen(m_server, SOMAXCONN);
		if (result == SOCKET_ERROR)
		{
			std::println("== [server] listen failed with error code '{}'", WSAGetLastError());
			throw std::runtime_error("listen failed");
		}

		std::println("== [server] listening on '{}:{}'", addr, port);
	}

	auto Accept() -> std::unique_ptr<Channel>
	{
		sockaddr_in client_addr{};
		int client_addr_size = sizeof(client_addr);

		SOCKET client = accept(m_server, reinterpret_cast<sockaddr*>(&client_addr), &client_addr_size);
		if (client == INVALID_SOCKET)
		{
			std::println("== [server] accept failed with error code '{}'", WSAGetLastError());
			throw std::runtime_error("accept failed");
		}

		char client_ip[INET_ADDRSTRLEN]{};
		inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
		auto client_port = ntohs(client_addr.sin_port);

		std::println("== [server] accepted client connection from '{}:{}'", client_ip, client_port);

		return std::make_unique<Channel>(client);
	}

	auto Close() -> void
	{
		closesocket(m_server);
		std::println("== [server] stopped listening");
	}

};
