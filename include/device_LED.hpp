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
	Led(const unsigned int _p, const std::string _n = "", const std::string _id = "")
	    : Device(_p, _n, _id),
	      lastAccess(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count())
	{
		logger->info("Led <{}> - pin <{}> constructed", getName(), getPowerPin());
	}

	Led(const json _j, const std::string _id = "") : Led(_j["powerPin"], _j["name"], _id)
	{
		if (_j.find("description") != _j.end()) {
			setDescription(_j["description"]);
		}

		if (_j.find("lastAccess") != _j.end()) {
			lastAccess = _j["lastAccess"];
		}
	}

	void off() override;
	auto isOn() { return read(); }
	long getLastAccessed() const;
	deviceType getType() const override { return deviceType::Led; }
	json dumpInfo() const override;
};
}

#endif
