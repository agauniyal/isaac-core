#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

namespace isaac {

namespace config {
	constexpr unsigned int gpioNumPins = 40;
	std::string getGPIOBasePath(const std::string = "config.json");
	std::string getJsonDBPath(const std::string = "config.json");
	std::string getLogPath(const std::string = "config.json");
	std::string getTempSensor(const std::string = "config.json");
}
}

#endif
