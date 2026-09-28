#include "WindowsChannel.h"
#include "WindowsClient.h"
#include "WindowsServer.h"

#include <thread>

auto Encode(char const* msg) -> std::vector<uint8_t>
{
	return std::vector<uint8_t>(msg, msg + strlen(msg));
}

auto main() -> int
{
	auto server_thread = std::thread([]()
	{
		auto server = WindowsServer();
		server.Listen();
		for (int i = 0; i < 3; ++i)
		{
			auto conn = server.Accept();
			conn->Receive(strlen("Hello, World!"));
		}
	});

	auto client_thread_1 = std::thread([]()
	{
		auto client = WindowsClient();
		auto conn = client.Connect();
		conn->Send(Encode("Hello, World!"));
	});

	auto client_thread_2 = std::thread([]()
	{
		auto client = WindowsClient();
		auto conn = client.Connect();
		conn->Send(Encode("Hello, World!"));
	});

	auto client_thread_3 = std::thread([]()
	{
		auto client = WindowsClient();
		auto conn = client.Connect();
		conn->Send(Encode("Hello, World!"));
	});

	server_thread.join();
	client_thread_1.join();
	client_thread_2.join();
	client_thread_3.join();

	return 0;
}
