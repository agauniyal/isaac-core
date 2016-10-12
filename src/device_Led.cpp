#include "device_Led.hpp"

using namespace isaac;


Led::Led(const int _p, const std::string _n, const std::string _id) : Device(_p, _n, _id)
{
	logger->info("Led <{}> - pin <{}> constructed", getName(), getPowerPin());
}


Led::Led(const json _j, const std::string _id) : Led(_j.at("powerPin"), _j.at("name"), _id)
{
	if (_j.find("description") != _j.end()) {
		setDescription(_j["description"]);
	}
}


void Led::on()
{
	if (m_power.try_lock()) {
		Device::on();
		m_power.unlock();
	}
}


void Led::off()
{
	if (m_power.try_lock()) {
		Device::off();
		m_power.unlock();
	}
}


json Led::dumpInfo() const
{
	auto j    = Device::dumpInfo();
	j["type"] = dToInt(deviceType::Led);
	return j;
}
