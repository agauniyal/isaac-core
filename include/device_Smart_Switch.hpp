#ifndef DEVICE_SMART_SWITCH_HPP
#define DEVICE_SMART_SWITCH_HPP

#include "device.hpp"
#include <chrono>


namespace isaac {

using namespace std::chrono;

class SmartSwitch final : public Device {

private:
	std::mutex m_runTime;
	seconds runTime;
	std::mutex m_lastOn;
	time_point<system_clock, seconds> lastOn;
	float powerConsumption;

	SmartSwitch(const SmartSwitch &) = delete;
	SmartSwitch &operator=(const SmartSwitch &) = delete;

public:
	SmartSwitch(
	  const unsigned int, const std::string = "", const std::string = "", const float = 0.0);
	SmartSwitch(const json, const std::string = "");

	void on() override;
	void off() override;
	auto isOn() { return read(); }
	seconds::rep getRunTime() const { return runTime.count(); }
	void resetRunTime() { runTime = 0s; }
	float getPowerConsumption() const { return powerConsumption; }
	void setPowerConsumption(float = 0.0);

	deviceType getType() const override { return deviceType::SmartSwitch; }
	json dumpInfo() const override;
};
}

#endif
