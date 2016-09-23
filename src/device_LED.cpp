#include "device_LED.hpp"
using namespace isaac;


void Led::off()
{
	std::lock_guard<std::mutex> lock(m_lastAccess);
	lastAccess = std::chrono::system_clock::now();
	Device::off();
}


auto Led::getLastAccessed() const
{
	return lastAccess.time_since_epoch() / std::chrono::milliseconds(1);
}


json Led::dumpInfo() const
{
	auto j          = Device::dumpInfo();
	j["lastAccess"] = lastAccess.time_since_epoch() / std::chrono::milliseconds(1);
	return j;
}
