#pragma once

#include "Channel.h"

#include <memory>

class IServer
{
public:

	virtual auto Listen() -> void = 0;
	virtual auto Accept() -> std::unique_ptr<IChannel> = 0;

};
