#include "device.hpp"
#include <algorithm>
#include <fstream>

using namespace isaac;

bool Device::occupied[config::gpioNumPins] = { 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1,
	0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0 };

const std::string Device::GPIO_PATH = config::getGPIOBasePath();

const std::shared_ptr<spdlog::logger> Device::logger
  = spdlog::rotating_logger_mt("D_Logger", config::getLogPath() + "device", 1048576 * 5, 3);


Device::Device(const unsigned int _p, const std::string _n, const std::string _id) : powerPin(_p)
{
	if (powerPin > config::gpioNumPins) {
		logger->error("Pin <{}> is not valid", powerPin);
		throw std::invalid_argument("pin number cannot exceed gpio::NumPins");
	}

	if (_id.size() == 8) {
		_id.copy(id, 8);
	} else {
		throw std::invalid_argument("id length must be of 8 characters");
	}

	if (_n.size() != 0 && _n.size() <= 50) {
		name.assign(_n);
	} else {
		throw std::invalid_argument("name length must be in between 1 and 50 characters");
	}

	// only 1 thread can enter following section at a time
	{
		std::lock_guard<std::mutex> lock(m_occupied);
		if (!occupied[powerPin]) {
			occupied[powerPin] = true;
		} else {
			logger->info(
			  "Device <{}> - pin <{}> cannot be created, pin already occupied", name, powerPin);
			throw std::runtime_error("pin already occupied");
		}
	}

	try {
		mount();
		logger->info("Device <{}> with pin <{}> mounted", name, powerPin);
	} catch (std::runtime_error &e) {
		logger->error("Device <{}> - pin <{}> could not be created\n{}", name, powerPin, e.what());
		throw std::runtime_error("Error: Device cannot be created");
	}
}


void Device::mount()
{
	std::string path;
	path.reserve(25);
	path.append(GPIO_PATH).append("export");

	std::ofstream gpioexport(path.c_str(), std::ofstream::trunc);
	if (gpioexport) {
		gpioexport << powerPin;
	} else {
		throw std::runtime_error("Could not open path to export");
	}
}


void Device::unmount()
{
	std::string path;
	path.reserve(25);
	path.append(GPIO_PATH).append("unexport");

	std::ofstream gpiounexport(path.c_str(), std::ofstream::trunc);
	if (gpiounexport) {
		gpiounexport << powerPin;
	} else {
		throw std::runtime_error("Could not open path to unexport");
	}
}


void Device::setDirection(bool _dir)
{
	// 0 => 'in' | 1 => 'out'
	const std::string direction = _dir ? "out" : "in";

	std::string path;
	path.reserve(40);
	path.append(GPIO_PATH).append("gpio").append(std::to_string(powerPin)).append("/direction");

	std::ofstream dirStream(path.c_str(), std::ofstream::trunc);
	if (dirStream) {
		{
			std::lock_guard<std::mutex> lock(m_dir);
			dirStream << direction;
		}
		logger->info("Set direction on device <{}> - <{}>", name, direction);
	} else {
		logger->error("Cannot set direction on device <{}>, <{}> is inaccessible", name, path);
		throw std::runtime_error("Could not open path to set direction");
	}
}


void Device::on()
{
	std::string path;
	path.reserve(40);
	path.append(GPIO_PATH).append("gpio").append(std::to_string(powerPin)).append("/value");

	std::ofstream writeStream(path.c_str(), std::ofstream::trunc);
	setDirection(1);
	if (writeStream) {
		writeStream << 1;
		logger->info("Device <{}> turned ON", name);
	} else {
		logger->error("Cannot turn on device <{}>, <{}> is inaccessible", name, path);
		throw std::runtime_error("Could not open path to turn on device");
	}
}

void Device::off()
{
	std::string path;
	path.reserve(40);
	path.append(GPIO_PATH).append("gpio").append(std::to_string(powerPin)).append("/value");

	std::ofstream writeStream(path.c_str(), std::ofstream::trunc);
	setDirection(1);
	if (writeStream) {
		writeStream << 0;
		logger->info("Device <{}> turned OFF", name);
	} else {
		logger->error("Cannot turn off device <{}>, <{}> is inaccessible", name, path);
		throw std::runtime_error("Could not open path to turn off device");
	}
}


bool Device::read()
{
	std::string path;
	path.reserve(40);
	path.append(GPIO_PATH).append("gpio").append(std::to_string(powerPin)).append("/value");

	std::ifstream readStream(path.c_str());
	setDirection(0);
	if (readStream) {
		bool result = false;
		readStream >> result;
		return result;
	} else {
		logger->error("Cannot read device <{}>, <{}> is inaccessible", name, path);
		throw std::runtime_error("Could not open path to read device");
	}
}


bool Device::setName(const std::string _n)
{
	if (_n.size() != 0 && _n.size() <= 50) {
		auto oldName = name;
		{
			std::lock_guard<std::mutex> lock(m_name);
			name = _n;
		}
		logger->info("Device <{}> name changed to <{}>", oldName, name);
		return true;
	} else {
		return false;
	}
}


void Device::setDescription(const json _j)
{
	const auto oldDesc = description;
	{
		std::lock_guard<std::mutex> lock(m_desc);
		description = _j;
	}
	logger->info("Device <{}>:\n{}\nchanged to\n{}", name, oldDesc.dump(4), description.dump(4));
}


json Device::dumpInfo() const
{
	json info           = json::object();
	info["powerPin"]    = powerPin;
	info["id"]          = id;
	info["name"]        = name;
	info["description"] = description;
	info["type"]        = dToInt(deviceType::Base);
	return info;
}


Device::~Device()
{
	try {
		unmount();
	} catch (std::runtime_error &e) {
		logger->error("{}", e.what());
	}

	std::lock_guard<std::mutex> lock(m_occupied);
	if (occupied[powerPin]) {
		occupied[powerPin] = false;
		logger->info("Device <{}> - <{}> removed", name, powerPin);
	} else {
		logger->critical("Pin number <{}> wasn't occupied by device <{}>", powerPin, name);
	}
}
