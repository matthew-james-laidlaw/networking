#pragma once

#include <cstdint>
#include <vector>

class IChannel
{
public:

	virtual auto Send(std::vector<uint8_t> const& bytes) -> void = 0;
	virtual auto Receive(size_t num_bytes) -> std::vector<uint8_t> = 0;
	virtual auto Close() -> void = 0;

};
