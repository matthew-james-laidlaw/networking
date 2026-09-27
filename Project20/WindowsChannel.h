#pragma once

#include "Channel.h"

#include <WinSock2.h>

#pragma comment(lib, "Ws2_32.lib")

class WindowsChannel : public IChannel
{
private:

	SOCKET m_socket;

public:

	explicit WindowsChannel(SOCKET socket)
		: m_socket(socket)
	{}

	auto Send(std::vector<uint8_t> const& bytes) -> void override
	{

	}

	auto Receive(size_t num_bytes) -> std::vector<uint8_t> override
	{
		return {};
	}

};
