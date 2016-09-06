#include "device_LED.hpp"


Led::Led(const unsigned int _p, const std::string _d)
    : Device(_p, _d), lastAccess(std::chrono::system_clock::now())
{
	// TODO: Log construction of LED here
}


bool Led::off()
{
	std::lock_guard<std::mutex> lock(m_lastAccess);
	lastAccess = std::chrono::system_clock::now();
	return write(false);
}
