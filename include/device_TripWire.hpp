#ifndef DEVICE_TRIPWIRE_HPP
#define DEVICE_TRIPWIRE_HPP

#include "device.hpp"
#include <chrono>
#include <atomic>
#include <uv.h>

namespace isaac {

using namespace std::chrono;

class TripWire final : public Device {

private:
	int maxCycles;
	std::atomic<bool> state;
	uv_timer_t t1_handle;

	std::mutex m_trip;
	std::mutex m_lastBreak;

	time_point<system_clock> lastBreak;

	static void cb(uv_timer_t *t)
	{
		TripWire *fL = static_cast<TripWire *>(t->data);
		fL->detect();
	}

	TripWire(const TripWire &) = delete;
	TripWire &operator=(const TripWire &) = delete;

public:
	TripWire(const int, const std::string = "", const std::string = "", const int = 200);

	TripWire(const json _j, const std::string _id = "")
	    : TripWire(_j.at("powerPin"), _j.at("name"), _id, _j.at("maxCycles"))
	{
	}

	bool getState() const { return state; }
	void detect() override;
	void reset() { state = false; }

	auto getLastBreak() const
	{
		return duration_cast<milliseconds>(lastBreak.time_since_epoch()).count();
	}

	deviceType getType() const override { return deviceType::TripWire; }
	json dumpInfo() const override;
};
}

#endif
