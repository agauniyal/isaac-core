#include "device_Smart_Switch.hpp"
#include <cmath>

using namespace isaac;

SmartSwitch::SmartSwitch(
  const unsigned int _p, const std::string _n, const std::string _id, const float _pC)
    : Device(_p, _n, _id),
      runTime(0s),
      lastOn(time_point_cast<seconds>(system_clock::now())),
      powerConsumption(std::abs(_pC))
{
	logger->info("SmartSwitch <{}> - pin <{}> constructed", getName(), getPowerPin());
}


SmartSwitch::SmartSwitch(const json _j, const std::string _id)
    : SmartSwitch(_j.at("powerPin"), _j.at("name"), _id)
{
	if (_j.find("powerConsumption") != _j.end()) {
		setPowerConsumption(_j["powerConsumption"]);
	}
	if (_j.find("lastOn") != _j.end()) {
		auto s = _j["lastOn"];
		time_point<system_clock, seconds> tp{ seconds{ s } };
		lastOn = tp;
	}
}

void SmartSwitch::setPowerConsumption(float _p) { powerConsumption = std::abs(_p); }


void SmartSwitch::on()
{
	std::lock_guard<std::mutex> lock(m_lastOn);
	lastOn = time_point_cast<seconds>(system_clock::now());
	Device::on();
}


void SmartSwitch::off()
{
	if (!isOn()) {
		return;
	}
	std::lock_guard<std::mutex> lock(m_runTime);
	auto now = duration_cast<seconds>(system_clock::now().time_since_epoch());
	runTime += now - lastOn.time_since_epoch();
	Device::off();
}


json SmartSwitch::dumpInfo() const
{
	auto j                = Device::dumpInfo();
	j["lastOn"]           = lastOn.time_since_epoch().count();
	j["powerConsumption"] = powerConsumption;
	j["type"]             = dToInt(deviceType::Led);
	return j;
}
