#include "device_TripWire.hpp"

using namespace isaac;

const std::string TripWire::TripWire_PATH = config::getGPIOBasePath();


int TripWire::getResistance()
{
	int cycles = 0;
	if (m_trip.try_lock()) {
		Device::on();
		Device::off();
		// std::this_thread::sleep_for(std::chrono::milliseconds(10));
		Device::setDirection(0);
		int pin = getPowerPin();
		while (read() == 0 && cycles < 2000) {
			++cycles;
		}
		m_trip.unlock();
	}
	return cycles;
}
