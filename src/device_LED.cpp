#include "device_LED.hpp"
using namespace isaac;


void Led::off()
{
	std::lock_guard<std::mutex> lock(m_lastAccess);
	lastAccess = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	Device::off();
}


long Led::getLastAccessed() const { return lastAccess; }


json Led::dumpInfo() const
{
	auto j          = Device::dumpInfo();
	j["lastAccess"] = lastAccess;
	j["type"]       = dToInt(deviceType::Led);
	return j;
}
