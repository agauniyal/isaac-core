#include "device.hpp"
#include <fstream>
#include <algorithm>

using namespace isaac;

const std::string Device::GPIO_PATH  = gpio::getGPIOBasePath("config.json");
bool Device::occupied[gpio::NumPins] = { 0 };

const std::shared_ptr<spdlog::logger> Device::logger
  = spdlog::rotating_logger_mt("D_Logger", "deviceLogs", 1048576 * 5, 3);


Device::Device(const unsigned int _p, const std::string _n, const std::string _id)
    : powerPin(_p), failState(true), exported(false)
{
	_id.copy(id, 8);
	if (_n.size() == 0) {
		name = "DEFAULT DEVICE NAME";
	} else {
		name = (_n.size() < 50) ? _n : _n.substr(0, 50);
	}

	if (powerPin > gpio::NumPins) {
		logger->error("Pin <{}> is not valid", powerPin);
	} else {

		// only 1 thread can enter following section at a time
		{
			std::lock_guard<std::mutex> lock(m_occupied);
			if (!occupied[powerPin]) {
				occupied[powerPin] = true;
				failState          = false;
			}
		}

		if (!failState) {
			mount();
			pinIO = readDirection();
			logger->info("Device <{}> with pin <{}> mounted", name, powerPin);
		} else {
			logger->error("Device <{}> with pin <{}> cannot be created", name, powerPin);
		}
	}
}


void Device::mount()
{
	if (exported) {
		logger->warn("Pin <{}> is already exported", powerPin);
		return;
	}

	if (failState) {
		logger->error("Cannot mount device <{}>, because it has failed", name);
		return;
	}

	std::string path;
	path.reserve(25);
	path.append(GPIO_PATH).append("/export");

	std::ofstream gpioexport(path.c_str(), std::ofstream::trunc);
	if (gpioexport) {
		gpioexport << powerPin;
		failState = false;
		exported  = true;
	} else {
		failState = true;
		exported  = false;
		logger->error("Cannot mount device <{}>, <{}> is inaccessible", name, path);
	}
}


void Device::unmount()
{
	if (!exported) {
		logger->warn("Pin <{}> is already unexported", powerPin);
		return;
	}

	if (failState) {
		logger->error("Cannot unmount device <{}>, because it has failed", name);
		// Don't return yet, try to unexport
	}

	std::string path;
	path.reserve(25);
	path.append(GPIO_PATH).append("/unexport");

	std::ofstream gpiounexport(path.c_str(), std::ofstream::trunc);
	if (gpiounexport) {
		gpiounexport << powerPin;
		failState = false;
		exported  = false;
	} else {
		failState = true;
		logger->error("Cannot unmount device <{}>, <{}> is inaccessible", name, path);
	}
}


bool Device::setDirection(bool _dir)
{
	// 0 => 'in' | 1 => 'out'
	const std::string direction = _dir ? "out" : "in";

	std::string path;
	path.reserve(40);
	path.append(GPIO_PATH).append("/gpio").append(std::to_string(powerPin)).append("/direction");

	std::ofstream dirStream(path.c_str(), std::ofstream::trunc);
	if (dirStream) {
		dirStream << direction;
		failState = false;
	} else {
		failState = true;
		logger->error("Cannot set direction on device <{}>, <{}> is inaccessible", name, path);
		return false;
	}
	return true;
}


bool Device::readDirection()
{
	std::string path;
	path.reserve(40);
	path.append(GPIO_PATH).append("/gpio").append(std::to_string(powerPin)).append("/direction");

	bool direction = true;  // "out"
	std::string input(3, ' ');
	std::ifstream dirStream(path.c_str());
	if (dirStream) {
		dirStream >> input;
		if (input == "in") {
			direction = false;  // "in"
		}
	} else {
		failState = true;
		logger->error("Cannot read direction of device <{}>, <{}> is inaccessible", name, path);
	}
	return direction;
}


bool Device::on()
{
	std::string path;
	path.reserve(40);
	path.append(GPIO_PATH).append("/gpio").append(std::to_string(powerPin)).append("/value");

	std::ofstream writeStream(path.c_str(), std::ofstream::trunc);
	setDirection(1);
	if (writeStream && exported && !failState) {
		writeStream << 1;
		pinIO = 1;
		logger->info("Device <{}> turned ON", name);
		return true;
	} else {
		failState = true;
		logger->error("Cannot turn on device <{}>, <{}> is inaccessible", name, path);
		return false;
	}
}

bool Device::off()
{
	std::string path;
	path.reserve(40);
	path.append(GPIO_PATH).append("/gpio").append(std::to_string(powerPin)).append("/value");

	std::ofstream writeStream(path.c_str(), std::ofstream::trunc);
	setDirection(1);
	if (writeStream && exported && !failState) {
		writeStream << 0;
		pinIO = 0;
		return true;
	} else {
		failState = true;
		logger->error("Cannot turn off device <{}>, <{}> is inaccessible", name, path);
		return false;
	}
}


bool Device::read()
{
	std::string path;
	path.reserve(40);
	path.append(GPIO_PATH).append("/gpio").append(std::to_string(powerPin)).append("/value");

	std::ifstream readStream(path.c_str());
	setDirection(false);
	if (readStream && exported && !failState) {
		bool result = false;
		readStream >> result;
		pinIO = result;
	} else {
		failState = true;
		logger->error("Cannot read device <{}>, <{}> is inaccessible", name, path);
	}
	return pinIO;
}


void Device::setName(const std::string _n)
{
	if (_n.size() == 0) {
		return;
	}
	const auto oldName = name;
	{
		std::lock_guard<std::mutex> lock(m_meta);
		name = (_n.size() < 50) ? _n : _n.substr(0, 50);
	}
	logger->info("<{}> name changed to <{}>", oldName, name);
}


void Device::setInfo(const json _i)
{
	const auto oldInfo = info;
	{
		std::lock_guard<std::mutex> lock(m_meta);
		info = _i;
	}
	logger->info("Device <{}> info\n{}\nchanged to\n{}", name, oldInfo.dump(4), info.dump(4));
}


Device::~Device()
{
	std::lock_guard<std::mutex> lock(m_occupied);
	if (occupied[powerPin]) {
		occupied[powerPin] = false;
		unmount();
		logger->info("Device <{}> removed along with pin <{}>", name, powerPin);
	} else {
		logger->critical("Pin number <{}> wasn't occupied by device <{}>", powerPin, name);
	}
}
