#ifndef DEVICE_BUZZER_HPP
#define DEVICE_BUZZER_HPP

#include "device.hpp"

namespace isaac {

class Buzzer final : public Device {

private:
	std::mutex m_power;
	Buzzer(const Buzzer &) = delete;
	Buzzer &operator=(const Buzzer &) = delete;

public:
	Buzzer(const int, const std::string = "", const std::string = "");
	Buzzer(const json, const std::string = "");

	void on() override;
	void off() override;
	auto isOn() { return read(); }
	deviceType getType() const override { return deviceType::Buzzer; }
	json dumpInfo() const override;
};
}

#endif
