#include "config.hpp"
#include <fstream>
#include <sstream>
#include <json.hpp>
#include <stdexcept>

using namespace isaac;

std::string config::getGPIOBasePath(const std::string _file)
{
	using json = nlohmann::json;

	std::ifstream config(_file);
	std::string path = "";
	if (config) {
		std::stringstream buffer;
		buffer << config.rdbuf();
		auto json_config = json::parse(buffer);
		return json_config["gpio_path"];

	} else {
		throw std::runtime_error("cannot read config.json");
	}
}

std::string config::getJsonDBPath(const std::string _file)
{
	using json = nlohmann::json;

	std::ifstream config(_file);
	std::string path = "";
	if (config) {
		std::stringstream buffer;
		buffer << config.rdbuf();
		auto json_config = json::parse(buffer);
		return json_config["db_path"];

	} else {
		throw std::runtime_error("cannot read config.json");
	}
}

std::string config::getLogPath(const std::string _file)
{
	using json = nlohmann::json;

	std::ifstream config(_file);
	std::string path = "";
	if (config) {
		std::stringstream buffer;
		buffer << config.rdbuf();
		auto json_config = json::parse(buffer);
		return json_config["log_path"];

	} else {
		throw std::runtime_error("cannot read config.json");
	}
}
