#include "device_LED.hpp"
using namespace isaac;

inline bool Led::off()
{
	std::lock_guard<std::mutex> lock(m_lastAccess);
	lastAccess = std::chrono::system_clock::now();
	return Device::off();
}
