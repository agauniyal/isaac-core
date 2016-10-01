#ifndef DEVICE_LED_HPP
#define DEVICE_LED_HPP

#include "device.hpp"
#include <chrono>

namespace isaac {

using namespace std::chrono;

class Led final : public Device {

private:
	std::mutex m_lastAccess;
	milliseconds::rep lastAccess;

	Led(const Led &) = delete;
	Led &operator=(const Led &) = delete;

public:
	Led(const unsigned int, const std::string = "", const std::string = "");
	Led(const json, const std::string = "");

	void off() override;
	auto isOn() { return read(); }
	long getLastAccessed() const;
	deviceType getType() const override { return deviceType::Led; }
	json dumpInfo() const override;
};
}

#endif
