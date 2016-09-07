#ifndef DEVICE_LED_HPP
#define DEVICE_LED_HPP

#include "device_driver.hpp"
#include <chrono>

class Led final : public Device_driver {

private:
	std::mutex m_lastAccess;
	std::chrono::time_point<std::chrono::system_clock> lastAccess;

public:
	Led(const unsigned int, const std::string = "");
	bool isOn() { return pinStatus(); }
	bool execute() override { return true; }
	void process() override {}
	bool on() override { return write(true); }
	bool off() override;
};

#endif
