#ifndef DEVICE_DRIVER_HPP
#define DEVICE_DRIVER_HPP

#include "device.hpp"

class Device_driver : private Device {

	Device_driver(const Device_driver &) = delete;
	Device_driver &operator=(const Device_driver &) = delete;

protected:
	Device_driver(const unsigned int _p, const std::string _n = "") : Device(_p, _n) {}

public:
	using Device::getId;
	using Device::setName;
	using Device::setInfo;
	using Device::getName;
	using Device::getInfo;
	using Device::hasFailed;
	using Device::pinStatus;
	using Device::write;
	using Device::read;

	bool isBad() { return !isMounted() && hasFailed(); }

	virtual bool on()      = 0;
	virtual bool off()     = 0;
	virtual bool execute() = 0;
	virtual void process() = 0;

	virtual ~Device_driver(){};
};

#endif
