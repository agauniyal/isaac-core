#ifndef DEVICE_FLAMESENSOR_HPP
#define DEVICE_FLAMESENSOR_HPP

#include "device.hpp"
#include <chrono>
namespace isaac {

using namespace std::chrono;

class FlameSensor final : public Device {

private:
	int dataPin;
	std::string readPath;
	std::mutex m_dir;
	std::mutex m_occupied;
	std::mutex m_detect;
	std::mutex m_lastDetected;
	time_point<system_clock> lastDetected;

	FlameSensor(const FlameSensor &) = delete;
	FlameSensor &operator=(const FlameSensor &) = delete;

public:
	FlameSensor(const int, const std::string = "", const std::string = "", const int = 0);

	FlameSensor(const json _j, const std::string _id = "")
	    : FlameSensor(_j.at("powerPin"), _j.at("name"), _id, _j.at("dataPin"))
	{
	}

	bool detect();
	auto active() { return read(); }

	auto getLastDetected() const
	{
		return duration_cast<milliseconds>(lastDetected.time_since_epoch()).count();
	}

	deviceType getType() const override { return deviceType::FlameSensor; }
	json dumpInfo() const override;

	~FlameSensor();
};
}

#endif
