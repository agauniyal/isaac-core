#ifndef DEVICE_BUZZER_HPP
#define DEVICE_BUZZER_HPP

#include "device.hpp"
#include <chrono>

namespace isaac {

using namespace std::chrono;

class Buzzer final : public Device {

private:
	std::mutex m_lastAccess;
	milliseconds::rep lastAccess;

	Buzzer(const Buzzer &) = delete;
	Buzzer &operator=(const Buzzer &) = delete;

public:
	Buzzer(const unsigned int, const std::string = "", const std::string = "");
	Buzzer(const json, const std::string = "");

	void off() override;
	auto isOn() { return read(); }
	long getLastAccessed() const;
	deviceType getType() const override { return deviceType::Buzzer; }
	json dumpInfo() const override;
};
}

#endif
