#ifndef DEVICE_TRIPWIRE_HPP
#define DEVICE_TRIPWIRE_HPP

#include "device.hpp"
#include <chrono>
#include <fstream>
namespace isaac {
class TripWire final : public Device {

private:
	std::mutex m_lastAccess;
	std::chrono::time_point<std::chrono::system_clock> lastAccess;
	//std::string folderName;
	TripWire(const TripWire &) = delete;
	TripWire &operator=(const TripWire &) = delete;
	static const std::string TripWire_PATH;  // done
	//std::string sensorFileName;  // done
	bool flag;  // done
	const int power;
	//std::chrono::milliseconds::rep delayTime;  // done
	int resistance;  // done

public:
	TripWire(const unsigned int _p, const std::string _n = "", const std::string _id = "")
	    : Device(_p, _n, _id),power(_p)
	{

	}

	TripWire(const json _j, const std::string _id = "") : TripWire(_j["powerPin"], _j["name"], _id) {}
	int getResistance();  // done
	int getValue();
	deviceType getType() const override { return deviceType::TripWireSensor; }  // done


};
}

#endif
