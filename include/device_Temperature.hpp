#ifndef DEVICE_TEMP_HPP
#define DEVICE_TEMP_HPP

#include "device.hpp"
#include <chrono>
#include <fstream>

namespace isaac {
class TempSensor final : public Device {

private:
	int currTemperature;
	bool busy;
	std::mutex m_getTemp;
	std::string folderName;
	static const std::string TEMPSEN_PATH;
	std::chrono::milliseconds::rep delayTime;

	TempSensor(const TempSensor &) = delete;
	TempSensor &operator=(const TempSensor &) = delete;

public:
	TempSensor(const unsigned int, const std::string = "", const std::string = "",
	  const std::string = "", std::chrono::milliseconds::rep = 0);

	TempSensor(const json _j, const std::string _id = "")
	    : TempSensor(_j["powerPin"], _j["name"], _id, _j["folderName"], _j["delayTime"])
	{
	}

	int getTemperature();

	deviceType getType() const override { return deviceType::TempSensor; }
	auto getDelay() const { return delayTime; }
	json dumpInfo() const override;
};
}

#endif
