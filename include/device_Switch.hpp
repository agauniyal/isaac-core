#ifndef DEVICE_SMART_SWITCH_HPP
#define DEVICE_SMART_SWITCH_HPP

#include "device.hpp"
#include <chrono>


namespace isaac {

using namespace std::chrono;

class Switch final : public Device {

private:
	std::mutex m_runTime;
	seconds runTime;
	std::mutex m_lastOn;
	time_point<system_clock, seconds> lastOn;
	float powerConsumption;

	Switch(const Switch &) = delete;
	Switch &operator=(const Switch &) = delete;

public:
	Switch(const int, const std::string = "", const std::string = "", const float = 0.0);
	Switch(const json, const std::string = "");

	void on() override;
	void off() override;
	auto isOn() { return read(); }

	seconds::rep getRunTime() const { return runTime.count(); }
	void resetRunTime() { runTime = 0s; }

	auto getLastOn() const
	{
		return duration_cast<milliseconds>(lastOn.time_since_epoch()).count();
	}

	float getPowerConsumption() const { return powerConsumption; }
	void setPowerConsumption(float = 0.0);

	deviceType getType() const override { return deviceType::Switch; }
	json dumpInfo() const override;
};
}

#endif
