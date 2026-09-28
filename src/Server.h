#pragma once

#include "Channel.h"

#include <memory>

class IServer
{
public:

	virtual auto Listen(uint16_t port) -> void = 0;
	virtual auto Accept() -> std::unique_ptr<IChannel> = 0;
	virtual auto Close() -> void = 0;

};
