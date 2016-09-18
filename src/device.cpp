#include "device.hpp"
#include <fstream>
#include <algorithm>

using namespace isaac;

const std::string Device::GPIO_PATH  = gpio::getGPIOBasePath("config.json");
bool Device::occupied[gpio::NumPins] = { 0 };


Device::Device(const unsigned int _p, const std::string _n)
    : powerPin(_p), failState(true), exported(false), name(_n)
{
	if (powerPin > gpio::NumPins) {
		// TODO: LOG error here
	} else {
		name = (name == "") ? "DEFAULT DEVICE NAME" : name;
		std::lock_guard<std::mutex> lock(m_occupied);
		// TODO: generate a unique id here and assign to ID var
		if (!occupied[powerPin]) {
			// TODO: Log pin successfully occupied
			occupied[powerPin] = true;
			failState          = false;
			mount();
			pinIO = readDirection();
		} else {
			// TODO: Log pin already occupied
		}
	}
}


void Device::mount()
{
	if (exported) {
		// TODO: Log 'pin is already exported'
		return;
	}

	if (failState) {
		// TODO: Log 'cannot export, device in failed state'
		return;
	}

	const std::string path = GPIO_PATH + "/export";
	std::ofstream gpioexport(path.c_str(), std::ofstream::trunc);
	if (gpioexport) {
		gpioexport << powerPin;
		failState = false;
		exported  = true;
	} else {
		failState = true;
		exported  = false;
		// TODO: LOG error here
	}
}


void Device::unmount()
{
	if (!exported) {
		// TODO: Log 'pin is already unexported'
		return;
	}

	if (failState) {
		// TODO: Log 'cannot unexport, device in failed state'
		// but don't return yet, try to unexport
	}

	const std::string path = GPIO_PATH + "/unexport";
	std::ofstream gpiounexport(path.c_str(), std::ofstream::trunc);
	if (gpiounexport) {
		gpiounexport << powerPin;
		failState = false;
		exported  = false;
	} else {
		failState = true;
		// TODO: LOG error here
	}
}


bool Device::setDirection(bool _dir)
{
	// 0 => 'in' | 1 => 'out'
	const std::string direction = _dir ? "out" : "in";

	const std::string path = GPIO_PATH + "/gpio" + std::to_string(powerPin) + "/direction";

	std::ofstream dirStream(path.c_str(), std::ofstream::trunc);
	if (dirStream) {
		dirStream << direction;
		failState = false;
	} else {
		failState = true;
		// TODO: LOG error here
		return false;
	}
	return true;
}


bool Device::readDirection()
{
	const std::string path = GPIO_PATH + "/gpio" + std::to_string(powerPin) + "/direction";

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
	}
	return direction;
}


bool Device::on()
{
	const std::string path = GPIO_PATH + "/gpio" + std::to_string(powerPin) + "/value";

	std::ofstream writeStream(path.c_str(), std::ofstream::trunc);
	setDirection(1);
	if (writeStream && exported && !failState) {
		writeStream << 1;
		pinIO = 1;
		return true;
	} else {
		failState = true;
		// TODO: LOG here
		return false;
	}
}

bool Device::off()
{
	const std::string path = GPIO_PATH + "/gpio" + std::to_string(powerPin) + "/value";

	std::ofstream writeStream(path.c_str(), std::ofstream::trunc);
	setDirection(1);
	if (writeStream && exported && !failState) {
		writeStream << 0;
		pinIO = 0;
		return true;
	} else {
		failState = true;
		// TODO: LOG here
		return false;
	}
}


bool Device::read()
{
	const std::string path = GPIO_PATH + "/gpio" + std::to_string(powerPin) + "/value";

	std::ifstream readStream(path.c_str());
	setDirection(false);
	if (readStream && exported && !failState) {
		bool result = false;
		readStream >> result;
		pinIO = result;
	} else {
		failState = true;
		// TODO: LOG here
	}
	return pinIO;
}


void Device::setName(const std::string _n)
{
	std::lock_guard<std::mutex> lock(m_meta);
	name = (_n == "") ? "DEFAULT DEVICE NAME" : _n.substr(0, 50);
}


void Device::setInfo(const json _i)
{
	std::lock_guard<std::mutex> lock(m_meta);
	info = _i;
}


Device::~Device()
{
	std::lock_guard<std::mutex> lock(m_occupied);
	if (occupied[powerPin]) {
		occupied[powerPin] = false;
		unmount();
		// TODO: Log pin successfully removed
	} else {
		// TODO: Log pin wasn't occupied, something's wrong
	}
}
