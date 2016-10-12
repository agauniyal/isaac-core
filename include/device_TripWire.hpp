#ifndef DEVICE_TRIPWIRE_HPP
#define DEVICE_TRIPWIRE_HPP

#include "device.hpp"
#include <chrono>
namespace isaac {
class TripWire final : public Device {

private:
	std::mutex m_trip;
	std::mutex m_lastBreak;
	std::chrono::time_point<std::chrono::system_clock> lastBreak;
	int maxcycles;
	TripWire(const TripWire &) = delete;
	TripWire &operator=(const TripWire &) = delete;

	static const std::string TripWire_PATH;

public:
	TripWire(const unsigned int _p, const std::string _n = "", const std::string _id = "",const int _c=200)
	    : Device(_p, _n, _id),maxcycles(_c)
	{
		logger->info("TripWire <{}> - pin <{}> constructed", getName(), getPowerPin());
	}

	TripWire(const json _j, const std::string _id = "")
	    : TripWire(_j.at("powerPin"), _j.at("name"), _id)
	{
		
	}

	bool intrusion();
	deviceType getType() const override { return deviceType::TripWire; }
};
}

#endif
