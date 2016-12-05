#include "deviceList.hpp"
#include <iostream>
#include <uWS.h>
#include <thread>

using namespace uWS;
using namespace isaac;

int main(int argc, char *argv[])
{
	if (argc != 2) {
		std::cerr << "Run program as 'program <port>'\n";
		return -1;
	}

	auto portNum = atoi(argv[1]);

	Hub h;

	 h.onConnection([](uWS::WebSocket<uWS::CLIENT> ws, uWS::UpgradeInfo ui) {
        std::cout << "CLIENT OPEN" << std::endl;
    });

    h.onDisconnection([](uWS::WebSocket<uWS::CLIENT> ws, int code, char *message, size_t length) {
        std::cout << "CLIENT CLOSE: " << code << std::endl;
    });

	h.onMessage([](WebSocket<SERVER> ws, const char *message, size_t length, OpCode op) {
		std::string s(message);
		std::cout << message << std::endl;
		ws.send("Done\0", 5, op);
	});

	h.onError([](void *user) {
        std::cout << "FAILURE: Connection failed! Timeout?" << std::endl;
        exit(-1);
    });

	auto listening = h.listen(portNum);
	if (!listening) {
		std::cerr << "Listening to port " << portNum << " failed" << std::endl;
		return -1;
	}

	deviceList::init();
	deviceList list;


	std::thread t1([]() { deviceList::run(); });

	h.run();
	t1.join();
}
