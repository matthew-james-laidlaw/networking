#pragma once

#include <Connection.h>

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#include <print>

class Server
{
private:

	SOCKET m_server;

	std::string m_addr = "127.0.0.1";
	uint16_t m_port;

public:

	Server(uint16_t port)
		: m_server(Socket())
		, m_port(port)
	{
		Bind(m_port);
		Listen();
	}

	~Server()
	{
		closesocket(m_server);
		std::println("== [server] stopped listening");
	}

	auto Accept() -> std::unique_ptr<Connection>
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

		return std::make_unique<Connection>(client);
	}

private:

	static auto Socket() -> SOCKET
	{
		auto sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sock == INVALID_SOCKET)
		{
			throw std::runtime_error(std::format("socket failed with error code '{}'", WSAGetLastError()));
		}
		return sock;
	}

	auto Bind(uint16_t port) -> void
	{
		sockaddr_in server_address{};
		server_address.sin_family = AF_INET;
		server_address.sin_port = htons(port);

		PCSTR addr = "127.0.0.1";
		inet_pton(AF_INET, addr, &server_address.sin_addr);

		auto result = bind(m_server, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address));
		if (result == SOCKET_ERROR)
		{
			throw std::runtime_error(std::format("bind failed with error code '{}'", WSAGetLastError()));
		}
	}

	auto Listen() -> void
	{
		auto result = listen(m_server, SOMAXCONN);
		if (result == SOCKET_ERROR)
		{
			throw std::runtime_error(std::format("listen failed with error code '{}'", WSAGetLastError()));
		}
		std::println("== [server] listening on '{}:{}'", m_addr, m_port);
	}

};
