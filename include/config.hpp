#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

namespace isaac {

namespace config {

	constexpr int gpioNumPins = 41;

	std::string getGPIOBasePath();
	std::string getJsonDBPath();
	std::string getLogPath();
	std::string getTempSensor();
}
}

#endif
