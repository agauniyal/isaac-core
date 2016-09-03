#include <iostream>
#include <uWS.h>
#include "device.hpp"

using namespace uWS;

int main()
{
	Device D1(4);
	try {
		EventSystem es(MASTER);
		Server server(es, 3000, PERMESSAGE_DEFLATE, 0);

		server.onConnection([](WebSocket socket) {});
		server.onMessage([](WebSocket socket, char *message, size_t length, OpCode opCode) {
			socket.send(message, length, opCode);
		});
		server.onDisconnection([](WebSocket socket, int code, char *message, size_t length) {});

		es.run();
	} catch (...) {
		std::cerr << "ERR_LISTEN" << std::endl;
	}
}
