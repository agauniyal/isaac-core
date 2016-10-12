#include "device_TripWire.hpp"

using namespace isaac;

const std::string TripWire::TripWire_PATH = config::getGPIOBasePath();


bool TripWire::intrusion()
{
	
	if (m_trip.try_lock()) {
		int cycles = 0;
		Device::on();
		Device::off();
		
		Device::setDirection(0);
		
		while (read() == 0 && cycles < maxcycles) {
			++cycles;
		}
		m_trip.unlock();
	
		return (cycles==maxcycles)?true:false;
	}
	return false;
}

