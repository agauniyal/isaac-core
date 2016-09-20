#include "gpio.hpp"
#include <fstream>
#include <sstream>
#include <json.hpp>
#include <stdexcept>

using namespace isaac;

std::string gpio::getGPIOBasePath(const std::string _file)
{
	using json = nlohmann::json;

	std::ifstream config(_file);
	std::string path = "";
	if (config) {
		std::stringstream buffer;
		buffer << config.rdbuf();
		auto json_config = json::parse(buffer);
		return json_config["path"];

	} else {
		throw std::runtime_error("cannot read config.json");
	}
}
