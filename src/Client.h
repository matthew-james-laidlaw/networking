#pragma once

#include "Channel.h"

#include <memory>

class IClient
{
public:

	virtual auto Connect() -> std::unique_ptr<IChannel> = 0;

};
