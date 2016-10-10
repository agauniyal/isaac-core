#include "device_buzzer.hpp"
using namespace isaac;


Buzzer::Buzzer(const unsigned int _p, const std::string _n, const std::string _id)
    : Device(_p, _n, _id),
      lastAccess(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count())
{
	logger->info("Buzzer <{}> - pin <{}> constructed", getName(), getPowerPin());
}


Buzzer::Buzzer(const json _j, const std::string _id) : Buzzer(_j.at("powerPin"), _j.at("name"), _id)
{
	if (_j.find("description") != _j.end()) {
		setDescription(_j["description"]);
	}

	if (_j.find("lastAccess") != _j.end()) {
		lastAccess = _j["lastAccess"];
	}
}


void Buzzer::off()
{
	std::lock_guard<std::mutex> lock(m_lastAccess);
	lastAccess = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	Device::off();
}


long Buzzer::getLastAccessed() const { return lastAccess; }


json Buzzer::dumpInfo() const
{
	auto j          = Device::dumpInfo();
	j["lastAccess"] = lastAccess;
	j["type"]       = dToInt(deviceType::Buzzer);
	return j;
}
