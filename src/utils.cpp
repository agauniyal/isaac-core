#include "utils.hpp"
#include <fstream>

std::string gpio::getGPIOBasePath(const std::string _file)
{
	std::ifstream config(_file);
	std::string path = "";
	if (config) {
		std::getline(config, path);
	} else {
		return "debug/raspi/path";
	}
	return path;
}
