#include "device_Buzzer.hpp"

using namespace isaac;


Buzzer::Buzzer(const unsigned int _p, const std::string _n, const std::string _id)
    : Device(_p, _n, _id)
{
	logger->info("Buzzer <{}> - pin <{}> constructed", getName(), getPowerPin());
}


Buzzer::Buzzer(const json _j, const std::string _id) : Buzzer(_j.at("powerPin"), _j.at("name"), _id)
{
	if (_j.find("description") != _j.end()) {
		setDescription(_j["description"]);
	}
}


void Buzzer::on()
{
	if (m_power.try_lock()) {
		Device::on();
		m_power.unlock();
	}
}


void Buzzer::off()
{
	if (m_power.try_lock()) {
		Device::off();
		m_power.unlock();
	}
}


json Buzzer::dumpInfo() const
{
	auto j    = Device::dumpInfo();
	j["type"] = dToInt(deviceType::Buzzer);
	return j;
}
