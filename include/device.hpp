#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "utils.hpp"
#include <atomic>
#include <mutex>
#include <string>


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
	std::string info;
	std::mutex m_meta;

	Device(const Device &) = delete;
	Device &operator=(const Device &) = delete;

public:
	// Pin number and Device name
	Device(const unsigned int, const std::string = "");

	bool write(bool);
	bool read();

	std::string getId() const { return id; }
	bool hasFailed() const { return failState; }
	bool isMounted() const { return exported; }
	bool pinStatus() const { return pinIO; }

	// 0 => 'in' | 1 => 'out'
	bool setDirection(bool);
	bool getDirection();

	void mount();
	void unmount();

	void setName(const std::string);
	void setInfo(const std::string);
	std::string getName() const { return name; }
	std::string getInfo() const { return info; }

	~Device();
};

#endif
