#include "device.hpp"
#include <chrono>

class Led : private Device {

private:
	std::mutex m_lastAccess;
	std::chrono::time_point<std::chrono::system_clock> lastAccess;

public:
	Led(const unsigned int, const std::string = "");
	bool on() { return write(true); }
	bool off();
	bool isOn() { return getStatus(); }
};
