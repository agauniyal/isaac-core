#ifndef DEVICE_LED_HPP
#define DEVICE_LED_HPP

#include "device.hpp"

namespace isaac {

using namespace std::chrono;

class Led final : public Device {

private:
	std::mutex m_power;

	Led(const Led &) = delete;
	Led &operator=(const Led &) = delete;

public:
	Led(const int, const std::string = "", const std::string = "");
	Led(const json, const std::string = "");

	void on() override;
	void off() override;
	deviceType getType() const override { return deviceType::Led; }
	json dumpInfo() const override;
};
}

#endif
