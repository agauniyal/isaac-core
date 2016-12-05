#ifndef DEVICE_FLAMESENSOR_HPP
#define DEVICE_FLAMESENSOR_HPP

#include <chrono>
#include <atomic>
#include "device.hpp"

namespace isaac {

using namespace std::chrono;

class FlameSensor final : public Device {

private:
	int dataPin;
	std::atomic<bool> state;
	std::string readPath;

	uv_timer_t t1_handle;

	std::mutex m_dir;
	std::mutex m_occupied;
	std::mutex m_detect;
	std::mutex m_lastDetected;

	time_point<system_clock> lastDetected;


	static void cb(uv_timer_t *t)
	{
		FlameSensor *fL = static_cast<FlameSensor *>(t->data);
		fL->detect();
	}

	FlameSensor(const FlameSensor &) = delete;
	FlameSensor &operator=(const FlameSensor &) = delete;

public:
	FlameSensor(const int, const std::string = "", const std::string = "", const int = 0);

	FlameSensor(const json _j, const std::string _id = "")
	    : FlameSensor(_j.at("powerPin"), _j.at("name"), _id, _j.at("dataPin"))
	{
	}

	bool getState() const { return state; }
	void detect() override;
	void reset() { state = false; }

	auto getLastDetected() const
	{
		return duration_cast<milliseconds>(lastDetected.time_since_epoch()).count();
	}

	deviceType getType() const override { return deviceType::FlameSensor; }
	json dumpInfo() const override;

	~FlameSensor();
};
}

#endif
