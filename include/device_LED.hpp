#ifndef DEVICE_LED_HPP
#define DEVICE_LED_HPP

#include "device.hpp"
#include <chrono>

namespace isaac {
class Led final : public Device {

private:
	std::mutex m_lastAccess;
	std::chrono::time_point<std::chrono::system_clock> lastAccess;

	Led(const Led &) = delete;
	Led &operator=(const Led &) = delete;

public:
	Led(const unsigned int _p, const std::string _n = "", const std::string _id = "")
	    : Device(_p, _n, _id), lastAccess(std::chrono::system_clock::now())
	{
		logger->info("Led <{}> - pin <{}> constructed", getName(), getPowerPin());
	}

	Led(const json _j, const std::string _id = "") : Led(_j["powerPin"], _j["name"], _id) {}

	void off() override;
	auto isOn() { return read(); }
	long getLastAccessed() const;
	deviceType getType() const override { return deviceType::Led; }
	json dumpInfo() const override;

	bool execute() override { return true; }
	void process() override {}
};
}

#endif
