#include "device_TripWire.hpp"
using namespace isaac;
const std::string TripWire::TripWire_PATH = config::getGPIOBasePath();
 int TripWire::getValue()
{
	std::string path;
	path.reserve(40);
	path.append(TripWire_PATH).append("gpio").append(std::to_string(power)).append("/value");

	std::ifstream readStream(path.c_str());
	if (readStream) {
		readStream >> resistance;
		return resistance;
	} else {
		logger->error("Cannot read device <{}>, <{}> is inaccessible", getName(), path);
		throw std::runtime_error("Could not open path to read device");
	}
}
int TripWire::getResistance()
{
	if (flag != true) {
		flag = true;
		Device::on();
		Device::off();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		Device::setDirection(0);
		while (getValue() == 0 && resistance < 2000) resistance += 1;
		flag = false;
	}
	return resistance;
}
