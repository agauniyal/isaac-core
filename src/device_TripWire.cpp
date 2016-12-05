#include "device_TripWire.hpp"
#include "deviceList.hpp"

using namespace isaac;


TripWire::TripWire(const int _p, const std::string _n, const std::string _id, const int _c)
    : Device(_p, _n, _id), maxCycles(_c), lastBreak(time_point_cast<seconds>(system_clock::now()))
{
	if (maxCycles < 1) {
		logger->error("TripWire <{}> maxCycles must be +ve integer", getName());
		throw std::invalid_argument("Supplied argument for maxCycles isn't valid");
	}

	state = false;

	uv_timer_init(deviceList::getLoop(), &t1_handle);
	uv_timer_start(&t1_handle, &cb, 0, 1000);
	t1_handle.data = this;

	logger->info("TripWire <{}> - pin <{}> constructed", getName(), getPowerPin());
}


void TripWire::detect()
{
	if (m_trip.try_lock()) {
		int cycles = 0;
		Device::off();
		Device::setDirection(0);
		while (read() == 0 && cycles < maxCycles) {
			++cycles;
		}
		Device::on();
		m_trip.unlock();
		state = (cycles == maxCycles) ? true : false;
	}
}


json TripWire::dumpInfo() const
{
	auto j         = Device::dumpInfo();
	j["maxCycles"] = maxCycles;
	j["lastBreak"] = lastBreak.time_since_epoch().count();
	j["type"]      = dToInt(deviceType::TripWire);
	return j;
}
