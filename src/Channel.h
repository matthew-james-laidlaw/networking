#pragma once

#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")

#include <cstdint>
#include <print>
#include <vector>

class Channel
{
private:

	SOCKET m_socket;

public:

	explicit Channel(SOCKET socket)
		: m_socket(socket)
	{}

	~Channel()
	{
		Close();
	}

	auto Send(std::vector<uint8_t> const& bytes) -> void
	{

	}

	auto Receive(size_t num_bytes) -> std::vector<uint8_t>
	{
		return {};
	}

	auto Close() -> void
	{
		closesocket(m_socket);
		std::println("== [client] closed connection");
	}

};
