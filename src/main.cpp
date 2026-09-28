#include <Connection.h>
#include <Server.h>

#include <thread>
#include <iostream>

class WinsockRuntime
{
public:

	WinsockRuntime()
	{
		WSAData data{};
		auto result = WSAStartup(MAKEWORD(2, 2), &data);
		if (result != 0)
		{
			throw std::runtime_error("WSAStartup failed");
		}
	}

	~WinsockRuntime()
	{
		WSACleanup();
	}

	WinsockRuntime(WinsockRuntime const&) = delete;
	WinsockRuntime& operator=(WinsockRuntime const&) = delete;

};

auto Encode(char const* msg) -> std::vector<uint8_t>
{
	return std::vector<uint8_t>(msg, msg + strlen(msg));
}

auto main() -> int
{
	auto winsock_runtime = WinsockRuntime();
	auto server = Server(8080);

	auto server_thread = std::thread([&server]()
	{
		try
		{
			for (int i = 0; i < 3; ++i)
			{
				auto conn = server.Accept();
				conn->Receive(strlen("Hello, World!"));
			}
		}
		catch (std::exception const& e)
		{
			std::cout << "== [server] " << e.what() << '\n';
		}
	});

	auto client_thread_1 = std::thread([]()
	{
		auto conn = Connect("127.0.0.1", 8080);
		conn->Send(Encode("Hello, World!"));
	});

	auto client_thread_2 = std::thread([]()
	{
		auto conn = Connect("127.0.0.1", 8080);
		conn->Send(Encode("Hello, World!"));
	});

	auto client_thread_3 = std::thread([]()
	{
		auto conn = Connect("127.0.0.1", 8080);
		conn->Send(Encode("Hello, World!"));
	});

	server_thread.join();
	client_thread_1.join();
	client_thread_2.join();
	client_thread_3.join();

	return 0;
}
