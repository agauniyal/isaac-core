#include "app.hpp"
#include <iostream>
#include <string>
#include <uWS.h>

using namespace uWS;

int main()
{
	try {
		EventSystem es(MASTER);
		Server server(es, 3000, PERMESSAGE_DEFLATE, 0);
		server.onConnection([](WebSocket socket) {});

		server.onMessage([](WebSocket socket, char *message, size_t length, OpCode opCode) {
			socket.send(message, length, opCode);
			std::cout << doSomething() << std::endl;
		});
		server.onDisconnection([](WebSocket socket, int code, char *message, size_t length) {});

		es.run();
	}
	catch (...) {
		std::cout << "ERR_LISTEN" << std::endl;
	}

	return 0;
}
