#ifndef DEVICE_LED_HPP
#define DEVICE_LED_HPP

#include "device.hpp"
#include <chrono>

namespace isaac {
class Led final : public Device {

private:
	std::mutex m_lastAccess;
	std::chrono::time_point<std::chrono::system_clock> lastAccess;

	Led(const Led &) = delete;
	Led &operator=(const Led &) = delete;

public:
	Led(const unsigned int _p, const std::string _d = "")
	    : Device(_p, _d), lastAccess(std::chrono::system_clock::now())
	{
		// TODO: Log construction of LED here
	}
	bool isOn() { return read(); }
	bool off() override;
	bool execute() override { return true; }
	void process() override {}
	auto getLastAccessed() const { return lastAccess; }
};
}

#endif
