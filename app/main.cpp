#include "device_Led.hpp"
#include <iostream>
#include <uWS.h>

using namespace uWS;

int main(int argc, char *argv[])
{
	if (argc != 2) {
		std::cerr << "Run program as 'program <port>'\n";
		return -1;
	}

	auto portNum = atoi(argv[1]);

	Hub h;

	h.onMessage([](WebSocket<SERVER> ws, const char *message, size_t length, OpCode op) {
		ws.send(message, length, op);
	});

	auto listening = h.listen(portNum);
	if (!listening) {
		std::cerr << "Listening to port " << portNum << " failed" << std::endl;
		return -1;
	}
	h.run();
}
