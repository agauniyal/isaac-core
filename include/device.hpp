#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "gpio.hpp"
#include <atomic>
#include <mutex>
#include <json.hpp>

namespace isaac {

using json = nlohmann::json;

class Device {

	unsigned int powerPin;
	std::atomic<bool> failState;
	std::atomic<bool> exported;
	std::string id;

	// 0 => 'in' | 1 => 'out'
	std::atomic<bool> pinIO;

	static const std::string GPIO_PATH;

	// make sure to guard against n > gpio::NumPins
	static bool occupied[gpio::NumPins];
	std::mutex m_occupied;

	std::string name;
	json info;
	std::mutex m_meta;

	Device(const Device &) = delete;
	Device &operator=(const Device &) = delete;

protected:
	virtual void mount();
	virtual void unmount();

	// 0 => 'in' | 1 => 'out'
	bool setDirection(bool);
	bool readDirection();

	// pinNumber and deviceName
	Device(const unsigned int, const std::string = "");

public:
	std::string getId() const { return id; }

	bool hasFailed() const { return failState; }
	bool isMounted() const { return exported; }
	virtual bool isBad() const { return !isMounted() && hasFailed(); }

	std::string getName() const { return name; }
	json getInfo() const { return info; }
	bool pinIOStatus() const { return pinIO; }

	void setName(const std::string = "DEFAULT DEVICE NAME");
	void setInfo(const json = json::object());

	static bool isOccupied(const unsigned int _p)
	{
		return (_p > gpio::NumPins) ? false : occupied[_p];
	}

	virtual bool on();
	virtual bool off();
	virtual bool read();

	virtual bool execute() = 0;
	virtual void process() = 0;

	virtual ~Device();
};
}

#endif
