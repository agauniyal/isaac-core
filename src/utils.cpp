#include "utils.hpp"
#include <fstream>

std::string gpio::getGPIOBasePath(const std::string file)
{
	std::ifstream config(file);
	std::string path = "";
	if (config) {
		std::getline(config, path);
	} else {
		return "raspi/path";
	}
	return path;
}
