#include "device_Led.hpp"
using namespace isaac;


Led::Led(const unsigned int _p, const std::string _n, const std::string _id)
    : Device(_p, _n, _id), lastAccess(time_point_cast<seconds>(system_clock::now()))
{
	logger->info("Led <{}> - pin <{}> constructed", getName(), getPowerPin());
}


Led::Led(const json _j, const std::string _id) : Led(_j.at("powerPin"), _j.at("name"), _id)
{
	if (_j.find("description") != _j.end()) {
		setDescription(_j["description"]);
	}

	if (_j.find("lastAccess") != _j.end()) {
		auto s = _j["lastAccess"];
		time_point<system_clock, seconds> tp{ seconds{ s } };
		lastAccess = tp;
	}
}


void Led::off()
{
	std::lock_guard<std::mutex> lock(m_lastAccess);
	lastAccess = time_point_cast<seconds>(system_clock::now());
	Device::off();
}


milliseconds::rep Led::getLastAccessed() const
{
	return duration_cast<milliseconds>(lastAccess.time_since_epoch()).count();
}


json Led::dumpInfo() const
{
	auto j          = Device::dumpInfo();
	j["lastAccess"] = lastAccess.time_since_epoch().count();
	j["type"]       = dToInt(deviceType::Led);
	return j;
}
