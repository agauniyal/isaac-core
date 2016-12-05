#include <fstream>
#include "uv.h"
#include "device_FlameSensor.hpp"
#include "deviceList.hpp"


using namespace isaac;


FlameSensor::FlameSensor(const int _p, const std::string _n, const std::string _id, const int _d)
    : Device(_p, _n, _id),
      dataPin(_d),
      lastDetected(time_point_cast<milliseconds>(system_clock::now()))
{
	if (!occupyPin(dataPin)) {
		logger->error("FlameSensor <{}> - Datapin <{}> could not be created\n", _n, _d);
		throw std::runtime_error("Error: dataPin cannot be occupied");
	}

	// export dataPin
	std::string path;
	path.reserve(40);
	path.append(GPIO_PATH).append("export");

	std::ofstream gpioexport(path.c_str(), std::ofstream::trunc);
	if (gpioexport) {
		gpioexport << dataPin;
	} else {
		throw std::runtime_error("Could not open path to export");
	}
	path.clear();

	// set dataPin to read mode
	path.append(GPIO_PATH).append("gpio").append(std::to_string(dataPin)).append("/direction");
	std::ofstream dirStream(path.c_str(), std::ofstream::trunc);
	if (dirStream) {
		dirStream << "in";
		logger->info("FlameSensor <{}> set on read mode", getName());
	} else {
		logger->error("Cannot set direction on <{}>, <{}> is inaccessible", getName(), path);
		throw std::runtime_error("Could not set FlameSensor to read mode");
	}

	readPath.reserve(40);
	readPath.append(GPIO_PATH).append("gpio").append(std::to_string(dataPin)).append("/value");

	state = false;

	uv_timer_init(deviceList::getLoop(), &t1_handle);
	uv_timer_start(&t1_handle, &cb, 0, 1000);
	t1_handle.data = this;

	logger->info("FlameSensor <{}> - dataPin <{}> constructed", getName(), dataPin);
}


void FlameSensor::detect()
{
	bool result = false;
	if (m_detect.try_lock()) {
		std::ifstream readStream(readPath.c_str());
		if (readStream) {
			readStream >> result;
		}
	}
	if (result) {
		state        = true;
		lastDetected = time_point_cast<milliseconds>(system_clock::now());
	}
}


json FlameSensor::dumpInfo() const
{
	auto j            = Device::dumpInfo();
	j["dataPin"]      = dataPin;
	j["lastDetected"] = lastDetected.time_since_epoch().count();
	j["type"]         = dToInt(deviceType::FlameSensor);
	return j;
}

FlameSensor::~FlameSensor()
{
	std::string path;
	path.reserve(25);
	path.append(GPIO_PATH).append("unexport");

	std::ofstream gpiounexport(path.c_str(), std::ofstream::trunc);
	if (gpiounexport) {
		gpiounexport << dataPin;
	} else {
		logger->error("{} - Could not open path to unexport during destruction", getName());
	}

	occupyPin(dataPin, false);
	logger->info("FlameSensor <{}> - <{}> removed", getName(), dataPin);
}
