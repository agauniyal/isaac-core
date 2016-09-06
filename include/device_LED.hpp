#include "device.hpp"
#include <chrono>

class Led final : public Device {

private:
	std::mutex m_lastAccess;
	std::chrono::time_point<std::chrono::system_clock> lastAccess;

public:
	Led(const unsigned int, const std::string = "");
	bool isOn() { return pinStatus(); }
	bool on() override { return write(true); }
	bool off() override;
};
