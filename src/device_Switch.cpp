#include "device_Switch.hpp"
#include <cmath>

using namespace isaac;

Switch::Switch(const int _p, const std::string _n, const std::string _id, const float _pC)
    : Device(_p, _n, _id),
      runTime(0s),
      lastOn(time_point_cast<seconds>(system_clock::now())),
      powerConsumption(std::abs(_pC))
{
	logger->info("Switch <{}> - pin <{}> constructed", getName(), getPowerPin());
}


Switch::Switch(const json _j, const std::string _id) : Switch(_j.at("powerPin"), _j.at("name"), _id)
{
	if (_j.find("description") != _j.end()) {
		setDescription(_j["description"]);
	}

	if (_j.find("powerConsumption") != _j.end()) {
		setPowerConsumption(_j["powerConsumption"]);
	}

	if (_j.find("lastOn") != _j.end()) {
		auto s = _j["lastOn"];
		time_point<system_clock, seconds> tp{ seconds{ s } };
		lastOn = tp;
	}
}

void Switch::setPowerConsumption(float _p) { powerConsumption = std::abs(_p); }


void Switch::on()
{
	std::lock_guard<std::mutex> lock(m_lastOn);
	lastOn = time_point_cast<seconds>(system_clock::now());
	Device::on();
}


void Switch::off()
{
	if (!isOn()) {
		return;
	}
	std::lock_guard<std::mutex> lock(m_runTime);
	auto now = duration_cast<seconds>(system_clock::now().time_since_epoch());
	runTime += now - lastOn.time_since_epoch();
	Device::off();
}


json Switch::dumpInfo() const
{
	auto j                = Device::dumpInfo();
	j["lastOn"]           = duration_cast<milliseconds>(lastOn.time_since_epoch()).count();
	j["powerConsumption"] = powerConsumption;
	j["runTime"]          = getRunTime();
	j["type"]             = dToInt(deviceType::Switch);
	return j;
}
