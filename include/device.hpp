#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <atomic>
#include <mutex>
#include <memory>
#include <json.hpp>
#include <spdlog/spdlog.h>
#include "config.hpp"
#include "deviceType.hpp"

namespace isaac {

using json = nlohmann::json;

class Device {

	unsigned int powerPin;
	char id[9] = { '\0' };

	static const std::string GPIO_PATH;

	// make sure to guard against n > config::gpioNumPins
	static bool occupied[config::gpioNumPins];
	std::mutex m_occupied;

	std::string name;
	json description;

	std::mutex m_dir;
	std::mutex m_name;
	std::mutex m_desc;

	void mount();
	void unmount();

	Device(const Device &) = delete;
	Device &operator=(const Device &) = delete;

protected:
	static const std::shared_ptr<spdlog::logger> logger;

	// 0 => 'in' | 1 => 'out'
	void setDirection(bool);
	bool getDirection() const;
	unsigned int getPowerPin() const { return powerPin; }

	// pinNumber and deviceName
	Device(const unsigned int, const std::string = "", const std::string = "");
	Device(const json _j, const std::string _id = "") : Device(_j["powerPin"], _j["name"], _id) {}

public:
	std::string getId() const { return id; }
	std::string getName() const { return name; }
	json getDescription() const { return description; }

	bool setName(const std::string = "DEFAULT DEVICE NAME");
	void setDescription(const json = json::object());

	virtual json dumpInfo() const;
	virtual deviceType getType() const { return deviceType::Base; }

	// both on() and off() calls are not thread safe
	virtual void on();
	virtual void off();

	virtual bool read();

	static bool isOccupied(const unsigned int _p)
	{
		return (_p > config::gpioNumPins) ? false : occupied[_p];
	}
	static void configure() { logger->set_pattern(" %c - [%l][%t] \"%v\" "); }

	virtual ~Device();
};
}

#endif
