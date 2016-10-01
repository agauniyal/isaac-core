#include "device_Led.hpp"
#include <iostream>
#include <uWS.h>

using namespace isaac;

int main()
{

	try {
		using namespace uWS;
		EventSystem es(MASTER);
		Server server(es, 3000, PERMESSAGE_DEFLATE, 0);

		server.onConnection([](WebSocket socket) {});
		server.onMessage([](WebSocket socket, char *message, size_t length, OpCode opCode) {
			socket.send(message, length, opCode);
		});
		server.onDisconnection([](WebSocket socket, int code, char *message, size_t length) {});

		es.run();
	} catch (const std::exception &ex) {
		std::cerr << ex.what() << std::endl;
	}
}
