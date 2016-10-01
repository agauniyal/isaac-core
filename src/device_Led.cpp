#include "device_Led.hpp"
using namespace isaac;


Led::Led(const unsigned int _p, const std::string _n, const std::string _id)
    : Device(_p, _n, _id),
      lastAccess(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count())
{
	logger->info("Led <{}> - pin <{}> constructed", getName(), getPowerPin());
}


Led::Led(const json _j, const std::string _id) : Led(_j.at("powerPin"), _j.at("name"), _id)
{
	if (_j.find("description") != _j.end()) {
		setDescription(_j["description"]);
	}

	if (_j.find("lastAccess") != _j.end()) {
		lastAccess = _j["lastAccess"];
	}
}


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
