#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "config.hpp"
#include "deviceType.hpp"
#include <json.hpp>
#include <memory>
#include <mutex>
#include <utility>
#include <unordered_map>
#include <functional>
#include <spdlog/spdlog.h>

namespace isaac {

using json = nlohmann::json;

class Device {

	int powerPin;
	char id[config::idLength + 1] = {};

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
	static const std::string GPIO_PATH;
	static const std::shared_ptr<spdlog::logger> logger;

	std::unordered_map<std::string, std::pair<std::string, std::string>> events;

	// 0 => 'in' | 1 => 'out'
	void setDirection(bool);
	bool getDirection() const;
	int getPowerPin() const { return powerPin; }
	bool occupyPin(const int, const bool = true);

	// pinNumber and deviceName and id
	Device(const int, const std::string = "", const std::string = "");

public:
	std::unordered_map<std::string, std::function<void()>> methods;

	std::string getId() const { return id; }
	std::string getName() { return name; }
	json getDescription() const { return description; }

	bool setName(const std::string = "DEFAULT DEVICE NAME");
	void setDescription(const json = json::object());

	virtual json dumpInfo() const;
	virtual deviceType getType() const { return deviceType::Base; }

	// both on() and off() calls are not thread safe
	virtual void on();
	virtual void off();
	virtual void detect() {}

	virtual bool read();
	auto isOn() { return read(); }

	static bool isOccupied(const int _p)
	{
		return (_p > config::gpioNumPins || _p < 1) ? false : occupied[_p];
	}

	virtual ~Device();
};
}

#endif
